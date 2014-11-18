
// Load spine skeleton from file
spSkeleton @skeleton = @spSkeleton(":/data/spineboy.json", ":/data/spineboy.atlas", 0.5f);

// Drawing batch
Batch @batch = @Batch();

// Current player animation
spAnimation @currentAnim;
spAnimationState @animationState;
spAnimationState @shootAnim;

// Player position
Vector2 position(200, 600);

void main()
{
	Console.log("** Spine Example **");
	
	spAnimationStateData @data = @spAnimationStateData(@skeleton);
	data.setMix("idle", "walk", 0.2f);
	data.setMix("walk", "idle", 0.5f);
	
	@animationState = @spAnimationState(@data);
	@shootAnim = @spAnimationState(@data);
	
	animationState.looping = true;
	animationState.setAnimation("idle");
}

bool lmb = false;
bool walk = false;

void update()
{
	// Movement
	if(Input.getKeyState(KEY_D))
	{
		if(!walk) {
			animationState.setAnimation("walk");
			walk = true;
		}
		position.x += 5;
		skeleton.setFlipX(false);
	}else if(Input.getKeyState(KEY_A)){
		if(!walk) {
			animationState.setAnimation("walk");
			walk = true;
		}
		position.x -= 5;
		skeleton.setFlipX(true);
	}else{
		if(walk) {
			animationState.setAnimation("idle");
		}
		walk = false;
	}
	
	if(Input.getKeyState(KEY_LMB))
	{
		if(!lmb) {
			shootAnim.setAnimation("shoot");
		}
		lmb = true;
	}else{
		lmb = false;
	}
	
	// Update skeleton and animation
	skeleton.setPosition(position);
	animationState.update(Graphics.dt);
	shootAnim.update(Graphics.dt);
}

void draw()
{
	// Draw skeleton to batch
	skeleton.draw(@batch);
	batch.draw();
	batch.clear();
}
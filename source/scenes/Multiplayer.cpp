#include "Multiplayer.h"
#include "SceneManager.h"

#include "Gui/UiObject.h"
#include "Gui/Button.h"
#include "Gui/LineEdit.h"
#include "Gui/Canvas.h"

#include "Constants.h"

MultiplayerScene::MultiplayerScene()
{
	LOG("Scene: Multiplayer");
	
	// Setup canvas
	canvas = new Canvas(800, 600);
	canvas->update();
	
	LineEdit *ipEdit = new LineEdit(canvas);
	ipEdit->setText("127.0.0.1");
	ipEdit->setAnchor(Vector2(0.5f, 0.5f));
	ipEdit->setPosition(Vector2(-0.2f, -0.1f));
	ipEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(ipEdit);
	
	LineEdit *portEdit = new LineEdit(canvas);
	portEdit->setText("5555");
	portEdit->setAnchor(Vector2(0.5f, 0.5f));
	portEdit->setPosition(Vector2(0.2f, -0.1f));
	portEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(portEdit);
	
	Button *joinBtn = new Button("Join", function<void()>(bind(&MultiplayerScene::join, this)), canvas);
	joinBtn->setAnchor(Vector2(0.5f, 1.0f));
	joinBtn->setPosition(Vector2(0.2f, -0.1f));
	joinBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(joinBtn);
	
	Button *hostBtn = new Button("Host", function<void()>(bind(&MultiplayerScene::host, this)), canvas);
	hostBtn->setAnchor(Vector2(0.5f, 1.0f));
	hostBtn->setPosition(Vector2(-0.2f, -0.1f));
	hostBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(hostBtn);
}
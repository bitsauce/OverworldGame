#include "Multiplayer.h"
#include "SceneManager.h"

#include "Gui/UiObject.h"
#include "Gui/Button.h"
#include "Gui/LineEdit.h"
#include "Gui/Canvas.h"

#include "Constants.h"

void MultiplayerScene::showEvent()
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
	
	LineEdit *portEdit = new LineEdit(canvas);
	portEdit->setText("5555");
	portEdit->setAnchor(Vector2(0.5f, 0.5f));
	portEdit->setPosition(Vector2(0.2f, -0.1f));
	portEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	
	Button *joinBtn = new Button("Join", function<void()>(bind(&MultiplayerScene::join, this)), canvas);
	joinBtn->setAnchor(Vector2(0.5f, 1.0f));
	joinBtn->setPosition(Vector2(0.2f, -0.1f));
	joinBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	
	Button *hostBtn = new Button("Host", function<void()>(bind(&MultiplayerScene::host, this)), canvas);
	hostBtn->setAnchor(Vector2(0.5f, 1.0f));
	hostBtn->setPosition(Vector2(-0.2f, -0.1f));
	hostBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));

	m_sceneObjects.push_back(hostBtn);
}

void MultiplayerScene::hideEvent()
{
	for(uint i = 0; i < m_sceneObjects.size(); ++i)
		delete m_sceneObjects[i];
	m_sceneObjects.clear();
}
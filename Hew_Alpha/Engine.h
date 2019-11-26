#pragma once
#include "WindowContainer.h"
#include "Timer.h"
#include "Network/Network.h"
#include "Scene/Title.h"
#include "Scene/Lobby.h"
#include "Scene/Game1.h"
#include "Scene/Game2.h"
	
class Engine :WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	void Set(int index);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
private:
	Timer timer;
	Network* network;
	SceneMgr* pScene;
};
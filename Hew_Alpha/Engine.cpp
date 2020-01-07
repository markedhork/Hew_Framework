#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	//this->DisableCursor();
	timer.Start();
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}
	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
	{
		return false;
	}

	this->Set(TITLE_NUM);
	return true;
}

void Engine::Set(int index)
{
	switch (index)
	{
	case TITLE_NUM:			//Title
		pScene = new Title;
		break;
	case LOBBY1_NUM:			//Lobby
		pScene = new Lobby;
		this->network = new Server;

		break;
	case LOBBY2_NUM:			//Lobby
		pScene = new Lobby;
		this->network = new Client;
		break;
	case GAME1_NUM:			//Game1
		pScene = new Game1;
		break;
	case GAME2_NUM:			//Game2
		pScene = new Game2;
		break;
	default:
		break;
	}

	pScene->Initialize(&this->gfx, &this->keyboard, &this->mouse, &this->timer, this->network);
	pScene->Set();
	this->oldScene = index;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	this->newScene = pScene->Update();
	if (this->newScene != this->oldScene)
	{
		this->Set(this->newScene);
	}
}

void Engine::RenderFrame()
{
	this->pScene->Draw();
}
#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->DisableCursor();
	timer.Start();
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}
	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
	{
		return false;
	}

	this->Set(3);
	return true;
}

void Engine::Set(int index)
{
	switch (index)
	{
	case 0:			//Title
		pScene = new Title;
		break;
	case 1:			//Lobby
		pScene = new Lobby;
		break;
	case 2:			//Game1
		pScene = new Game1;
		this->network = new Server;
		break;
	case 3:			//Game2
		pScene = new Game2;
		this->network = new Client;
		break;
	default:
		break;
	}

	pScene->Initialize(&this->gfx, &this->keyboard, &this->mouse, &this->timer, this->network);
	pScene->Set();
}

bool Engine::ProcessMessages()
{

	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	pScene->Update();
}

void Engine::RenderFrame()
{
	this->pScene->Draw();
}
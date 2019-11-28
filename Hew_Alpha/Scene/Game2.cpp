#include "Game2.h"
Sprite Game2_sprite[] = {
	{D3DXVECTOR3(0,0,-0.2),D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1),1},
	//{D3DXVECTOR3(0,0,5),D3DXVECTOR3(0,0,0),D3DXVECTOR3(10,10,1),1}
};

// 読み込みテクスチャ数
static const int SPRITE_COUNT_G2 = sizeof(Game2_sprite) / sizeof(Game2_sprite[0]);

bool Game2::Set()
{
	this->gfx->Set(Game2_sprite, SPRITE_COUNT_G2);
	this->gfx->camera.SetPosition(0, 0, -3.0f);
	return true;
}

bool Game2::Update()
{

	float dt = this->timer->GetMilisecondsElapsed();
	this->timer->Restart();

	while (!this->keyboard->CharBufferIsEmpty())
	{
		unsigned char ch = this->keyboard->ReadChar();
		//std::string outmsg = "Char: ";
		//outmsg += ch;
		//outmsg += "\n";
		//OutputDebugStringA(outmsg.c_str());
	}

	while (!this->keyboard->KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = this->keyboard->ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	//this->gfx.model.AdjustRotation(0.0f, 0.001f*dt, 0.0f);
	   
	while (!this->mouse->EventBufferIsEmpty())
	{
		MouseEvent me = this->mouse->ReadEvent();
		if (this->mouse->IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				this->gfx->camera.AdjustRotation((float)me.GetPosX()*0.01f, (float)me.GetPosY()*0.01f, 0.0f);
			}
		}
	}

	const float cameraSpeed = 0.01f;


	//if (this->keyboard->KeyIsPressed(VK_UP))
	if(this->mouse->IsLeftDoubleClick())
	{
		this->gfx->camera.AdjustPosition(this->gfx->camera.GetForwardVector()*cameraSpeed*dt);
	}
	if (this->keyboard->KeyIsPressed(VK_DOWN))
	{
		this->gfx->camera.AdjustPosition(this->gfx->camera.GetBackwardVector()*cameraSpeed*dt);
	}
	if (this->keyboard->KeyIsPressed(VK_LEFT))
	{
		this->gfx->camera.AdjustPosition(this->gfx->camera.GetLeftVector()*cameraSpeed*dt);
	}
	if (this->keyboard->KeyIsPressed(VK_RIGHT))
	{
		this->gfx->camera.AdjustPosition(this->gfx->camera.GetRightVector()*cameraSpeed*dt);
	}
	if (this->keyboard->KeyIsPressed(VK_SPACE))
	{
		this->gfx->camera.AdjustPosition(0.0f, cameraSpeed*dt, 0.0f);
	}
	if (this->keyboard->KeyIsPressed('N'))
	{
		this->gfx->camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
	}

	return true;
}

bool Game2::Draw()
{
	this->gfx->RenderFrame();

	return true;
}


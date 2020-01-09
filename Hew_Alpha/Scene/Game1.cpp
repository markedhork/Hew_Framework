#include "Game1.h"

// position.xyz , rotation.xyz , size.xyz , TextureIndex number
Sprite Game1_sprite[] = {
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BG},
};
Mesh Game1_mesh[] = {
	{D3DXVECTOR3(0,0,-1),D3DXVECTOR3(0,180,0),D3DXVECTOR3(1,1,1),MESH_INDEX_PLAYER},
};
// 読み込みテクスチャ数
static const int SPRITE_COUNT_G1 = sizeof(Game1_sprite) / sizeof(Game1_sprite[0]);
static const int MESH_COUNT_G1 = sizeof(Game1_mesh) / sizeof(Game1_mesh[0]);

bool Game1::Set()
{
	this->gfx->Set(Game1_sprite, SPRITE_COUNT_G1, Game1_mesh, MESH_COUNT_G1);
	this->gfx->camera.SetPosition(PLAYER_POS.x, PLAYER_POS.y + 1.0f, PLAYER_POS.z - 3.0f);
	this->gfx->camera.SetRotation(0, 0, 0);
	this->player.x = PLAYER_POS.x;
	this->player.y = PLAYER_POS.y;
	this->player.z = PLAYER_POS.z;

	this->handhold.SetDevice(this->gfx->GetDevice());
	this->handhold.CreateMeshBuffer();

	return true;
}

int Game1::Update()
{
	float dt = this->timer->GetMilisecondsElapsed();
	this->timer->Restart();



	if (this->network->IfMsgFromClient())
	{
		CLIENT_MSG temp;
		temp = this->network->GetMsgFromClient();
		this->handhold.holders[this->handhold.activedTotal].use = true;
		this->handhold.holders[this->handhold.activedTotal].px = temp.x;
		this->handhold.holders[this->handhold.activedTotal].py = temp.y;
		this->handhold.activedTotal++;
	}


	this->recPlayer = this->player;

	while (!this->keyboard->CharBufferIsEmpty())
	{
		unsigned char ch = this->keyboard->ReadChar();
	}

	while (!this->keyboard->KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = this->keyboard->ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}



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

	//temp
	static int tempcount = 0;
	if (this->keyboard->KeyIsTrigger('Q'))
	{
		for (int i = tempcount; i < MAX_HOLDER; i++)
		{
			if (this->handhold.holders[i].use == true)
			{
				PLAYER_POS.x = this->handhold.holders[i].px;
				PLAYER_POS.y = this->handhold.holders[i].py;
				tempcount++;
				break;
			}
		}
	}
	else if (this->keyboard->KeyIsTrigger('W'))
	{


	}
	else if (this->keyboard->KeyIsTrigger('E'))
	{


	}
	else if (this->keyboard->KeyIsTrigger('A'))
	{


	}
	else if (this->keyboard->KeyIsTrigger('D'))
	{


	}
	else if (this->keyboard->KeyIsTrigger('Z'))
	{


	}
	else if (this->keyboard->KeyIsTrigger('X'))
	{


	}
	else if (this->keyboard->KeyIsTrigger('C'))
	{


	}



	this->player.x = PLAYER_POS.x;
	this->player.y = PLAYER_POS.y;

	const float cameraSpeed = 0.01f;
	D3DXVECTOR3 camPos = this->gfx->camera.GetPositionVector();

	if (camPos != D3DXVECTOR3(this->player.x, this->player.y + 1.0f, PLAYER_POS.z - 3.0f))
	{
		float distance = sqrtf((this->player.x - camPos.x) * (this->player.x - camPos.x) +
			(this->player.y + 1.0f - camPos.y) * (this->player.y + 1.0f - camPos.y));
		if (distance < CAMERA_SPD)
		{
			this->gfx->camera.SetPosition(this->player.x, this->player.y + 1.0f, PLAYER_POS.z - 3.0f);
		}
		else
		{
			float tmpSin = (this->player.y + 1.0f - camPos.y) / distance;
			float tmpCos = (this->player.x - camPos.x) / distance;
			this->gfx->camera.AdjustPosition(CAMERA_SPD*tmpCos, CAMERA_SPD*tmpSin, 0.0f);
		}
	}


	//if moved
	if (this->player.x != recPlayer.x || player.y != recPlayer.y)
	{
		
		SERVER_MSG msg;
		msg.px = PLAYER_POS.x;
		msg.py = PLAYER_POS.y;
		this->network->Send(&msg);
	}

	if (this->keyboard->KeyIsPressed(VK_UP))
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

	return GAME1_NUM;
}

bool Game1::Draw()
{
	this->gfx->RenderFrame();

	this->handhold.Draw();

	this->gfx->RenderFrame_end();

	return true;
}

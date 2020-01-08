#include "Game1.h"

// position.xyz , rotation.xyz , size.xyz , TextureIndex number
Sprite Game1_sprite[] = {
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BG},
};
Mesh Game1_mesh[] = {
	{D3DXVECTOR3(0,0,-0.2),D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1),MESH_INDEX_PLAYER},
};
// 読み込みテクスチャ数
static const int SPRITE_COUNT_G1 = sizeof(Game1_sprite) / sizeof(Game1_sprite[0]);
static const int MESH_COUNT_G1 = sizeof(Game1_mesh) / sizeof(Game1_mesh[0]);

bool Game1::Set()
{
	this->gfx->Set(Game1_sprite, SPRITE_COUNT_G1, Game1_mesh, MESH_COUNT_G1);
	this->gfx->camera.SetPosition(0, 0, -3.0f);
	this->gfx->camera.SetRotation(0, 0, 0);
	this->player.PX = 2;
	this->player.PY = 0;
	return true;
}

int Game1::Update()
{
	float dt = this->timer->GetMilisecondsElapsed();
	this->timer->Restart();

	this->recPlayer.PX = this->player.PX;
	this->recPlayer.PY = this->player.PY;

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

	if (this->keyboard->KeyIsTrigger('Q'))
	{
		this->player.PX--;
		this->player.PY++;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.x--;
			PLAYER_STATE.pos.y++;
		}
		else
		{
			this->player.PX++;
			this->player.PY--;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);
	}
	else if (this->keyboard->KeyIsTrigger('W'))
	{
		this->player.PY++;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.y++;
		}
		else
		{
			this->player.PY--;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);

	}
	else if (this->keyboard->KeyIsTrigger('E'))
	{
		this->player.PX++;
		this->player.PY++;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.x++;
			PLAYER_STATE.pos.y++;
		}
		else
		{
			this->player.PX--;
			this->player.PY--;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);

	}
	else if (this->keyboard->KeyIsTrigger('A'))
	{
		this->player.PX--;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.x--;
		}
		else
		{
			this->player.PX++;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);

	}
	else if (this->keyboard->KeyIsTrigger('D'))
	{
		this->player.PX++;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.x++;
		}
		else
		{
			this->player.PX--;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);

	}
	else if (this->keyboard->KeyIsTrigger('Z'))
	{
		this->player.PX--;
		this->player.PY--;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.x--;
			PLAYER_STATE.pos.y--;
		}
		else
		{
			this->player.PX++;
			this->player.PY++;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);

	}
	else if (this->keyboard->KeyIsTrigger('X'))
	{
		this->player.PY--;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.y--;
		}
		else
		{
			this->player.PY++;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);

	}
	else if (this->keyboard->KeyIsTrigger('C'))
	{
		this->player.PX++;
		this->player.PY--;
		if (this->player.PY >= 0 && this->player.PX >= 0 && this->player.PX < MAP_X
			&& Build[this->player.PY / MAP_Y] > 0
			&& map[Build[this->player.PY / MAP_Y] - 1][this->player.PY % MAP_Y][this->player.PX] == 1)
		{
			PLAYER_STATE.pos.x++;
			PLAYER_STATE.pos.y--;
		}
		else
		{
			this->player.PX--;
			this->player.PY++;
		}
		this->gfx->camera.SetPosition(PLAYER_STATE.pos.x, PLAYER_STATE.pos.y, -5.0f);

	}
	
	if (this->player.PX != recPlayer.PX || player.PY != recPlayer.PY)
	{
		int pos[] = { player.PX ,player.PY };
		this->network->Send(pos);
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


	this->gfx->RenderFrame_end();

	return true;
}

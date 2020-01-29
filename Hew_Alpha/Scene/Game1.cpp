#include "Game1.h"

// position.xyz , rotation.xyz , size.xyz , TextureIndex number
Sprite Game1_sprite[] = {
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BG},

};
Mesh Game1_mesh[] = {
	{D3DXVECTOR3(0,-0.5f,-1),D3DXVECTOR3(0,180,0),D3DXVECTOR3(1,1,1),MESH_INDEX_PLAYER},
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
	this->player.y = PLAYER_POS.y + 1.5f;
	this->player.z = PLAYER_POS.z;

	this->nextPos.x = this->player.x;
	this->nextPos.y = this->player.y;
	this->nextPos.z = this->player.z;

	this->recPos.x = this->player.x;
	this->recPos.y = this->player.y;
	this->recPos.z = this->player.z;

	this->handhold.SetDevice(this->gfx->GetDevice());
	this->handhold.CreateMeshBuffer();

	D3DXCreateFont(this->gfx->GetDevice(), 100, 0, FW_ULTRABOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Comic Sans MS", &pDXfont);

	return true;
}

int Game1::Update()
{
	this->gfx->hight = (PLAYER_POS.y + 0.5f)*10;
	float dt = this->timer->GetMilisecondsElapsed();
	if (dt > 1000.0f)
	{
		this->timer->Restart();
		CountDown--;
	}

	if (this->Rdy&&this->ReDistance == false)
	{
		this->handhold.CheckArea(this->player.x, this->player.y);
		this->ReDistance = true;
	}

	if (this->network->IfMsgFromClient())
	{
		CLIENT_MSG temp;
		temp = this->network->GetMsgFromClient();
		this->handhold.holders[this->handhold.activedTotal].use = true;
		this->handhold.holders[this->handhold.activedTotal].px = temp.x;
		this->handhold.holders[this->handhold.activedTotal].py = temp.y;
		this->handhold.holders[this->handhold.activedTotal].distance = sqrtf((temp.y - this->player.y)*(temp.y - this->player.y)
			+ (temp.x - this->player.x)*(temp.x - this->player.x));
		this->handhold.holders[this->handhold.activedTotal].degree = atan2f(temp.y - this->player.y, temp.x - this->player.x) * 180 / D3DX_PI;

		this->handhold.CheckSolo(this->handhold.activedTotal);
		this->handhold.activedTotal++;
		if (this->handhold.activedTotal >= MAX_HOLDER)
		{
			this->handhold.activedTotal = MAX_HOLDER;
		}
	}



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


	if (this->Rdy&&this->ReDistance == true)
	{
		AlreadySend = false;
		if (this->keyboard->KeyIsTrigger('Q'))
		{
			if (this->handhold.targets[3].use == true)
			{
				nextPos.x = this->handhold.targets[3].px;
				nextPos.y = this->handhold.targets[3].py;
				this->Rdy = false;
				this->ReDistance = false;
			}

		}
		else if (this->keyboard->KeyIsTrigger('W'))
		{
			if (this->handhold.targets[2].use == true)
			{
				nextPos.x = this->handhold.targets[2].px;
				nextPos.y = this->handhold.targets[2].py;
				this->Rdy = false;
				this->ReDistance = false;
			}

		}
		else if (this->keyboard->KeyIsTrigger('E'))
		{
			if (this->handhold.targets[1].use == true)
			{
				nextPos.x = this->handhold.targets[1].px;
				nextPos.y = this->handhold.targets[1].py;
				this->Rdy = false;
				this->ReDistance = false;
			}

		}
		else if (this->keyboard->KeyIsTrigger('A'))
		{
			if (this->handhold.targets[4].use == true)
			{
				nextPos.x = this->handhold.targets[4].px;
				nextPos.y = this->handhold.targets[4].py;
				this->Rdy = false;
				this->ReDistance = false;
			}

		}
		else if (this->keyboard->KeyIsTrigger('D'))
		{
			if (this->handhold.targets[0].use == true)
			{
				nextPos.x = this->handhold.targets[0].px;
				nextPos.y = this->handhold.targets[0].py;
				this->Rdy = false;
				this->ReDistance = false;
			}

		}
		else if (this->keyboard->KeyIsTrigger('Z'))
		{
			if (this->handhold.targets[5].use == true)
			{
				nextPos.x = this->handhold.targets[5].px;
				nextPos.y = this->handhold.targets[5].py;
				this->Rdy = false;
				this->ReDistance = false;
			}

		}
		else if (this->keyboard->KeyIsTrigger('X'))
		{
			if (this->handhold.targets[6].use == true)
			{
				nextPos.x = this->handhold.targets[6].px;
				nextPos.y = this->handhold.targets[6].py;
				this->Rdy = false;
				this->ReDistance = false;
			}

		}
		else if (this->keyboard->KeyIsTrigger('C'))
		{
			if (this->handhold.targets[7].use == true)
			{
				nextPos.x = this->handhold.targets[7].px;
				nextPos.y = this->handhold.targets[7].py;
				this->Rdy = false;
				this->ReDistance = false;
			}
		}
		if (this->Rdy == false)
		{
			this->handhold.ResetTarget();
		}
	}


	if (this->nextPos != this->recPos)
	{
		if (this->player == this->nextPos)
		{
			this->recPos = this->nextPos;
			this->Rdy = true;
		}
		else
		{
			float dist = sqrtf((nextPos.x - this->player.x) * (nextPos.x - this->player.x) +
				(nextPos.y - this->player.y) * (nextPos.y - this->player.y));
			if (dist < PLAYER_SPD)
			{
				this->player = this->nextPos;
			}
			else
			{
				float tmpSin = (nextPos.y - this->player.y) / dist;
				float tmpCos = (nextPos.x - this->player.x) / dist;
				this->gfx->camera.AdjustPosition(CAMERA_SPD*tmpCos, CAMERA_SPD*tmpSin, 0.0f);
				this->player.x += PLAYER_SPD * tmpCos;
				this->player.y += PLAYER_SPD * tmpSin;
			}
			PLAYER_POS.x = this->player.x;
			PLAYER_POS.y = this->player.y - 1.5f;
		}
	}


	const float cameraSpeed = 0.01f;
	D3DXVECTOR3 camPos = this->gfx->camera.GetPositionVector();

	if (camPos != D3DXVECTOR3(this->player.x, this->player.y - 0.5f, PLAYER_POS.z - 3.0f))
	{
		float distance = sqrtf((this->player.x - camPos.x) * (this->player.x - camPos.x) +
			(this->player.y - 0.5f - camPos.y) * (this->player.y - 0.5f - camPos.y));
		if (distance < CAMERA_SPD)
		{
			this->gfx->camera.SetPosition(this->player.x, this->player.y - 0.5f, PLAYER_POS.z - 3.0f);
		}
		else
		{
			float tmpSin = (this->player.y - 0.5f - camPos.y) / distance;
			float tmpCos = (this->player.x - camPos.x) / distance;
			this->gfx->camera.AdjustPosition(CAMERA_SPD*tmpCos, CAMERA_SPD*tmpSin, 0.0f);
		}
	}

	//if moved
	if (this->player.x != nextPos.x || player.y != nextPos.y)
	{
		if (!AlreadySend)
		{
			SERVER_MSG msg;
			msg.px = nextPos.x;
			msg.py = nextPos.y;
			this->network->Send(&msg);
			AlreadySend = true;
		}
	}

	return GAME1_NUM;
}

bool Game1::Draw()
{
	this->gfx->RenderFrame();

	this->handhold.Draw();

	for (int i = 0; i < TOTAL_TARGET; i++)
	{
		if (this->handhold.targets[i].use)
		{
			this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
				D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_TARGET);
			switch (i)
			{
			case 0:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_CR);
				break;
			case 1:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_TR);
				break;
			case 2:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_TC);
				break;
			case 3:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_TL);
				break;
			case 4:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_CL);
				break;
			case 5:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_DL);
				break;
			case 6:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_DC);
				break;
			case 7:
				this->gfx->DrawBillboard(D3DXVECTOR3(this->handhold.targets[i].px, this->handhold.targets[i].py, -1.0f),
					D3DXVECTOR2(1.0f, 1.0f), TEXTURE_INDEX_DR);
				break;
			default:
				break;
			}
		}
	}

	RECT textbox;
	SetRect(&textbox, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (CountDown > 0)
	{
		static char timeText[5];
		_itoa_s(CountDown, timeText, 10);
		pDXfont->DrawTextA(NULL,
			(LPCSTR)&timeText,
			strlen((LPCSTR)&timeText),
			&textbox,
			DT_CENTER,
			D3DCOLOR_ARGB(255, 120, 120, 255));
	}
	else
	{
		pDXfont->DrawTextA(NULL,
			"GAME OVER",
			10,
			&textbox,
			DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 120, 120, 255));
	}

	
	static char scoreText[5];
	_itoa_s(this->gfx->hight, scoreText, 10);
	pDXfont->DrawTextA(NULL,
		(LPCSTR)&scoreText,
		strlen((LPCSTR)&scoreText),
		&textbox,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 120, 120, 255));


	this->gfx->RenderFrame_end();

	return true;
}

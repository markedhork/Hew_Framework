#include "Game1.h"

// position.xyz , rotation.xyz , size.xyz , TextureIndex number
Sprite Game1_sprite[] = {
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BG},
	{D3DXVECTOR3(0,-1000,0.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TARGET},
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

	D3DXCreateFont(this->gfx->GetDevice(), 100, 0, FW_ULTRABOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Comic Sans MS", &pDXfont);

	return true;
}

int Game1::Update()
{
	float dt = this->timer->GetMilisecondsElapsed();
	if (dt > 1000.0f)
	{
		this->timer->Restart();
		CountDown--;
	}


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

	if (this->handhold.holders[tempcount].use == true)
	{
		TARGET1_POS.x = this->handhold.holders[tempcount].px;
	
		TARGET1_POS.y = this->handhold.holders[tempcount].py;

	}

	if (this->keyboard->KeyIsTrigger('Q'))
	{
		if (this->handhold.holders[tempcount].use == true)
		{
			PLAYER_POS.x = this->handhold.holders[tempcount].px;
			PLAYER_POS.y = this->handhold.holders[tempcount].py;
			tempcount++;
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

	return GAME1_NUM;
}

bool Game1::Draw()
{
	this->gfx->RenderFrame();

	this->handhold.Draw();

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


	this->gfx->RenderFrame_end();

	return true;
}

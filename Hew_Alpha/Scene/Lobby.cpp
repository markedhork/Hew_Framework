#include "Lobby.h"

Sprite Lobby_sprite[] = {
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BG},
};
Mesh Lobby_mesh[] = {
	{},
};
// 読み込みテクスチャ数
static const int SPRITE_COUNT_TT = sizeof(Lobby_sprite) / sizeof(Lobby_sprite[0]);
static const int MESH_COUNT_TT = 0;

bool Lobby::Set()
{
	this->gfx->Set(Lobby_sprite, SPRITE_COUNT_TT, Lobby_mesh, MESH_COUNT_TT);

	D3DXCreateFont(this->gfx->GetDevice(), 100, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &pDXfont);

	return true;
}

int Lobby::Update()
{
	float dt = this->timer->GetMilisecondsElapsed();
	if (dt > 1000.0f)
	{
		this->timer->Restart();
		if (RdyToGo)
		{
			CountDown--;
		}
	}

	this->RdyToGo = this->network->IfConected();
	
	if (this->network->GetType() == SERVER_TYPE)
	{
		if (CountDown < 0)
		{
			return GAME1_NUM;
		}
		else
		{
			return LOBBY1_NUM;
		}
	}
	else
	{
		if (CountDown < 0)
		{
			return GAME2_NUM;
		}
		else
		{
			return LOBBY2_NUM;
		}
	}

}

bool Lobby::Draw()
{
	this->gfx->RenderFrame();

	RECT textbox;
	SetRect(&textbox, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (this->RdyToGo)
	{
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
				"GO!",
				4,
				&textbox,
				DT_CENTER,
				D3DCOLOR_ARGB(255, 120, 120, 255));
		}
		
	}
	else
	{
		pDXfont->DrawTextA(NULL,
			"WAITING...",
			11,
			&textbox,
			DT_CENTER,
			D3DCOLOR_ARGB(177, 255, 255, 255));
	}
	

	this->gfx->RenderFrame_end();

	return true;
}


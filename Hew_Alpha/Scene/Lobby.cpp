#include "Lobby.h"

Sprite Lobby_sprite[] = {
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BG},
};

// 読み込みテクスチャ数
static const int SPRITE_COUNT_TT = sizeof(Lobby_sprite) / sizeof(Lobby_sprite[0]);

bool Lobby::Set()
{
	this->gfx->Set(Lobby_sprite, SPRITE_COUNT_TT);
	return true;
}

int Lobby::Update()
{
	if (this->network->GetType()==SERVER_TYPE)
	{
		if (this->network->IfConected())
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
		if (this->network->IfConected())
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
	this->gfx->RenderFrame_end();

	return true;
}


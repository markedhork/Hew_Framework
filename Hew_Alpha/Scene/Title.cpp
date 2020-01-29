#include "Title.h"

Sprite Title_sprite[] = {
	{D3DXVECTOR3(0,0,0.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_1},
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1.0f,1.0f),TEXTURE_INDEX_TITLE_BG},
	{D3DXVECTOR3(565.5f,565.5f,0.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BT},
};
Mesh Title_mesh[] = {
	{D3DXVECTOR3(0,-0.5f,-1),D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1),MESH_INDEX_PLAYER},
};

// 読み込みテクスチャ数
static const int SPRITE_COUNT_TT = sizeof(Title_sprite) / sizeof(Title_sprite[0]);
static const int MESH_COUNT_TT = sizeof(Title_mesh) / sizeof(Title_mesh[0]);

bool Title::Set()
{
	this->gfx->Set(Title_sprite, SPRITE_COUNT_TT, Title_mesh, MESH_COUNT_TT);
	this->gfx->camera.SetPosition(0, 0, -5.0f);
	this->gfx->camera.SetRotation(0, -45, 0);

	D3DXCreateFont(this->gfx->GetDevice(), 100, 30, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &pDXfont);

	return true;
}

int Title::Update()
{
	
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

	if (this->keyboard->KeyIsTrigger(VK_UP))
	{
		ck--;
	}
	if (this->keyboard->KeyIsTrigger(VK_DOWN))
	{
		ck++;
	}
	if (ck < 0)
	{
		ck = 1;
	}
	if (ck > 1)
	{
		ck = 0;
	}
	if (ck == 0)
	{
		Title_sprite[2].pos.y = 565.5f;
	}
	else if (ck == 1)
	{
		Title_sprite[2].pos.y = 665.5f;

	}
	if (this->keyboard->KeyIsPressed(VK_RETURN))
	{
		switch (ck)
		{
		case 0:
			return LOBBY1_NUM;
		case 1:
			return LOBBY2_NUM;
		default:
			break;
		}

	}

	return TITLE_NUM;
}

bool Title::Draw()
{
	this->gfx->RenderFrame();

	// display the font

	RECT textbox;
	SetRect(&textbox, 680, 550, 1000, 650);
	pDXfont->DrawTextA(NULL,
		"PLAYER 1",
		9,
		&textbox,
		DT_SINGLELINE|DT_LEFT,
		D3DCOLOR_ARGB(177, 255, 255, 255));

	SetRect(&textbox, 680, 650, 1000, 750);
	pDXfont->DrawTextA(NULL,
		"PLAYER 2",
		9,
		&textbox,
		DT_SINGLELINE | DT_LEFT,
		D3DCOLOR_ARGB(177, 255, 255, 255));

	this->gfx->RenderFrame_end();
	return true;
}


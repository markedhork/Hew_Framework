#include "Game2.h"
Sprite Game2_sprite[] = {
	{D3DXVECTOR3(0,0,1.0f),D3DXVECTOR3(0,0,0),D3DXVECTOR2(1,1),TEXTURE_INDEX_TITLE_BG},

};

Mesh Game2_mesh[] = {
	{D3DXVECTOR3(0,-0.5f,-1),D3DXVECTOR3(0,180,0),D3DXVECTOR3(1,1,1),MESH_INDEX_PLAYER},
};

// 読み込みテクスチャ数
static const int SPRITE_COUNT_G2 = sizeof(Game2_sprite) / sizeof(Game2_sprite[0]);
static const int MESH_COUNT_G2 = sizeof(Game2_mesh) / sizeof(Game2_mesh[0]);
bool Game2::Set()
{
	this->gfx->Set(Game2_sprite, SPRITE_COUNT_G2, Game2_mesh, MESH_COUNT_G2);
	this->gfx->camera.SetPosition(0, 0, -15.0f);
	this->gfx->camera.SetRotation(0, 0, 0);

	this->ball.SetDevice(this->gfx->GetDevice());
	this->ball.CreateMeshBuffer();
	this->handhold.SetDevice(this->gfx->GetDevice());
	this->handhold.CreateMeshBuffer();

	player.x = PLAYER_POS.x;
	player.y = PLAYER_POS.y + 1.5f;
	player.z = PLAYER_POS.z;

	nextPos.x = player.x;
	nextPos.y = player.y;
	nextPos.z = player.z;

	D3DXCreateFont(this->gfx->GetDevice(), 100, 0, FW_ULTRABOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Comic Sans MS", &pDXfont);


	ctimer.Start();
	return true;
}

int Game2::Update()
{
	float ct = this->timer->GetMilisecondsElapsed();
	float dt = ctimer.GetMilisecondsElapsed() / 1000.0f;
	ctimer.Restart();
	if (ct > 1000.0f)
	{
		this->timer->Restart();
		CountDown--;
	}

	this->ball.Update();

	if (this->pointTotal != ball.pointTotal)
	{
		if (ball.points[ball.pointTotal - 1].pos.x < MAX_FLOAT_X
			&&ball.points[ball.pointTotal - 1].pos.x > MIN_FLOAT_X
			&&ball.points[ball.pointTotal - 1].pos.y > MIN_FLOAT_Y
			&&ball.points[ball.pointTotal - 1].pos.y < MAX_FLOAT_Y)
		{
			int floorX;
			int floorY;

			float tmpx, tmpy;
			tmpx = ball.points[ball.pointTotal - 1].pos.x - MIN_FLOAT_X;
			tmpy = ball.points[ball.pointTotal - 1].pos.y - MIN_FLOAT_Y;

			floorX = tmpx / 1;
			floorY = tmpy / 1;

			if (Build[floorY / MAP_Y] > 0)
			{
				if (map[Build[floorY / MAP_Y] - 1][floorY % MAP_Y][floorX] == 1)
				{
					//true
					this->points[this->pointTotal].use = true;
					this->points[this->pointTotal].pos = ball.points[ball.pointTotal - 1].pos;

					this->handhold.holders[this->pointTotal].use = true;
					this->handhold.holders[this->pointTotal].px = this->points[this->pointTotal].pos.x;
					this->handhold.holders[this->pointTotal].py = this->points[this->pointTotal].pos.y;
					//Send message
					CLIENT_MSG msg;
					msg.number = this->pointTotal;
					msg.x = this->handhold.holders[this->pointTotal].px;
					msg.y = this->handhold.holders[this->pointTotal].py;
					this->network->Send(&msg);
				}
				else
				{
					//false 
					ball.points[ball.pointTotal - 1].use = false;
					ball.pointTotal--;
				}
			}
			else
			{
				//false 
				ball.points[ball.pointTotal - 1].use = false;
				ball.pointTotal--;
			}
		}
		else
		{
			ball.points[ball.pointTotal - 1].use = false;
			ball.pointTotal--;
		}

		this->pointTotal = ball.pointTotal;
		this->handhold.activedTotal = this->pointTotal;
	}

	if (this->network->IfMsgFromServer())
	{
		SERVER_MSG temp = this->network->GetMsgFromServer();

		nextPos.x = temp.px;
		nextPos.y = temp.py;
		nextPos.z = PLAYER_POS.z;
	}

	if (this->nextPos != player)
	{
		float dist = sqrtf((nextPos.x - nextPos.x) * (nextPos.x - player.x) +
			(nextPos.y - player.y) * (nextPos.y - player.y));
		if (dist < PLAYER_SPD)
		{
			player = this->nextPos;
		}
		else
		{
			float tmpSin = (nextPos.y - player.y) / dist;
			float tmpCos = (nextPos.x - player.x) / dist;
			player.x += PLAYER_SPD * tmpCos;
			player.y += PLAYER_SPD * tmpSin;
		}
		PLAYER_POS = player;
		PLAYER_POS.y -= 1.5f;
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

	const float cameraSpeed = 2.0f;

	if (this->keyboard->KeyIsPressed(VK_LEFT))
	{
		this->gfx->camera.AdjustPosition(this->gfx->camera.GetLeftVector()*cameraSpeed*dt);
	}
	if (this->keyboard->KeyIsPressed(VK_RIGHT))
	{
		this->gfx->camera.AdjustPosition(this->gfx->camera.GetRightVector()*cameraSpeed*dt);
	}
	if (this->keyboard->KeyIsPressed(VK_UP))
	{
		this->gfx->camera.AdjustPosition(0.0f, cameraSpeed*dt, 0.0f);
	}
	if (this->keyboard->KeyIsPressed(VK_DOWN))
	{
		this->gfx->camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
	}
	if (this->keyboard->KeyIsPressed(VK_SPACE))
	{
		ball.Generate(this->gfx->camera.GetPositionVector(), NULL); // wanna change second param to degree
	}
	return GAME2_NUM;
}

bool Game2::Draw()
{
	this->gfx->RenderFrame();

	this->ball.Draw();
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

bool Game2::CheckClickInZone()
{
	float len = sqrtf((this->mouse->GetPosX() - CLICK_ZONE_POS_X)*(this->mouse->GetPosX() - CLICK_ZONE_POS_X) + (this->mouse->GetPosY() - CLICK_ZONE_POS_Y)*(this->mouse->GetPosY() - CLICK_ZONE_POS_Y));
	if (len <= CLICK_ZONE_POS_RADIUS)
	{
		//Click
		return true;
	}
	else
	{
		return false;
	}
}


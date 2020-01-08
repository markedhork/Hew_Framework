#include "Ball.h"
#include "Timer.h"
#include <string>
static Timer timer;
static Timer ctimer;
Ball::Ball()
{
	timer.Start();
	ctimer.Start();
	for (int i = 0; i < MAX_BALL; i++)
	{
		balls[i].degree = 0.0f;
		balls[i].use = false;
		balls[i].x = 0.0f;
		balls[i].y = 0.0f;
		balls[i].z = 0.0f;
	}
}

Ball::~Ball()
{
	this->Uninitialize();
}

void Ball::SetDevice(LPDIRECT3DDEVICE9 pD3Device)
{
	this->pDevice = pD3Device;
}

void Ball::Update()
{
	float dt = timer.GetMilisecondsElapsed() / 1000;
	timer.Restart();
	if (ctimer.GetMilisecondsElapsed() > 1000.0f)
	{
		std::string outmsg = "";
		outmsg += std::to_string(dt);
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
		ctimer.Restart();
	}
	for (int i = 0; i < MAX_BALL; i++)
	{
		if (balls[i].use == true)
		{
			balls[i].time += dt;
			balls[i].z += balls[i].vz*dt;
			balls[i].y += balls[i].vy*dt - (GRAVITY / 2) * balls[i].time;
		}
		if (balls[i].z >= 0.0f)
		{
			balls[i].use = false;
			balls[i].degree = 0.0f;
			balls[i].vz = 0.0f;
			balls[i].vy = 0.0f;
			balls[i].x = 0.0f;
			balls[i].y = 0.0f;
			balls[i].z = 0.0f;
			balls[i].time = 0.0f;
		}
		if (balls[i].y < -300)
		{
			balls[i].use = false;
			balls[i].degree = 0.0f;
			balls[i].vz = 0.0f;
			balls[i].vy = 0.0f;
			balls[i].x = 0.0f;
			balls[i].y = 0.0f;
			balls[i].z = 0.0f;
			balls[i].time = 0.0f;
		}
	}
}

void Ball::Draw()
{
	for (int i = 0; i < MAX_BALL; i++)
	{
		if (balls[i].use == true)
		{
			D3DXMATRIX mtxWorld;
			D3DXMatrixIdentity(&mtxWorld);	//ワールド行列の単位行列の初期化
			D3DXMATRIX rotMTX;
			D3DXMATRIX mtxTrs;
			D3DXMATRIX mtxScl;
			//平行移動行列を作成＆ワールド行列への合成
			D3DXMatrixScaling(&mtxScl, BALL_SIZE, BALL_SIZE, BALL_SIZE);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);	//World*Rotation


			D3DXMatrixRotationYawPitchRoll(&rotMTX, balls[i].degree, 0.0f, 0.0f);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &rotMTX);	//World*Rotation

			D3DXMatrixTranslation(&mtxTrs, balls[i].x, balls[i].y, balls[i].z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs);	//World*Tranlation

			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			for (int i = 0; i < numMaterials; i++)    // loop through each subset
			{
				pDevice->SetTexture(0, texture);
				pDevice->SetMaterial(&material[i]);    // set the appropriate material for the subset
				pDXMeshModel->DrawSubset(i);    // draw the subset
			}

		}
	}
}

void Ball::Generate(D3DXVECTOR3 pos, float pst)
{
	for (int i = 0; i < MAX_BALL; i++)
	{
		if (balls[i].use == false)
		{
			balls[i].degree = 0.0f;
			balls[i].x = pos.x;
			balls[i].y = pos.y -1.0f;
			balls[i].z = pos.z + 5.0f;
			balls[i].vz = pst * WALL_DISTANCE / FLYING_TIME;
			balls[i].vy = balls[i].vz * tanf(D3DXToRadian(SHOOTING_DEGREE));
			balls[i].use = true;

			break;
		}
	}
}

void Ball::CreateMeshBuffer()
{
	LPD3DXBUFFER	bufShipMaterials;		//マテリアルなどのインタフェース


	if (D3D_OK != D3DXLoadMeshFromX("asset/mesh/Soccer.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&bufShipMaterials,
		NULL,
		&numMaterials,
		&pDXMeshModel))
	{
		OutputDebugStringA("FAILED TO LOAD MESH FROM X FILE\n");
	}

	D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufShipMaterials->GetBufferPointer();
	material = new D3DMATERIAL9[numMaterials];
	for (DWORD i = 0; i < numMaterials; i++)    // for each material...
	{
		material[i] = tempMaterials[i].MatD3D;    // get the material info...
		material[i].Ambient = material[i].Diffuse;    // and make ambient the same as diffuse
	}
}

void Ball::Uninitialize()
{
	if (pDXMeshModel != NULL)
	{
		pDXMeshModel->Release();
		pDXMeshModel = NULL;
	}
}

#include "Ball.h"
#include "Timer.h"
#include <string>
static Timer timer;
Ball::Ball()
{
	timer.Start();
	ball.degree = 0.0f;
	ball.use = false;
	ball.x = 0.0f;
	ball.y = 0.0f;
	ball.z = 0.0f;

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

	if (ball.use == true)
	{
		ball.time += dt;
		ball.z += ball.vz*dt;
		ball.y += ball.vy*dt - (GRAVITY / 2) * ball.time;

		if (ball.z >= 0.0f)
		{
			int count;
			for (count = 0; count < MAX_POINT; count++)
			{
				if (this->points[count].use == false)
				{
					this->points[count].use = true;
					this->points[count].pos.x = ball.x;
					this->points[count].pos.y = ball.y;
					break;
				}
			}
			this->pointTotal = count + 1;

			ball.use = false;
			ball.degree = 0.0f;
			ball.x = 0.0f;
			ball.y = 0.0f;
			ball.z = 0.0f;
			ball.time = 0.0f;
		}
		else if (ball.y < -20)
		{
			ball.use = false;
			ball.degree = 0.0f;
			ball.x = 0.0f;
			ball.y = 0.0f;
			ball.z = 0.0f;
			ball.time = 0.0f;
		}
	}

}

void Ball::Draw()
{

	if (ball.use == true)
	{
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);	//ワールド行列の単位行列の初期化
		D3DXMATRIX rotMTX;
		D3DXMATRIX mtxTrs;
		D3DXMATRIX mtxScl;
		//平行移動行列を作成＆ワールド行列への合成
		D3DXMatrixScaling(&mtxScl, BALL_SIZE, BALL_SIZE, BALL_SIZE);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);	//World*Rotation


		D3DXMatrixRotationYawPitchRoll(&rotMTX, ball.degree, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &rotMTX);	//World*Rotation

		D3DXMatrixTranslation(&mtxTrs, ball.x, ball.y, ball.z);
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

void Ball::Generate(D3DXVECTOR3 pos, float pst)
{
	if (ball.use == false)
	{
		ball.degree = 0.0f;
		ball.use = true;
		ball.x = pos.x;
		ball.y = pos.y - 1.0f;
		ball.z = pos.z + 5.0f;

		ball.vz = 2.0f * WALL_DISTANCE / FLYING_TIME;
		ball.vy = ball.vz * tanf(D3DXToRadian(SHOOTING_DEGREE));
	}
}

void Ball::CreateMeshBuffer()
{
	LPD3DXBUFFER	bufShipMaterials;		//マテリアルなどのインタフェース


	if (D3D_OK != D3DXLoadMeshFromX(
		MESH_FILE_PATH,
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
		//std::string addr = "asset/texture/";
		//addr += tempMaterials[i].pTextureFilename;
		// if there is a texture to load, load it
		//if (FAILED(D3DXCreateTextureFromFileA(this->pDevice,
		//	addr.c_str(),
		//	&texture)))
			texture = NULL;    // if there is no texture, set the texture to NULL

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



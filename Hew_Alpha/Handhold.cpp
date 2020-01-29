#include "Handhold.h"
#include <string>

Handhold::~Handhold()
{
	this->Uninitialize();
}

void Handhold::CreateMeshBuffer()
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
		OutputDebugStringA("FAILED TO LOAD MESH FROM X FILE(HandHold)\n");
	}

	D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufShipMaterials->GetBufferPointer();
	material = new D3DMATERIAL9[numMaterials];
	for (DWORD i = 0; i < numMaterials; i++)    // for each material...
	{
		material[i] = tempMaterials[i].MatD3D;    // get the material info...
		material[i].Ambient = material[i].Diffuse;    // and make ambient the same as diffuse

		std::string addr = "asset/texture/";
		addr += tempMaterials[i].pTextureFilename;
		// if there is a texture to load, load it
		if (FAILED(D3DXCreateTextureFromFileA(
			pDevice,
			addr.c_str(),
			&texture)))
			texture = NULL;
	}
}

void Handhold::SetDevice(LPDIRECT3DDEVICE9 pD3Device)
{
	this->pDevice = pD3Device;
}

void Handhold::Uninitialize()
{
	if (pDXMeshModel != NULL)
	{
		pDXMeshModel->Release();
		pDXMeshModel = NULL;
	}
}

void Handhold::UpdateTarget()
{
	for (int i = 0; i < activedTotal; i++)
	{
		if (this->holders[i].distance > MAX_DISTANCE || this->holders[i].distance < MIN_DISTANCE)
		{
			continue;
		}
		if (this->holders[i].degree >= -22.5f&&this->holders[i].degree < 0.0f
			|| this->holders[i].degree >= 0.0f&&this->holders[i].degree < 22.5f)
		{
			if (this->targets[0].distance > this->holders[i].distance)
			{
				this->targets[0] = this->holders[i];
			}
		}
		else if (this->holders[i].degree >= 22.5f&&this->holders[i].degree < 67.5f)
		{
			if (this->targets[1].distance > this->holders[i].distance)
			{
				this->targets[1] = this->holders[i];
			}
		}
		else if (this->holders[i].degree >= 67.5f&&this->holders[i].degree < 112.5f)
		{
			if (this->targets[2].distance > this->holders[i].distance)
			{
				this->targets[2] = this->holders[i];
			}
		}
		else if (this->holders[i].degree >= 112.5f&&this->holders[i].degree < 157.5f)
		{
			if (this->targets[3].distance > this->holders[i].distance)
			{
				this->targets[3] = this->holders[i];
			}
		}
		else if (this->holders[i].degree >= 157.5f&&this->holders[i].degree < 180.0f
			|| this->holders[i].degree >= -180.0f&&this->holders[i].degree < -157.5f)
		{
			if (this->targets[4].distance > this->holders[i].distance)
			{
				this->targets[4] = this->holders[i];
			}
		}
		else if (this->holders[i].degree >= -157.5f&&this->holders[i].degree < -112.5f)
		{
			if (this->targets[5].distance > this->holders[i].distance)
			{
				this->targets[5] = this->holders[i];
			}
		}
		else if (this->holders[i].degree >= -112.5f&&this->holders[i].degree < -67.5f)
		{
			if (this->targets[6].distance > this->holders[i].distance)
			{
				this->targets[6] = this->holders[i];
			}
		}
		else if (this->holders[i].degree >= -67.5f&&this->holders[i].degree < -22.5f)
		{
			if (this->targets[7].distance > this->holders[i].distance)
			{
				this->targets[7] = this->holders[i];
			}
		}
	}

}

void Handhold::CheckArea(float playerX, float playerY)
{
	for (int i = 0; i < activedTotal; i++)
	{
		this->holders[i].distance = sqrtf((this->holders[i].py - playerY)*(this->holders[i].py - playerY)
			+ (this->holders[i].px - playerX)*(this->holders[i].px - playerX));
		this->holders[i].degree = atan2f(this->holders[i].py - playerY, this->holders[i].px - playerX) * 180 / D3DX_PI;
	}
	this->UpdateTarget();
}

void Handhold::CheckSolo(int index)
{
	if (this->holders[index].distance > MAX_DISTANCE || this->holders[index].distance < MIN_DISTANCE)
	{
		return;
	}
	if (this->holders[index].degree >= -22.5f&&this->holders[index].degree < 0.0f
		|| this->holders[index].degree >= 0.0f&&this->holders[index].degree < 22.5f)
	{
		if (this->targets[0].distance > this->holders[index].distance)
		{
			this->targets[0] = this->holders[index];
		}
	}
	else if (this->holders[index].degree >= 22.5f&&this->holders[index].degree < 67.5f)
	{
		if (this->targets[1].distance > this->holders[index].distance)
		{
			this->targets[1] = this->holders[index];
		}
	}
	else if (this->holders[index].degree >= 67.5f&&this->holders[index].degree < 112.5f)
	{
		if (this->targets[2].distance > this->holders[index].distance)
		{
			this->targets[2] = this->holders[index];
		}
	}
	else if (this->holders[index].degree >= 112.5f&&this->holders[index].degree < 157.5f)
	{
		if (this->targets[3].distance > this->holders[index].distance)
		{
			this->targets[3] = this->holders[index];
		}
	}
	else if (this->holders[index].degree >= 157.5f&&this->holders[index].degree < 180.0f
		|| this->holders[index].degree >= -180.0f&&this->holders[index].degree < -157.5f)
	{
		if (this->targets[4].distance > this->holders[index].distance)
		{
			this->targets[4] = this->holders[index];
		}
	}
	else if (this->holders[index].degree >= -157.5f&&this->holders[index].degree < -112.5f)
	{
		if (this->targets[5].distance > this->holders[index].distance)
		{
			this->targets[5] = this->holders[index];
		}
	}
	else if (this->holders[index].degree >= -112.5f&&this->holders[index].degree < -67.5f)
	{
		if (this->targets[6].distance > this->holders[index].distance)
		{
			this->targets[6] = this->holders[index];
		}
	}
	else if (this->holders[index].degree >= -67.5f&&this->holders[index].degree < -22.5f)
	{
		if (this->targets[7].distance > this->holders[index].distance)
		{
			this->targets[7] = this->holders[index];
		}
	}
}

void Handhold::ResetTarget()
{
	for (int i = 0; i < TOTAL_TARGET; i++)
	{
		this->targets[i].use = false;
		this->targets[i].distance = 9999.9f;
		this->targets[i].px = 0.0f;
		this->targets[i].py = 0.0f;
	}
}

void Handhold::Draw()
{
	for (int i = 0; i < this->activedTotal; i++)
	{
		if (this->holders[i].use == true)
		{
			D3DXMATRIX mtxWorld;
			D3DXMatrixIdentity(&mtxWorld);	//ワールド行列の単位行列の初期化

			D3DXMATRIX mtxTrs;
			D3DXMatrixTranslation(&mtxTrs, holders[i].px, holders[i].py, -1);
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

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

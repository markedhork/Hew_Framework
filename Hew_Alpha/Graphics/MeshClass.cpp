#include "MeshClass.h"
#include <string>
void MeshClass::CreateMeshBuffer(HWND hwnd, LPDIRECT3DDEVICE9 pDevice)
{
	this->pD3DDevice = pDevice;
	

	this->mesh_members = new MeshMember[MESH_FILE_COUNT];

	for (int i = 0; i < MESH_FILE_COUNT; i++) {

		LPD3DXBUFFER	bufShipMaterials;		//マテリアルなどのインタフェース

		if (D3D_OK != D3DXLoadMeshFromX(
			g_MeshFiles[i].filename,
			D3DXMESH_SYSTEMMEM,
			pD3DDevice,
			NULL,
			&bufShipMaterials,
			NULL,
			&mesh_members[i].numMaterials,
			&mesh_members[i].pDXMeshModel))
		{
			MessageBox(hwnd, "FAILED TO LOAD MESH FROM X FILE", "エラー", MB_OK);
			OutputDebugStringA("FAILED TO LOAD MESH FROM X FILE\n");
		}

		D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufShipMaterials->GetBufferPointer();
		mesh_members[i].material = new D3DMATERIAL9[mesh_members[i].numMaterials];
		mesh_members[i].texture = new LPDIRECT3DTEXTURE9[mesh_members[i].numMaterials];


		for (DWORD j = 0; j < mesh_members[i].numMaterials; j++)    // for each material...
		{
			mesh_members[i].material[j] = tempMaterials[j].MatD3D;    // get the material info...
			mesh_members[i].material[j].Ambient = mesh_members[i].material[j].Diffuse;    // and make ambient the same as diffuse
			std::string addr = "asset/texture/";
			addr += tempMaterials[j].pTextureFilename;
			// if there is a texture to load, load it
			if (FAILED(D3DXCreateTextureFromFileA(pD3DDevice,
				addr.c_str(),
				&mesh_members[i].texture[j])))
				mesh_members[i].texture[j] = NULL;    // if there is no texture, set the texture to NULL
		}

	}
}


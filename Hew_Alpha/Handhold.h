#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#define MAX_HOLDER (500)
#define MESH_FILE_PATH			"asset/mesh/curveWall_small.x"

#define TOTAL_TARGET	(8)

struct Holders
{
	bool use = false;
	float px, py;
	int type;
};

class Handhold
{
public:
	~Handhold();
	Holders holders[MAX_HOLDER];
	int activedTotal = 0;
	void CreateMeshBuffer();

	void SetDevice(LPDIRECT3DDEVICE9 pD3Device);
	void Uninitialize();

	void Update();
	Holders targets[TOTAL_TARGET];

	void Draw();

private:
	LPD3DXMESH		pDXMeshModel;		//���f����\���C���^�t�F�[�X
	LPDIRECT3DTEXTURE9 texture = NULL;

	DWORD			numMaterials;		//�}�e���A���̐�
	D3DMATERIAL9	*material;    // create the material struct
	LPDIRECT3DDEVICE9 pDevice;

};


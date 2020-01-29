#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#define MAX_HOLDER (500)
#define MESH_FILE_PATH			"asset/mesh/curveWall_small.x"

#define TOTAL_TARGET	(8)
#define MAX_DISTANCE	(1.3f)
#define MIN_DISTANCE	(0.5f)

#define PLAYER_SPD		(0.2f)

struct Holders
{
	bool use = false;
	float px, py;
	int type;
	float distance = 9999.9f;
	float degree;
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

	void UpdateTarget();
	void CheckArea(float playerX, float playerY);
	void CheckSolo(int index);

	void ResetTarget();

	//3:TL 2:TC 1:TR 4:CL 0:CR 5:DL 6:DC 7:DR
	Holders targets[TOTAL_TARGET];
	Holders temp[50];
	void Draw();

private:
	LPD3DXMESH		pDXMeshModel;		//モデルを表すインタフェース
	LPDIRECT3DTEXTURE9 texture = NULL;

	DWORD			numMaterials;		//マテリアルの数
	D3DMATERIAL9	*material;    // create the material struct
	LPDIRECT3DDEVICE9 pDevice;

};


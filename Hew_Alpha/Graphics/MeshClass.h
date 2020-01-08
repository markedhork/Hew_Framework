#pragma once
#include <d3dx9.h>

#define MESH_FILENAME_MAX (64)

// meshファイル管理構造体
typedef struct MeshFile_tag
{
	char filename[MESH_FILENAME_MAX]; // meshファイル名

} MeshFile;

static const MeshFile g_MeshFiles[] = {
	{ "asset/mesh/girl.x"},
};

// 読み込みmesh数
static const int MESH_FILE_COUNT = sizeof(g_MeshFiles) / sizeof(g_MeshFiles[0]);

typedef enum MeshIndex
{
	MESH_INDEX_PLAYER,

	MESH_INDEX_MAX
};

struct MeshMember
{
	LPD3DXMESH				pDXMeshModel;		//モデルを表すインタフェース
	DWORD					numMaterials;		//マテリアルの数
	D3DMATERIAL9			*material;			// create the material struct
	LPDIRECT3DTEXTURE9		*texture;
};

class MeshClass
{
public:
	void CreateMeshBuffer(HWND hwnd, LPDIRECT3DDEVICE9 pDevice);
	MeshMember				*mesh_members;
private:

	LPDIRECT3DDEVICE9		pD3DDevice;
};


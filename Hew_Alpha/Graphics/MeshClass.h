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

struct CUSTOM_FRAME : public D3DXFRAME
{
	D3DXMATRIX CombTransformationMatrix;    // transform data multiplied by all parents' data
};

struct CUSTOM_MESHCONTAINER : public D3DXMESHCONTAINER
{
	D3DXMATRIX** ppFrameMatrices;    // an array of matrices to store current animation data
	LPD3DXMESH pFinalMesh;    // a duplicate of the mesh, representing it's current pose
	LPDIRECT3DTEXTURE9* pTextures;    // an array of applicable textures
};

class MeshClass
{
public:
	void CreateMeshBuffer(HWND hwnd, LPDIRECT3DDEVICE9 pDevice);
	MeshMember				*mesh_members;
private:

	LPDIRECT3DDEVICE9		pD3DDevice;
};


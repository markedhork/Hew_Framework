#pragma once
#include <d3dx9.h>

#define MESH_FILENAME_MAX (64)

// mesh�t�@�C���Ǘ��\����
typedef struct MeshFile_tag
{
	char filename[MESH_FILENAME_MAX]; // mesh�t�@�C����

} MeshFile;

static const MeshFile g_MeshFiles[] = {
	{ "asset/mesh/girl.x"},
};

// �ǂݍ���mesh��
static const int MESH_FILE_COUNT = sizeof(g_MeshFiles) / sizeof(g_MeshFiles[0]);

typedef enum MeshIndex
{
	MESH_INDEX_PLAYER,

	MESH_INDEX_MAX
};

struct MeshMember
{
	LPD3DXMESH				pDXMeshModel;		//���f����\���C���^�t�F�[�X
	DWORD					numMaterials;		//�}�e���A���̐�
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


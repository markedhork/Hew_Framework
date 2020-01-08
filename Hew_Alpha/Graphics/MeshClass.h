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

class MeshClass
{
public:
	void CreateMeshBuffer(HWND hwnd, LPDIRECT3DDEVICE9 pDevice);
	MeshMember				*mesh_members;
private:

	LPDIRECT3DDEVICE9		pD3DDevice;
};


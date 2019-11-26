#pragma once
#include <d3dx9.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3dx9.lib")
#include "Texture.h"
#define FVF_VERTEX2D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ２Dポリゴン頂点フォーマット
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// ２Dポリゴン頂点構造体
struct Vertex2D
{
	D3DXVECTOR3 vtx; // 頂点座標（座標変換済み頂点）
	D3DCOLOR color;
	D3DXVECTOR2 tex;
};

struct Sprite
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 size;
	int index;
};

struct VERTEX_3D
{
	D3DXVECTOR3 vtx;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR diffuse;	//反射光
	D3DXVECTOR2 tex;	//テクスチャ座標
};
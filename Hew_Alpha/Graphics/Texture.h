#pragma once
#include <d3dx9.h>

#define TEXTURE_FILENAME_MAX (64)

// テクスチャファイル管理構造体
typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX]; // テクスチャファイル名
	int width;  // テクスチャ解像度横幅
	int height; // テクスチャ解像度縦幅
} TextureFile;

static const TextureFile g_TextureFiles[] = {
	{ "asset/texture/airou.png", 256,  256 },
	{ "asset/texture/KIZUNA.jpg", 225, 225 }
};

// 読み込みテクスチャ数
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);


typedef enum TextureIndex
{
	TEXTURE_INDEX_WHITE,
	TEXTURE_INDEX_KIZUNA,

	TEXTURE_INDEX_MAX
};

class Texture
{
public:
	void Load(HWND hwnd, LPDIRECT3DDEVICE9 pD3DDevice);
	void Release();

	LPDIRECT3DTEXTURE9 GetTexture(TextureIndex index);

	int GetWidth(TextureIndex index);
	int GetHeight(TextureIndex index);

private:
	LPDIRECT3DTEXTURE9 pTextures[TEXTURE_FILE_COUNT] = {};
	LPDIRECT3DDEVICE9 pD3DDevice;
};


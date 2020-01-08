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
	{ "asset/texture/wall.jpg", 64, 64 },

	{ "asset/texture/Title_1.png", 1600, 900 },
	{ "asset/texture/Title_2.jpg", 1600, 900 },
	{ "asset/texture/bt.png", 69, 69 },
};

// 読み込みテクスチャ数
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);


typedef enum TextureIndex
{
	TEXTURE_INDEX_WALL,

	TEXTURE_INDEX_TITLE_1,
	TEXTURE_INDEX_TITLE_BG,
	TEXTURE_INDEX_TITLE_BT,

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


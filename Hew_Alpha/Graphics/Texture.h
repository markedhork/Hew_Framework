#pragma once
#include <d3dx9.h>

#define TEXTURE_FILENAME_MAX (64)

// �e�N�X�`���t�@�C���Ǘ��\����
typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX]; // �e�N�X�`���t�@�C����
	int width;  // �e�N�X�`���𑜓x����
	int height; // �e�N�X�`���𑜓x�c��
} TextureFile;

static const TextureFile g_TextureFiles[] = {
	{ "asset/texture/airou.png", 256,  256 },
	{ "asset/texture/KIZUNA.jpg", 225, 225 }
};

// �ǂݍ��݃e�N�X�`����
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


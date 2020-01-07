#include "Texture.h"

void Texture::Load(HWND hwnd, LPDIRECT3DDEVICE9 pDevice)
{
	this->pD3DDevice = pDevice;

	for (int i = 0; i < TEXTURE_FILE_COUNT; i++) {

		if (FAILED(D3DXCreateTextureFromFileEx
		(this->pD3DDevice, g_TextureFiles[i].filename,
			g_TextureFiles[i].width,    // default width
			g_TextureFiles[i].height,    // default height
			D3DX_DEFAULT,    // no mip mapping
			NULL,    // regular usage
			D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
			D3DPOOL_MANAGED,    // typical memory handling
			D3DX_DEFAULT,    // no filtering
			D3DX_DEFAULT,    // no mip filtering
			D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
			NULL,    // no image info struct
			NULL,    // not using 256 colors
			&pTextures[i]))) {
			MessageBox(hwnd, "テクスチャの読み取りが失敗しました", "エラー", MB_OK);
			OutputDebugStringA("failed to read texture \n");
		}
	}
}

void Texture::Release()
{
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++) {

		if (pTextures[i]) {
			pTextures[i]->Release();
			pTextures[i] = NULL;
		}
	}
}

LPDIRECT3DTEXTURE9 Texture::GetTexture(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) {
		return NULL;
	}

	return pTextures[index];
}

int Texture::GetWidth(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) {
		return NULL;
	}

	return g_TextureFiles[index].width;
}

int Texture::GetHeight(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) {
		return NULL;
	}

	return g_TextureFiles[index].height;
}

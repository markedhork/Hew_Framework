#include "Texture.h"

void Texture::Load(HWND hwnd, LPDIRECT3DDEVICE9 pDevice)
{
	this->pD3DDevice = pDevice;

	for (int i = 0; i < TEXTURE_FILE_COUNT; i++) {

		if (FAILED(D3DXCreateTextureFromFile(this->pD3DDevice, g_TextureFiles[i].filename, &pTextures[i]))) {
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

#pragma once
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include "Vertex.h"
#include <d3d9.h>
#include "..\\Timer.h"
#include "Camera.h"
#include <string>
#include "Texture.h"
#include "Map.h"

class Graphics
{
public:
	Graphics() {};
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void Set(Sprite* sprite, int total);
	Camera camera;
private:
	bool InitializeDirectX(HWND hwnd);
	bool UninitializeDirectX(HWND hwnd);
	bool InitializeVB();
	void Draw();
	void DrawWall();

	LPDIRECT3DDEVICE9	pD3DDevice;
	LPDIRECT3D9			pD3D;

	Texture				textureController;

	int					windowWidth = 0;
	int					windowHeight = 0;
	Timer				fpsTimer;
	int *				pSpritesIndex;
	int					totalSprites;
	Sprite *			pSprite;

	D3DXMATRIX			mtxWorld;												//ワールドマトリクス

	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 pVB_Sprite;

};




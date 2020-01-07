#pragma once
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include "..\\RenderWindow.h"
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
	void RenderFrame_end();
	void Set(Sprite* sprite, int total);
	LPDIRECT3DDEVICE9 GetDevice();
	Camera camera;
private:
	bool InitializeDirectX(HWND hwnd);
	bool UninitializeDirectX(HWND hwnd);
	bool InitializeVB();
	void Draw();
	void DrawUI();
	void DrawWall();


	LPDIRECT3DDEVICE9	pD3DDevice;
	LPDIRECT3D9			pD3D;
	LPD3DXSPRITE		pD3Dspt;

	Texture				textureController;

	int					windowWidth = 0;
	int					windowHeight = 0;
	Timer				fpsTimer;

	int *				pSpritesIndex;
	int					totalSprites;
	Sprite *			pSprite;
	Mesh *				pMesh;

	D3DXMATRIX			mtxWorld;												//ワールドマトリクス

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 pVB_Sprite;

};




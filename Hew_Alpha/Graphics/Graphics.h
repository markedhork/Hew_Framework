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
#include "MeshClass.h"
#include "Map.h"

class Graphics
{
public:
	Graphics() {};
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void RenderFrame_end();
	void Set(Sprite* sprite, int totalSprite, Mesh * mesh, int meshTotal);
	LPDIRECT3DDEVICE9 GetDevice();
	Camera camera;
private:
	bool InitializeDirectX(HWND hwnd);
	bool UninitializeDirectX(HWND hwnd);
	void Init_light();
	bool InitializeVB();

	void Draw();
	void DrawUI();
	void DrawWall();



	LPDIRECT3DDEVICE9	pD3DDevice;
	LPDIRECT3D9			pD3D;
	LPD3DXSPRITE		pD3Dspt;

	Texture				textureController;
	MeshClass			meshController;

	int					windowWidth = 0;
	int					windowHeight = 0;
	Timer				fpsTimer;

	int					totalSprites;
	int					totalMesh;
	Sprite *			pSprite;
	Mesh *				pMesh;

	D3DXMATRIX			mtxWorld;												//ワールドマトリクス

	LPDIRECT3DVERTEXBUFFER9 pVB_Wall;
	LPDIRECT3DVERTEXBUFFER9 pVB_Sprite;

};




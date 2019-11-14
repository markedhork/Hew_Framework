#pragma once
#include "Vertex.h"
#include <d3d9.h>
#include "..\\Timer.h"
#include "Camera.h"
#include "Model.h"
#include <string>
#include <wrl/client.h>

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	Camera camera;
	Model model;
private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeScene();
	bool UninitializeDirectX(HWND hwnd);


	LPDIRECT3DDEVICE9 pD3DDevice;
	LPDIRECT3D9 pD3D;


	int windowWidth = 0;
	int windowHeight = 0;
	Timer fpsTimer;
};
#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->fpsTimer.Start();

	if (!InitializeDirectX(hwnd))
	{
		return false;
	}

	if (!InitializeScene())
	{
		return false;
	}

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f,0.0f,0.0f,1.0f };
	
	UINT offset = 0;

	{//Pavement Texture
		//this->model.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	}

	//Draw Text
	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0)
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	} 

	
	//Render Draw Data
	this->pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	// Direct3Dインターフェースの取得
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (pD3D == NULL) {
		// Direct3Dインターフェースの取得に失敗
		MessageBox(hwnd, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// デバイスの生成に利用する情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = this->windowWidth;                       // バックバッファの横幅				
	d3dpp.BackBufferHeight = this->windowHeight;                     // バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;                    // バックバッファのフォーマット指定
	d3dpp.BackBufferCount = 1;                                  // バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // スワップエフェクト（スワップ方法）の設定		
	d3dpp.Windowed = TRUE;                                      // ウィンドウモード or フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;                        // 深度バッファ・ステンシルバッファの使用				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // 深度バッファ・ステンシルバッファのフォーマット指定
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // フルスクリーン時のリフレッシュレートの指定
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;       // リフレッシュレートとPresent処理の関係
	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice);

	// Direct3Dデバイスの取得
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
		// デバイスの作成に失敗
		MessageBox(hwnd, "Direct3Dデバイスの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// サンプラーの設定
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// アルファブレンドの設定
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 頂点カラーとテクスチャのブレンド設定
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return true;
}


bool Graphics::InitializeScene()
{
	
	return true;
}

bool Graphics::UninitializeDirectX(HWND hwnd)
{
	// Direct3Dデバイスの解放
	if (pD3DDevice) {
		pD3DDevice->Release();
		pD3DDevice = NULL;
	}

	// Direct3Dインターフェースの解放
	if (pD3D) {
		pD3D->Release();
		pD3D = NULL;
	}
	return true;
}

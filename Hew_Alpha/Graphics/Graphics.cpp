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

	this->textureController.Load(hwnd, this->pD3DDevice);
	this->meshController.CreateMeshBuffer(hwnd, this->pD3DDevice);

	this->camera.SetProjectionValues((float)this->windowWidth / (float)this->windowHeight);

	return true;
}

void Graphics::Set(Sprite* sprite, int total, Mesh * mesh, int meshTotal)
{
	this->pSprite = sprite;
	this->totalSprites = total;

	this->totalMesh = meshTotal;
	this->pMesh = mesh;

	this->Init_light();
	this->InitializeVB();
}

LPDIRECT3DDEVICE9 Graphics::GetDevice()
{
	return pD3DDevice;
}


void Graphics::RenderFrame()
{
	// 画面のクリア
	this->pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(200, 200, 200, 255), 1.0f, 0);

	// 描画バッチ命令の開始
	this->pD3DDevice->BeginScene();

	this->pD3DDevice->SetTransform(D3DTS_VIEW, &this->camera.GetViewMatrix());
	this->pD3DDevice->SetTransform(D3DTS_PROJECTION, &this->camera.GetProjectionMatrix());

	//============================================================================================
	//Draw 3D mesh model

	this->DrawWall();
	this->Draw();
	//============================================================================================


	this->pD3Dspt->Begin(D3DXSPRITE_ALPHABLEND);    // begin sprite drawing

	this->DrawUI();


	this->pD3Dspt->End();    // end sprite drawing


}

void Graphics::RenderFrame_end()
{
	// 描画バッチ命令の終了
	this->pD3DDevice->EndScene();
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
		OutputDebugStringA("failed to create Direct3D interface \n");
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
		OutputDebugStringA("failed to create Direct3D device \n");
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

	// Direct3D Spriteの取得
	D3DXCreateSprite(pD3DDevice, &pD3Dspt);

	return true;
}


bool Graphics::UninitializeDirectX(HWND hwnd)
{
	//Texture release
	this->textureController.Release();

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

void Graphics::Init_light()
{

	D3DLIGHT9 light[10];
	ZeroMemory(&light, sizeof(light));
	light[0].Type = D3DLIGHT_POINT;
	light[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	light[0].Range = 100.0f;
	light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light[0].Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light[0].Attenuation0 = 0.0f;
	light[0].Attenuation1 = 1.0f;
	light[0].Attenuation2 = 0.0f;

	light[1].Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	light[1].Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);

	light[2].Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	light[2].Direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	light[3].Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	light[3].Direction = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);

	pD3DDevice->SetLight(0, &light[0]);
	pD3DDevice->LightEnable(0, FALSE);
	pD3DDevice->SetLight(1, &light[1]);
	pD3DDevice->LightEnable(1, FALSE);
	pD3DDevice->SetLight(2, &light[2]);
	pD3DDevice->LightEnable(2, TRUE);
	pD3DDevice->SetLight(3, &light[3]);
	pD3DDevice->LightEnable(3, TRUE);

	
}


//================================================================
//ここを追加　3Dポリゴン用頂点の準備
bool Graphics::InitializeVB()
{
	this->pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&this->pVB_Sprite,
		NULL);
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this->pVB_Sprite->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);		//Left top
	pVtx[1].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.0f);		//Right top
	pVtx[2].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.0f);		//Right bottom
	pVtx[3].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);		//Left bottom

	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);		//Left top
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);		//Right top
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);		//Right bottom
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);		//Left bottom

	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);			//Left top
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);			//Right top
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);			//Right bottom
	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);			//Left bottom

	//頂点データをアンロックする
	this->pVB_Sprite->Unlock();


	//オブジェクトの頂点バッファを生成
	this->pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * 6,	//頂点データ用に確保するバッファサイズ
		D3DUSAGE_WRITEONLY,								//頂点データの使用法
		FVF_VERTEX3D,									//使用する頂点フォーマット
		D3DPOOL_MANAGED,								//リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuff,									//頂点バッファインターフェースへのポインタ
		NULL);											//NULLに設定

	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx2;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx2, 0);

	//1
	/*****************************************************************************************/
	//頂点座標の設定
	pVtx2[0].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx2[1].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx2[2].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx2[3].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	//法線ベクトルの設定
	pVtx2[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//反射光の設定
	pVtx2[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[3].tex = D3DXVECTOR2(0.0f, 1.0f);

	//2
	/********************************************************************************/
	//頂点座標の設定
	pVtx2[4].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx2[5].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx2[6].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx2[7].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	//法線ベクトルの設定
	pVtx2[4].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx2[5].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx2[6].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx2[7].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	//反射光の設定
	pVtx2[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[6].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[7].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx2[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[7].tex = D3DXVECTOR2(0.0f, 1.0f);

	//3
	/********************************************************************************/
	//頂点座標の設定
	pVtx2[8].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx2[9].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx2[10].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVtx2[11].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);

	//法線ベクトルの設定
	pVtx2[8].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx2[9].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx2[10].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx2[11].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//反射光の設定
	pVtx2[8].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[9].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[10].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[11].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx2[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[10].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[11].tex = D3DXVECTOR2(0.0f, 1.0f);

	//4
	/********************************************************************************/
	//頂点座標の設定
	pVtx2[12].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx2[13].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx2[14].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx2[15].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	//法線ベクトルの設定
	pVtx2[12].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[13].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[14].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[15].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//反射光の設定
	pVtx2[12].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[13].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[15].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx2[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[14].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[15].tex = D3DXVECTOR2(0.0f, 1.0f);

	//5
	/********************************************************************************/
	//頂点座標の設定
	pVtx2[16].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx2[17].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx2[18].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx2[19].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);

	//法線ベクトルの設定
	pVtx2[16].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx2[17].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx2[18].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx2[19].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//反射光の設定
	pVtx2[16].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[18].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[19].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx2[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[18].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[19].tex = D3DXVECTOR2(0.0f, 1.0f);

	//6
	/********************************************************************************/
	//頂点座標の設定
	pVtx2[20].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx2[21].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx2[22].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx2[23].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	//法線ベクトルの設定
	pVtx2[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx2[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx2[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx2[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//反射光の設定
	pVtx2[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[21].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[22].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx2[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[22].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[23].tex = D3DXVECTOR2(0.0f, 1.0f);

	//頂点データをアンロックする
	pD3DVtxBuff->Unlock();

	return true;
}

void Graphics::DrawWall()
{
	for (int i = 0; i < MAX_BUILD; i++)
	{
		int mapType = Build[i];
		if (mapType > TOTAL_MAP_STYLE || mapType <= 0)
		{
			break;
		}
		mapType--;
		for (int y = 0; y < MAP_Y; y++)
		{
			for (int x = 0; x < MAP_X; x++)
			{
				D3DXMATRIX mtxTrs;					//平行移動行列

				D3DXVECTOR3 pos = D3DXVECTOR3(-(float)(MAP_X / 2) + (float)x, i * MAP_Y + y, 0);					//位置

				D3DXMatrixIdentity(&mtxWorld);	//ワールド行列の単位行列の初期化

				//平行移動行列を作成＆ワールド行列への合成
				D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs);	//World*Tranlation

				//ワールドマトリックスを設定
				this->pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
				//描画したいポリゴンの頂点バッファをデータストリーム(データの通り道)にセット
				this->pD3DDevice->SetStreamSource(0, pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				//描画したいポリゴンの頂点フォーマットを設定
				this->pD3DDevice->SetFVF(FVF_VERTEX3D);

				int check = map[mapType][y][x];
				//ポリゴンの描画
				switch (check)
				{
				case 1:
					this->pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

					break;
				case 2:
					this->pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

					break;
				default:
					break;
				}
				this->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				this->pD3DDevice->SetTexture(0, this->textureController.GetTexture(TEXTURE_INDEX_WALL));
				this->pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
				this->pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 4, 2);
				this->pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 8, 2);
				this->pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 12, 2);
				this->pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 16, 2);
				this->pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 20, 2);
			}
		}
	}
}



void Graphics::Draw()
{

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DXMATRIX mtxScl;					//スケーリング行列
	D3DXMATRIX mtxRot;					//回転行列
	D3DXMATRIX mtxTrs;					//平行移動行列

	

	for (int j = 0; j < totalMesh; j++)
	{
		D3DXMatrixIdentity(&mtxWorld);	//ワールド行列の単位行列の初期化
	//スケール行列を作成＆ワールド行列へ合成
		D3DXMatrixScaling(&mtxScl, this->pMesh[j].size.x, this->pMesh[j].size.y, 1);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);	//World*Scaling

		//回転行列を作成＆ワールド行列への合成
		D3DXMatrixRotationYawPitchRoll(&mtxRot, this->pMesh[j].rot.y, this->pMesh[j].rot.x, this->pMesh[j].rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);	//World*Rotation
		//平行移動行列を作成＆ワールド行列への合成
		D3DXMatrixTranslation(&mtxTrs, this->pMesh[j].pos.x, this->pMesh[j].pos.y, this->pMesh[j].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs);	//World*Tranlation

		pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


		for (int i = 0; i < this->meshController.mesh_members[pMesh[j].index].numMaterials; i++)    // loop through each subset
		{
			pD3DDevice->SetMaterial(&this->meshController.mesh_members[pMesh[j].index].material[i]);    // set the appropriate material for the subset
			if (this->meshController.mesh_members[pMesh[j].index].texture[i] != NULL)    // if the subset has a texture (if texture is not NULL)
				pD3DDevice->SetTexture(0, this->meshController.mesh_members[pMesh[j].index].texture[i]);    // ...then set the texture

			this->meshController.mesh_members[pMesh[j].index].pDXMeshModel->DrawSubset(i);    // draw the subset
		}
	}
}

void Graphics::DrawUI()
{
	for (int index = 0; index < totalSprites; index++)
	{
		D3DXVECTOR3 center(0, 0, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position(this->pSprite[index].pos.x, this->pSprite[index].pos.y, this->pSprite[index].pos.z);    // position at 50, 50 with no depth

		D3DXMATRIX mtxScl;					//スケーリング行列
		D3DXMATRIX mtxRot;					//回転行列
		D3DXMATRIX mtxTrs;					//平行移動行列

		D3DXMatrixIdentity(&mtxWorld);	//ワールド行列の単位行列の初期化
		//スケール行列を作成＆ワールド行列へ合成
		D3DXMatrixScaling(&mtxScl, this->pSprite[index].size.x, this->pSprite[index].size.y, 1);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);	//World*Scaling

		//回転行列を作成＆ワールド行列への合成
		D3DXMatrixRotationYawPitchRoll(&mtxRot, this->pSprite[index].rot.y, this->pSprite[index].rot.x, this->pSprite[index].rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);	//World*Rotation

		this->pD3Dspt->SetTransform(&mtxWorld);

		this->pD3Dspt->Draw(this->textureController.GetTexture((TextureIndex)this->pSprite[index].index), NULL,
			&center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));  // draw it!


	}
}

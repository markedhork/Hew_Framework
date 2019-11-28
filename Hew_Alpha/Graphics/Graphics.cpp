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

	this->camera.SetProjectionValues((float)this->windowWidth / (float)this->windowHeight);

	return true;
}

void Graphics::Set(Sprite* sprite, int total)
{
	this->pSprite = sprite;
	this->totalSprites = total;
	this->pSpritesIndex = new int[this->totalSprites];

	for (int i = 0; i < this->totalSprites; i++)
	{
		this->pSpritesIndex[i] = sprite[i].index;
	}
	this->InitializeVB();
}


void Graphics::RenderFrame()
{
	// ��ʂ̃N���A
	this->pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(200, 200, 200, 255), 1.0f, 0);

	// �`��o�b�`���߂̊J�n
	this->pD3DDevice->BeginScene();

	this->pD3DDevice->SetTransform(D3DTS_VIEW, &this->camera.GetViewMatrix());
	this->pD3DDevice->SetTransform(D3DTS_PROJECTION, &this->camera.GetProjectionMatrix());

	this->DrawWall();
	this->Draw();

	// �`��o�b�`���߂̏I��
	this->pD3DDevice->EndScene();
	//Render Draw Data
	this->pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	// Direct3D�C���^�[�t�F�[�X�̎擾
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (pD3D == NULL) {
		// Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s
		MessageBox(hwnd, "Direct3D�C���^�[�t�F�[�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		OutputDebugStringA("failed to create Direct3D interface \n");
		return false;
	}

	// �f�o�C�X�̐����ɗ��p������\���̂̍쐬
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = this->windowWidth;                       // �o�b�N�o�b�t�@�̉���				
	d3dpp.BackBufferHeight = this->windowHeight;                     // �o�b�N�o�b�t�@�̏c��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;                    // �o�b�N�o�b�t�@�̃t�H�[�}�b�g�w��
	d3dpp.BackBufferCount = 1;                                  // �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // �X���b�v�G�t�F�N�g�i�X���b�v���@�j�̐ݒ�		
	d3dpp.Windowed = TRUE;                                      // �E�B���h�E���[�h or �t���X�N���[��
	d3dpp.EnableAutoDepthStencil = TRUE;                        // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̎g�p				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̃t�H�[�}�b�g�w��
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // �t���X�N���[�����̃��t���b�V�����[�g�̎w��
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;       // ���t���b�V�����[�g��Present�����̊֌W
	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice);

	// Direct3D�f�o�C�X�̎擾
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
		// �f�o�C�X�̍쐬�Ɏ��s
		MessageBox(hwnd, "Direct3D�f�o�C�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		OutputDebugStringA("failed to create Direct3D device \n");

		return false;
	}

	// �T���v���[�̐ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// �A���t�@�u�����h�̐ݒ�
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���_�J���[�ƃe�N�X�`���̃u�����h�ݒ�
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return true;
}


bool Graphics::UninitializeDirectX(HWND hwnd)
{
	//Texture release
	this->textureController.Release();

	// Direct3D�f�o�C�X�̉��
	if (pD3DDevice) {
		pD3DDevice->Release();
		pD3DDevice = NULL;
	}

	// Direct3D�C���^�[�t�F�[�X�̉��
	if (pD3D) {
		pD3D->Release();
		pD3D = NULL;
	}
	return true;
}


//================================================================
//������ǉ��@3D�|���S���p���_�̏���

bool Graphics::InitializeVB()
{
	this->pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&this->pVB_Sprite,
		NULL);
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
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

	//���_�f�[�^���A�����b�N����
	this->pVB_Sprite->Unlock();


	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	this->pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * 6,	//���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,								//���_�f�[�^�̎g�p�@
		FVF_VERTEX3D,									//�g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,								//���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuff,									//���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);											//NULL�ɐݒ�

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx2;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx2, 0);

	//1
	/*****************************************************************************************/
	//���_���W�̐ݒ�
	pVtx2[0].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx2[1].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx2[2].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx2[3].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	//�@���x�N�g���̐ݒ�
	pVtx2[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���ˌ��̐ݒ�
	pVtx2[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[3].tex = D3DXVECTOR2(0.0f, 1.0f);

	//2
	/********************************************************************************/
	//���_���W�̐ݒ�
	pVtx2[4].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx2[5].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx2[6].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx2[7].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	//�@���x�N�g���̐ݒ�
	pVtx2[4].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx2[5].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx2[6].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx2[7].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	//���ˌ��̐ݒ�
	pVtx2[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[6].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[7].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx2[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[7].tex = D3DXVECTOR2(0.0f, 1.0f);

	//3
	/********************************************************************************/
	//���_���W�̐ݒ�
	pVtx2[8].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx2[9].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx2[10].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVtx2[11].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);

	//�@���x�N�g���̐ݒ�
	pVtx2[8].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx2[9].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx2[10].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx2[11].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//���ˌ��̐ݒ�
	pVtx2[8].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[9].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[10].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[11].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx2[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[10].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[11].tex = D3DXVECTOR2(0.0f, 1.0f);

	//4
	/********************************************************************************/
	//���_���W�̐ݒ�
	pVtx2[12].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx2[13].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx2[14].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx2[15].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	//�@���x�N�g���̐ݒ�
	pVtx2[12].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[13].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[14].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx2[15].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���ˌ��̐ݒ�
	pVtx2[12].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[13].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[15].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx2[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[14].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[15].tex = D3DXVECTOR2(0.0f, 1.0f);

	//5
	/********************************************************************************/
	//���_���W�̐ݒ�
	pVtx2[16].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx2[17].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx2[18].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx2[19].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);

	//�@���x�N�g���̐ݒ�
	pVtx2[16].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx2[17].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx2[18].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx2[19].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���ˌ��̐ݒ�
	pVtx2[16].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[18].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[19].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx2[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[18].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[19].tex = D3DXVECTOR2(0.0f, 1.0f);

	//6
	/********************************************************************************/
	//���_���W�̐ݒ�
	pVtx2[20].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx2[21].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx2[22].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx2[23].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	//�@���x�N�g���̐ݒ�
	pVtx2[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx2[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx2[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx2[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//���ˌ��̐ݒ�
	pVtx2[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[21].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[22].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx2[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx2[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2[22].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx2[23].tex = D3DXVECTOR2(0.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
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
				D3DXMATRIX mtxTrs;					//���s�ړ��s��

				D3DXVECTOR3 pos = D3DXVECTOR3(-(float)(MAP_X / 2) + (float)x, i * MAP_Y + y, 0);					//�ʒu

				D3DXMatrixIdentity(&mtxWorld);	//���[���h�s��̒P�ʍs��̏�����

				//���s�ړ��s����쐬�����[���h�s��ւ̍���
				D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs);	//World*Tranlation

				//���[���h�}�g���b�N�X��ݒ�
				this->pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
				//�`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[��(�f�[�^�̒ʂ蓹)�ɃZ�b�g
				this->pD3DDevice->SetStreamSource(0, pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				//�`�悵�����|���S���̒��_�t�H�[�}�b�g��ݒ�
				this->pD3DDevice->SetFVF(FVF_VERTEX3D);

				int check = map[mapType][y][x];
				//�|���S���̕`��
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
				this->pD3DDevice->SetTexture(0, this->textureController.GetTexture(TEXTURE_INDEX_KIZUNA));
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
	for (int index = 0; index < totalSprites; index++)
	{
		D3DXMATRIX mtxScl;					//�X�P�[�����O�s��
		D3DXMATRIX mtxRot;					//��]�s��
		D3DXMATRIX mtxTrs;					//���s�ړ��s��

		D3DXMatrixIdentity(&mtxWorld);	//���[���h�s��̒P�ʍs��̏�����
		//�X�P�[���s����쐬�����[���h�s��֍���
		D3DXMatrixScaling(&mtxScl, this->pSprite[index].size.x, this->pSprite[index].size.y, this->pSprite[index].size.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);	//World*Scaling
		//��]�s����쐬�����[���h�s��ւ̍���
		D3DXMatrixRotationYawPitchRoll(&mtxRot, this->pSprite[index].rot.y, this->pSprite[index].rot.x, this->pSprite[index].rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);	//World*Rotation
		//���s�ړ��s����쐬�����[���h�s��ւ̍���
		D3DXMatrixTranslation(&mtxTrs, this->pSprite[index].pos.x, this->pSprite[index].pos.y, this->pSprite[index].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs);	//World*Tranlation

		//���[���h�}�g���b�N�X��ݒ�
		this->pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		this->pD3DDevice->SetStreamSource(0, this->pD3DVtxBuff, 0, sizeof(VERTEX_3D));
		this->pD3DDevice->SetFVF(FVF_VERTEX3D);
		this->pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		this->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		this->pD3DDevice->SetTexture(0, this->textureController.GetTexture((TextureIndex)this->pSprite[index].index));
		this->pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}
}
#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define SHOOT_POINT_X			(0.0f)
#define SHOOT_POINT_Y			(10.0f)
#define SHOOT_POINT_Z			(-10.0f)
#define SHOOTING_DEGREE			(30.0f)
#define WALL_DISTANCE			(50.0f)
#define FLYING_TIME				(2.0f)
#define GRAVITY					(6.0f)
#define BALL_SIZE				(0.2f)

#define	MAX_POINT				(500)
#define MESH_FILE_PATH			"asset/mesh/Soccer.x"

struct NewPoint
{
	bool use = false;
	D3DXVECTOR2 pos;
};

class Ball
{
public:
	Ball();
	~Ball();
	void SetDevice(LPDIRECT3DDEVICE9 pD3Device);
	void Update();
	void Draw();
	void Generate(D3DXVECTOR3 pos, float pst);
	void CreateMeshBuffer();
	void Uninitialize();

	NewPoint points[MAX_POINT];
	int pointTotal = 0;
private:
	struct BallStruct
	{
		bool use;
		float x, y, z;
		float degree;
		float vz, vy;
		float time;
	};
	BallStruct		ball;

	LPD3DXMESH		pDXMeshModel;		//モデルを表すインタフェース
	LPDIRECT3DTEXTURE9 texture = NULL;

	DWORD			numMaterials;		//マテリアルの数
	D3DMATERIAL9	*material;    // create the material struct
	LPDIRECT3DDEVICE9 pDevice;
};


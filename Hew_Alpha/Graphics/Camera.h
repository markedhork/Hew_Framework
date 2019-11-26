#pragma once
#include <d3dx9.h>
//プロジェクション用パラメータ
#define VIEW_ANGLE		(D3DXToRadian(45.0f))								//ビュー平面の視野角
#define VIEW_NEAR_Z		(0.2f)												//ビュー平面のNearZ値
#define VIEW_FAR_Z		(1000.0f)	
class Camera
{
public:
	Camera();
	void SetProjectionValues(float aspectRatio);

	const D3DXMATRIX & GetViewMatrix() const;
	const D3DXMATRIX & GetProjectionMatrix() const;

	const D3DXVECTOR3 & GetPositionVector() const;
	const D3DXVECTOR3 & GetRotationVector() const;

	void SetPosition(const D3DXVECTOR3 & pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const D3DXVECTOR3 & pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const D3DXVECTOR3 & rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const D3DXVECTOR3 & rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(D3DXVECTOR3 lookAtPos);

	const D3DXVECTOR3 & GetForwardVector();
	const D3DXVECTOR3 & GetRightVector();
	const D3DXVECTOR3 & GetBackwardVector();
	const D3DXVECTOR3 & GetLeftVector();

private:
	void UpdateViewMatrix();
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;

	const D3DXVECTOR3 DEFAULT_FORWARD_VECTOR = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	const D3DXVECTOR3 DEFAULT_UP_VECTOR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 DEFAULT_BACKWARD_VECTOR = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	const D3DXVECTOR3 DEFAULT_LEFT_VECTOR = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 DEFAULT_RIGHT_VECTOR = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	D3DXVECTOR3 vec_forward;
	D3DXVECTOR3 vec_left;
	D3DXVECTOR3 vec_right;
	D3DXVECTOR3 vec_backward;
};
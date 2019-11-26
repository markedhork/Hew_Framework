//2019/08/04
#include "Camera.h"

Camera::Camera()
{
	this->pos = D3DXVECTOR3(0, 0, 0);
	this->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float aspectRatio)
{
	D3DXMatrixPerspectiveFovLH(&projectionMatrix,
		VIEW_ANGLE, aspectRatio, VIEW_NEAR_Z, VIEW_FAR_Z);
}

const D3DXMATRIX & Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const D3DXMATRIX & Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const D3DXVECTOR3 & Camera::GetPositionVector() const
{
	return this->pos;
}

const D3DXVECTOR3 & Camera::GetRotationVector() const
{
	return this->rot;
}


void Camera::SetPosition(const D3DXVECTOR3 & pos)
{
	this->pos = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = D3DXVECTOR3(x, y, z);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const D3DXVECTOR3 & pos)
{
	this->pos += pos;
	this->UpdateViewMatrix();
}


void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->UpdateViewMatrix();
}



void Camera::SetRotation(const D3DXVECTOR3 & rot)
{
	this->rot = rot;
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = D3DXVECTOR3(x, y, z);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const D3DXVECTOR3 & rot)
{
	this->rot += rot;
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(D3DXVECTOR3 lookAtPos)
{
	//you can not see yourself
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
	{
		yaw += D3DX_PI;
	}
	this->SetRotation(pitch, yaw, 0.0f);
}

const D3DXVECTOR3 & Camera::GetForwardVector()
{
	return this->vec_forward;
}

const D3DXVECTOR3 & Camera::GetRightVector()
{
	return this->vec_right;
}

const D3DXVECTOR3 & Camera::GetBackwardVector()
{
	return this->vec_backward;
}

const D3DXVECTOR3 & Camera::GetLeftVector()
{
	return this->vec_left;
}

void Camera::UpdateViewMatrix()		//Update view matrix and this movement vectors
{
	//Calculate camera rotation matrix
	D3DXMATRIX camRotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&camRotationMatrix,this->rot.x, this->rot.y, this->rot.z);
	
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	D3DXVECTOR3 camTarget;
	D3DXVec3TransformCoord(&camTarget, &this->DEFAULT_FORWARD_VECTOR, &camRotationMatrix);

	//Adjust cam target to be offset by the camera's current position
	camTarget += this->pos;
	//Calculate up direction based on current rotation
	D3DXVECTOR3 upDir;
	D3DXVec3TransformCoord(&upDir, &this->DEFAULT_UP_VECTOR, &camRotationMatrix);

	//Rebuild view matrix
	D3DXMatrixLookAtLH(&this->viewMatrix, &this->pos, &camTarget, &upDir);

	D3DXMATRIX vecRotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&vecRotationMatrix, this->rot.x, this->rot.y, this->rot.z);
	
	D3DXVec3TransformCoord(&this->vec_forward, &this->DEFAULT_FORWARD_VECTOR, &vecRotationMatrix);
	D3DXVec3TransformCoord(&this->vec_backward, &this->DEFAULT_BACKWARD_VECTOR, &vecRotationMatrix);
	D3DXVec3TransformCoord(&this->vec_left, &this->DEFAULT_LEFT_VECTOR, &vecRotationMatrix);
	D3DXVec3TransformCoord(&this->vec_right, &this->DEFAULT_RIGHT_VECTOR, &vecRotationMatrix);
}

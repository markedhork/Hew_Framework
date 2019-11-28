#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseClass
{
public:
	void OnLeftPressed(int x, int y);
	void OnLeftDoubleClick(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightDoubleClick(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown();
	bool IsLeftDoubleClick();
	bool IsMiddleDown();
	bool IsRightDown();
	bool IsRightDoubleClick();

	int GetPosX();
	int GetPosY();
	MousePoint GetPos();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool leftDoubleClick = false;
	bool RightIsDown = false;
	bool RightDoubleClick = false;
	bool mbuttonIsDown = false;
	int x = 0;
	int y = 0;
};

#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnLeftDoubleClick(int x, int y)
{
	this->leftDoubleClick = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LDoubleClick, x, y));
}

void MouseClass::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
 	this->RightIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightDoubleClick(int x, int y)
{
	this->RightDoubleClick = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RDoubleClick, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{
	this->RightIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	this->mbuttonIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y)
{
	this->mbuttonIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}
void MouseClass::OnWheelDown(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown()
{
	return this->leftIsDown;
}

bool MouseClass::IsLeftDoubleClick()
{
	if (this->leftDoubleClick)
	{
		this->leftDoubleClick = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool MouseClass::IsMiddleDown()
{
	return this->mbuttonIsDown;
}

bool MouseClass::IsRightDown()
{
	return this->RightIsDown;
}

bool MouseClass::IsRightDoubleClick()
{
	if (this->RightDoubleClick)
	{
		this->RightDoubleClick = false;
		return true;
	}
	else
	{
		return false;
	}
}

int MouseClass::GetPosX()
{
	return this->x;
}

int MouseClass::GetPosY()
{
	return this->y;
}

MousePoint MouseClass::GetPos()
{
	return{ this->x,this->y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = this->eventBuffer.front();	//Get first event from buffer
		this->eventBuffer.pop();					//Remove first event from buffer
		return e;
	}
}
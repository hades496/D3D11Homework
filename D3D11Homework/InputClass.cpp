#include "pch.h"
#include "InputClass.h"
#include <winapifamily.h>
using namespace Windows::UI::Core;
using namespace Windows::System;
InputClass::InputClass():
Listener(CoreWindow::GetForCurrentThread())
{
	
}

InputClass::~InputClass()
{
}

const bool InputClass::IsWPressed()
{
	return Listener->GetAsyncKeyState(VirtualKey::W) == CoreVirtualKeyStates::Down;
}

const bool InputClass::IsSPressed()
{
	return Listener->GetAsyncKeyState(VirtualKey::S) == CoreVirtualKeyStates::Down;
}

const bool InputClass::IsAPressed()
{
	return Listener->GetAsyncKeyState(VirtualKey::A) == CoreVirtualKeyStates::Down;
}

const bool InputClass::IsDPressed()
{
	return Listener->GetAsyncKeyState(VirtualKey::D) == CoreVirtualKeyStates::Down;
}

const bool InputClass::IsQPressed()
{
	return Listener->GetAsyncKeyState(VirtualKey::Q) == CoreVirtualKeyStates::Down;
}

const bool InputClass::IsEPressed()
{
	return Listener->GetAsyncKeyState(VirtualKey::E) == CoreVirtualKeyStates::Down;
}



#pragma once
#include <pch.h>
using namespace Windows::UI::Core;
using namespace Platform;
class InputClass
{
	//CoreWindow ^ Listener;
	Agile<CoreWindow> Listener;
public:
	InputClass();
	~InputClass();
	const bool IsWPressed();
	const bool IsSPressed();

};


#include "pch.h"
#include "MoveLookController.h"

using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Devices::Input;
using namespace DirectX;

void MoveLookController::OnPointerPressed(
	_In_ CoreWindow^ sender,
	_In_ PointerEventArgs^ args)
{
	// Get the current pointer position.
	uint32 pointerID = args->CurrentPoint->PointerId;
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);

	auto device = args->CurrentPoint->PointerDevice;
	auto deviceType = device->PointerDeviceType;
	if (deviceType == PointerDeviceType::Mouse)
	{
		// Action, Jump, or Fire
	}

	// Check  if this pointer is in the move control.
	// Change the values  to percentages of the preferred screen resolution.
	// You can set the x value to <preferred resolution> * <percentage of width>
	// for example, ( position.x < (screenResolution.x * 0.15) ).

	if ((position.x < 300 && position.y > 380) && (deviceType != PointerDeviceType::Mouse))
	{
		if (!m_moveInUse) // if no pointer is in this control yet
		{
			// Process a DPad touch down event.
			m_moveFirstDown = position;                 // Save the location of the initial contact.
			m_movePointerPosition = position;
			m_movePointerID = pointerID;                // Store the id of the pointer using this control.
			m_moveInUse = TRUE;
		}
	}
	else // This pointer must be in the look control.
	{
		if (!m_lookInUse) // If no pointer is in this control yet...
		{
			m_lookLastPoint = position;                         // save the point for later move
			m_lookPointerID = args->CurrentPoint->PointerId;  // store the id of pointer using this control
			m_lookLastDelta.x = m_lookLastDelta.y = 0;          // these are for smoothing
			m_lookInUse = TRUE;
		}
	}
}


void MoveLookController::OnPointerMoved(
	_In_ CoreWindow ^sender,
	_In_ PointerEventArgs ^args)
{
	uint32 pointerID = args->CurrentPoint->PointerId;
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);

	// Decide which control this pointer is operating.
	if (pointerID == m_movePointerID)           // This is the move pointer.
	{
		// Move control
		m_movePointerPosition = position;       // Save the current position.

	}
	else if (pointerID == m_lookPointerID)      // This is the look pointer.
	{
		// Look control

		DirectX::XMFLOAT2 pointerDelta;
		pointerDelta.x = position.x - m_lookLastPoint.x;        // How far did pointer move
		pointerDelta.y = position.y - m_lookLastPoint.y;

		DirectX::XMFLOAT2 rotationDelta;
		rotationDelta.x = pointerDelta.x * ROTATION_GAIN;   // Scale for control sensitivity.
		rotationDelta.y = pointerDelta.y * ROTATION_GAIN;

		m_lookLastPoint = position;                     // Save for the next time through.

														// Update our orientation based on the command.
		m_pitch -= rotationDelta.y;                     // Mouse y increases down, but pitch increases up.
		m_yaw -= rotationDelta.x;                       // Yaw is defined as CCW around the y-axis.

														// Limit the pitch to straight up or straight down.
		m_pitch = (float)__max(-DirectX::XM_PI / 2.0f, m_pitch);
		m_pitch = (float)__min(+DirectX::XM_PI / 2.0f, m_pitch);
	}
}


void MoveLookController::OnPointerReleased(
	_In_ CoreWindow ^sender,
	_In_ PointerEventArgs ^args)
{
	uint32 pointerID = args->CurrentPoint->PointerId;
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);


	if (pointerID == m_movePointerID)    // This was the move pointer.
	{
		m_moveInUse = FALSE;
		m_movePointerID = 0;
	}
	else if (pointerID == m_lookPointerID) // This was the look pointer.
	{
		m_lookInUse = FALSE;
		m_lookPointerID = 0;
	}
}


void MoveLookController::OnKeyDown(
	__in CoreWindow^ sender,
	__in KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)     // Forward
		m_forward = true;
	if (Key == VirtualKey::S)     // Back
		m_back = true;
	if (Key == VirtualKey::A)     // Left
		m_left = true;
	if (Key == VirtualKey::D)     // Right
		m_right = true;
	if (Key == VirtualKey::Q)	  // Up
		m_up = true;
	if (Key == VirtualKey::E)	  // Down
		m_down = true;
	if (Key == VirtualKey::O)	  // On Box
		m_onBox = true;
	if (Key == VirtualKey::P)	  // not On Box
		m_onBox = false;

}

void MoveLookController::OnKeyUp(
	__in CoreWindow^ sender,
	__in KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)     // forward
		m_forward = false;
	if (Key == VirtualKey::S)     // back
		m_back = false;
	if (Key == VirtualKey::A)     // left
		m_left = false;
	if (Key == VirtualKey::D)     // right
		m_right = false;
	if (Key == VirtualKey::Q)	  // Up
		m_up = false;
	if (Key == VirtualKey::E)	  // Down
		m_down = false;
}

void MoveLookController::Initialize(_In_ CoreWindow^ window)
{

	// Opt in to recieve touch/mouse events.
	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &MoveLookController::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &MoveLookController::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &MoveLookController::OnPointerReleased);

	//window->CharacterReceived +=
	//	ref new TypedEventHandler<CoreWindow^, CharacterReceivedEventArgs^>(this, &MoveLookController::OnCharacterReceived);

	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MoveLookController::OnKeyDown);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MoveLookController::OnKeyUp);

	// Initialize the state of the controller.
	m_moveInUse = FALSE;                // No pointer is in the Move control.
	m_movePointerID = 0;

	m_lookInUse = FALSE;                // No pointer is in the Look control.
	m_lookPointerID = 0;

	//  Need to init this as it is reset every frame.
	m_moveCommand = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	SetOrientation(0, 0);             // Look straight ahead when the app starts.

}

void MoveLookController::SetPosition(_In_ DirectX::XMFLOAT3 pos)
{
	m_position = pos;
}

// Accessor to set the position of the controller.
void MoveLookController::SetOrientation(_In_ float pitch, _In_ float yaw)
{
	m_pitch = pitch;
	m_yaw = yaw;
}

// Returns the position of the controller object.
DirectX::XMFLOAT3 MoveLookController::get_Position()
{
	return m_position;
}

// Returns the point at which the camera controller is facing.
DirectX::XMFLOAT3 MoveLookController::get_LookPoint()
{
	float y = sinf(m_pitch);        // Vertical
	float r = cosf(m_pitch);        // In the plane
	float z = r * cosf(m_yaw);        // Fwd-back
	float x = r * sinf(m_yaw);        // Left-right
	DirectX::XMFLOAT3 result(x, y, z);
	result.x += m_position.x;
	result.y += m_position.y;
	result.z += m_position.z;

	// Return m_position + DirectX::XMFLOAT3(x, y, z);
	return result;
}

// Returns the viewMatrix
XMMATRIX MoveLookController::get_ViewMatrix()
{
	XMFLOAT3 eye = get_Position();
	XMFLOAT3 at = get_LookPoint();
	XMFLOAT3 up(0.0f, 1.0f, 0.0f);

	return XMMatrixLookAtRH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up));
}

bool MoveLookController::OnBox()
{
	return m_onBox;
}

void MoveLookController::Update(CoreWindow ^window)
{
	// Check for input from the Move control.
	if (m_moveInUse)
	{
		DirectX::XMFLOAT2 pointerDelta(m_movePointerPosition);
		pointerDelta.x -= m_moveFirstDown.x;
		pointerDelta.y -= m_moveFirstDown.y;

		// Figure out the command from the touch-based virtual joystick.
		if (pointerDelta.x > 16.0f)      // Leave 32 pixel-wide dead spot for being still.
			m_moveCommand.x = 1.0f;
		else
			if (pointerDelta.x < -16.0f)
				m_moveCommand.x = -1.0f;

		if (pointerDelta.y > 16.0f)      // Joystick y is up, so change sign.
			m_moveCommand.y = -1.0f;
		else
			if (pointerDelta.y < -16.0f)
				m_moveCommand.y = 1.0f;
	}

	// Poll our state bits that are set by the keyboard input events.
	if (!m_onBox)
	{
		if (m_forward)
			m_moveCommand.y += 1.0f;
		if (m_back)
			m_moveCommand.y -= 1.0f;

		if (m_left)
			m_moveCommand.x -= 1.0f;
		if (m_right)
			m_moveCommand.x += 1.0f;

		if (m_up)
			m_moveCommand.z += 1.0f;
		if (m_down)
			m_moveCommand.z -= 1.0f;
	}
	

	// Make sure that 45 degree cases are not faster.
	DirectX::XMFLOAT3 command = m_moveCommand;
	DirectX::XMVECTOR vector;
	vector = DirectX::XMLoadFloat3(&command);

	if (fabsf(command.x) > 0.1f || fabsf(command.y) > 0.1f || fabsf(command.z) > 0.1f)
	{
		vector = DirectX::XMVector3Normalize(vector);
		DirectX::XMStoreFloat3(&command, vector);
	}


	// Rotate command to align with our direction (world coordinates).
	DirectX::XMFLOAT3 wCommand;
	wCommand.x = command.x*cosf(m_yaw) - command.y*sinf(m_yaw);
	wCommand.y = command.x*sinf(m_yaw) + command.y*cosf(m_yaw);
	wCommand.z = command.z;

	// Scale for sensitivity adjustment.
	wCommand.x = wCommand.x * MOVEMENT_GAIN;
	wCommand.y = wCommand.y * MOVEMENT_GAIN;
	wCommand.z = wCommand.z * MOVEMENT_GAIN;

	// Our velocity is based on the command.
	// Also note that y is the up-down axis. 
	DirectX::XMFLOAT3 Velocity;
	Velocity.x = -wCommand.x;
	Velocity.z = wCommand.y;
	Velocity.y = wCommand.z;

	// Integrate
	m_position.x += Velocity.x;
	m_position.y += Velocity.y;
	m_position.z += Velocity.z;

	if (m_position.y < 0.1f) m_position.y = 0.1f;
	

	// Clear movement input accumulator for use during the next frame.
	m_moveCommand = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}


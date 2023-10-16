
namespace myLib

struct CameraControls
{
	double prevMouseX, prevMouseY
	float yaw = 0, pit = 0;
	float mouseSensitivity;
	bool firstMouse = true;
	float moveSpeed = 5.0f;
};
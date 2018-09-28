#ifndef _USER_IO_HG_
#define _USER_IO_HG_
#include <GLFW/glfw3.h>

class UserIO
{
public:
	static void key_callback(GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mods);
};

#endif // !_USER_IO_HG_




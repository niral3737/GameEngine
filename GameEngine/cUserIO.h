#ifndef _USER_IO_HG_
#define _USER_IO_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class cUserIO
{
public:
	static void key_callback(GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mods);
	static void processAsynKeys(GLFWwindow* window);

private:
	static bool mIsShiftDown(GLFWwindow* window);
	static bool mIsCtrlDown(GLFWwindow* window);
	static bool mIsAltDown(GLFWwindow* window);
	static bool mAreAllModifiersUp(GLFWwindow* window);
};

#endif // !_USER_IO_HG_




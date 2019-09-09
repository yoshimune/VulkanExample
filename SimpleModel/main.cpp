#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <array>
#include <cassert>
#include <sstream>
#include <numeric>

#include <GLFW/glfw3.h>


const int WindowWidth = 640;
const int WindowHeight = 480;

const char* AppTitle = "SimpleModel";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	auto window = glfwCreateWindow(WindowWidth, WindowHeight, AppTitle, nullptr, nullptr);

	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		glfwPollEvents();
	}

	// App 終了
	glfwTerminate();
	return 0;
}
#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cassert>
#include <sstream>
#include <numeric>
#include <iostream>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan_win32.h>

class VulkanBase
{
public:
	VulkanBase();
	~VulkanBase();

#ifdef _DEBUG 
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif
	const std::vector<const char*> validationLayers = {
		// SDK内にある一般的なvalidation layer
		"VK_LAYER_KHRONOS_validation"
	};
	virtual const std::vector<const char*> getValidationLayers() {
		return validationLayers;
	}

	virtual void start();


protected:
	GLFWwindow* window;

	virtual const int windowWidth() { return 960; }
	virtual const int windowHeight() { return 640; }

	virtual const char* appTitle() { return "VulkanBase"; }

	virtual void initialize();
	void initInstance();
	void initWindow();
	virtual void mainLoop();
	virtual void render();

	// 必要なValidationLayersがサポートされているか?
	virtual bool checkValidationLayerSupport();
};
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
		// SDK���ɂ����ʓI��validation layer
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> extensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#ifdef _DEBUG
		, VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#endif
	};

	virtual void start();


protected:
	GLFWwindow* window;

	// VK�I�u�W�F�N�g ====================================
	VkInstance  instance;

	virtual const int windowWidth() { return 960; }
	virtual const int windowHeight() { return 640; }

	virtual const char* appTitle() { return "VulkanBase"; }

	virtual void initialize();
	void initInstance();
	void initWindow();
	virtual void mainLoop();
	virtual void render();

	// �K�v��ValidationLayers���T�|�[�g����Ă��邩?
	bool checkValidationLayerSupport();

	// �K�v�Ȋg�����T�|�[�g����Ă��邩?
	bool checkExtensionSupport();

	// ���ʃ`�F�b�N
	void checkResult(VkResult result, const char* message);
};
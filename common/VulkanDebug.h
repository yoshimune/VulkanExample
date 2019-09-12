#pragma once

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cassert>
#include "vulkan/vulkan.h"

namespace VulkanDebug
{

	VKAPI_ATTR VkBool32 VKAPI_CALL messageCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char* pLayerPrefix,
		const char* pMsg,
		void* pUserData);

	void setupDebugging(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportCallbackEXT callBack);
	void freeDebugCallback(VkInstance instance);
}
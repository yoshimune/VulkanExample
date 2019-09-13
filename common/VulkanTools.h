#pragma once

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <vector>

#include "vulkan/vulkan.h"

namespace VulkanTools
{

#ifdef _DEBUG
#define TRACE(x) std::cout << x << std::endl;
#else
#define TRACE(x)
#endif // _DEBUG


#define VK_CHECK_RESULT(f)																								\
{																														\
	VkResult res = (f);																									\
	if (res != VK_SUCCESS)																								\
	{																													\
		std::cout << "Fatal : VkResult is \"" << res << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl;	\
		assert(res == VK_SUCCESS);																						\
	}																													\
}

#define VK_CHECK_RESULT_AND_RETURN(f)				\
{													\
	VK_CHECK_RESULT(f)								\
	return f;										\
}
	// バッファ作成
	VkResult createBuffer(
		VkPhysicalDevice physicalDevice,
		VkDevice device,
		VkBufferUsageFlags usageFlags,
		VkMemoryPropertyFlags memoryPropertyFlags,
		VkDeviceSize size,
		VkBuffer* buffer,
		VkDeviceMemory* memory,
		void* data = nullptr);

	// イメージ作成
	VkResult createImage(
		VkPhysicalDevice physicalDevice,
		VkDevice device,
		uint32_t width,
		uint32_t height,
		uint32_t mipLevels,
		VkSampleCountFlagBits numSamples,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory);

	// イメージビュー作成
	VkResult createImageView(
		VkDevice device,
		VkImage image,
		VkFormat format,
		VkImageAspectFlags aspectFlags,
		uint32_t mipLevels,
		VkImageView* pImageView);

	// サポートされているフォーマットを検索する
	VkFormat findSupportedFormat(
		VkPhysicalDevice physicalDevice,
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features);

	// メモリタイプを取得する
	uint32_t findMemoryType(
		VkPhysicalDevice physicalDevice,
		uint32_t typeFilter,
		VkMemoryPropertyFlags properties);
}

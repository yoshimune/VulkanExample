#pragma once

#include <stdexcept>
#include <vector>

#include "vulkan/vulkan.h"

namespace VulkanTools
{
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
}
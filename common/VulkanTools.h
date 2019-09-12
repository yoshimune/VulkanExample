#pragma once

#include <stdexcept>
#include <vector>

#include "vulkan/vulkan.h"

namespace VulkanTools
{
	// �C���[�W�r���[�쐬
	VkResult createImageView(
		VkDevice device,
		VkImage image,
		VkFormat format,
		VkImageAspectFlags aspectFlags,
		uint32_t mipLevels,
		VkImageView* pImageView);

	// �T�|�[�g����Ă���t�H�[�}�b�g����������
	VkFormat findSupportedFormat(
		VkPhysicalDevice physicalDevice,
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features);
}
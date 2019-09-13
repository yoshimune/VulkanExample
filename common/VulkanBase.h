#pragma once

#define NOMINMAX	// min/max関数の競合回避


#include <memory>
#include <vector>
#include <array>
#include <cassert>
#include <sstream>
#include <iostream>
#include <optional>
#include <set>
#include <algorithm>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan.h>


#include "../common/VulkanTools.h"
#include "../common/VulkanDebug.h"


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

	const std::vector<const char*> instanceExtensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#ifdef _DEBUG
		, VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#endif
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// キューファミリインデックス
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value()
				&& presentFamily.has_value();
		}
	};

	// スワップチェーン作成に必要な情報
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;		// 基本的なサーフェイス機能
		std::vector<VkSurfaceFormatKHR> formats;	// カラーフォーマット、カラースペース
		std::vector<VkPresentModeKHR> presentModes;	// 表示（Vsyncなど）モード
	};

	virtual void start();


protected:
	GLFWwindow* window;

	// VKオブジェクト ====================================
	VkInstance  instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkCommandPool commandPool;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	std::vector<VkFence> inFences;


	// プロパティ
	virtual const int windowWidth() { return 960; }
	virtual const int windowHeight() { return 640; }
	virtual const char* appTitle() { return "VulkanBase"; }

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	std::optional<VkFormat> depthFormat_;
	VkFormat depthFormat() {
		if(!depthFormat_.has_value()) { depthFormat_ = findDepthFormat(); }
		return depthFormat_.value();
	}


	virtual void initialize();
	void initWindow();
	void initInstance();
	void setupDebugMessenger();
	void createSurface();
	void selectPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createCommandPool();
	void createRenderPass();
	void createColorBuffer();
	void createDepthBuffer();
	void createFramebuffer();
	void createSyncObjects();
	void createCommandBuffers();

	virtual void prepare() {};
	virtual void mainLoop();
	virtual void render() {};
	virtual void cleanup() {};

	void terminate();


	// 必要なValidationLayersがサポートされているか?
	virtual bool checkValidationLayerSupport();

	// インスタンス拡張チェック
	virtual bool checkInstanceExtensionSupport();

	// 物理デバイス適合チェック
	virtual bool isDeviceSuitable(VkPhysicalDevice device);

	// キューファミリインデックスを検索
	virtual QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	// デバイス拡張サポートチェック
	virtual bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	// スワップチェーン作成に必要な情報を集める
	virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	// カラーフォーマット・カラースペースを選択する
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	// ディスプレイとの同期方法を選択する
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	// スワップチェーンの解像度を決定する
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// デプスフォーマットを検索する
	VkFormat findDepthFormat();

	// コマンドバッファの作成とレコード開始を行う
	VkCommandBuffer beginSingleTimeCommands();

	// コマンドバッファのレコード完了・サブミット・解放
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	// イメージレイアウト遷移
	void transitionImageLayout(
		VkImage image,
		VkFormat format,
		VkImageLayout oldLayout,
		VkImageLayout newLayout,
		uint32_t mipLevels);
};
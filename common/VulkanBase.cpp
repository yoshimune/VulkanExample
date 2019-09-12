#include "VulkanBase.h"

VulkanBase::VulkanBase()
{
}

VulkanBase::~VulkanBase()
{
}

void VulkanBase::start()
{

	initialize();
	mainLoop();

	// App 終了
	glfwTerminate();
}

void VulkanBase::initialize()
{
	initWindow();
	initInstance();
	createSurface();
	selectPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
}

void VulkanBase::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	window = glfwCreateWindow(windowWidth(), windowHeight(), appTitle(), nullptr, nullptr);

}

void VulkanBase::initInstance()
{
	// validationレイヤーをチェック&有効化
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not avilable!");
	}

	// 拡張情報のサポート状況確認
	if (!checkInstanceExtensionSupport()) {
		throw std::runtime_error("extensions requested, but not avilable!");
	}

	// アプリケーション情報を初期化
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appTitle();
	appInfo.pEngineName = appTitle();
	appInfo.apiVersion = VK_API_VERSION_1_1;
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	
	VkInstanceCreateInfo instaceCreateInfo{};
	instaceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instaceCreateInfo.enabledExtensionCount = uint32_t(instanceExtensions.size());
	instaceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instaceCreateInfo.pApplicationInfo = &appInfo;

#ifdef _DEBUG
	instaceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	instaceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#endif // DEBUG

	VK_CHECK_RESULT(vkCreateInstance(&instaceCreateInfo, nullptr, &instance));
}

void VulkanBase::createSurface()
{
	VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window, nullptr, &surface));
}

void VulkanBase::selectPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void VulkanBase::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// 物理デバイス機能
	VkPhysicalDeviceFeatures deviceFeatures = {};

	// 異方性フィルタリング有効化
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	// 論理デバイス作成情報
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	// 論理デバイス作成
	VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));

	// キュー取得
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}


VulkanBase::SwapChainSupportDetails VulkanBase::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	// スワップチェーン作成に必要な、基本的なサーフェイス機能の問い合わせ
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// スワップチェーン作成に必要な、基本的なサーフェイスが対応しているスワップチェーンフォーマットについて問い合わせる
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			device,
			surface,
			&formatCount,
			details.formats.data());
	}

	// サーフェイスに対応した、サポートされているpresentation modeの問い合わせ
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		device,
		surface,
		&presentModeCount,
		nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device,
			surface,
			&presentModeCount,
			details.presentModes.data());
	}

	return details;
}

void VulkanBase::mainLoop()
{

	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		glfwPollEvents();
		render();
	}
}

void VulkanBase::render()
{

}

bool VulkanBase::checkValidationLayerSupport()
{
	// 有効なレイヤーリストを取得
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// 必要なレイヤーリスト(validationLayers)と
	// 有効なレイヤーリスト(availableLayers)を比較し、
	// 必要なレイヤーリストが全て存在するかをチェック
	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			TRACE(layerProperties.layerName);

			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

bool VulkanBase::checkInstanceExtensionSupport()
{
	// 拡張情報の取得
	std::vector<VkExtensionProperties> props;
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	props.resize(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

	for (const auto& e : instanceExtensions)
	{
		bool extensionFound = false;

		for (const auto& p : props)
		{
			if (strcmp(e, p.extensionName) == 0) {
				extensionFound = true;
				break;
			}
		}
		if (!extensionFound) {
			TRACE(e);
			return false;
		}
	}
	return true;
}

bool VulkanBase::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty()
			&& !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

VulkanBase::QueueFamilyIndices VulkanBase::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}
		i++;
	}

	return indices;
}

bool VulkanBase::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// デバイスがサポートしている拡張のリストを取得する
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	// 有効化したい拡張と、使用可能な拡張を突き合わせる
	for (const auto& extension : deviceExtensions)
	{
		bool extensionFound = false;
		for (const auto& available : availableExtensions)
		{
			if (strcmp(extension, available.extensionName) == 0) {
				extensionFound = true;
				break;
			}
		}
		
		if (!extensionFound) {
			return false;
		}
	}
	return true;
}

void VulkanBase::createSwapChain()
{
	// スワップチェーン作成に必要な情報を集める
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

	// スワップチェーン作成要件から、最適な設定を選択する
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	// スワップチェーンが保持するイメージ数
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	// スワップチェーンイメージ数が最大数を超えていないかチェック
	if (swapChainSupport.capabilities.maxImageCount > 0
		&& imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// スワップチェーンオブジェクト作成情報を作成
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	if (indices.graphicsFamily != indices.presentFamily) {
		// グラフィックスキューファミリとプレゼンテーションキューファミリが異なる場合は
		// VK_SHARING_MODE_CONCURRENT（同時モード）を使用する
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		// グラフィックスキューファミリとプレゼンテーションキューファミリが同一の場合は
		// VK_SHARING_MODE_EXCLUSIVE（独占モード）を使用する
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;		// Optional
		createInfo.pQueueFamilyIndices = nullptr;	// Optional
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;	// ウィンドウが隠れたときなどはクリッピングする
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// スワップチェーン作成
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

VkSurfaceFormatKHR VulkanBase::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM	// BGRA 8bit unsignd int
			&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR	// SRGB
			) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanBase::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentModes : availablePresentModes) {
		if (availablePresentModes == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentModes;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanBase::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height),
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}
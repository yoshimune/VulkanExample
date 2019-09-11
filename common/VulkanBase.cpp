#include "VulkanBase.h"

using namespace std;

#ifdef _DEBUG
#define TRACE(x) std::cout << x << std::endl;
#else
#define TRACE(x)
#endif // _DEBUG


VulkanBase::VulkanBase()
	:window(nullptr)
{
}

VulkanBase::~VulkanBase()
{
}

void VulkanBase::checkResult(VkResult result, const char* message) {
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(message);
	}
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
}

void VulkanBase::initInstance()
{
	// validationレイヤーをチェック&有効化
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not avilable!");
	}

	// 拡張情報のサポート状況確認
	if (!checkExtensionSupport()) {
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
	instaceCreateInfo.enabledExtensionCount = uint32_t(extensions.size());
	instaceCreateInfo.ppEnabledExtensionNames = extensions.data();
	instaceCreateInfo.pApplicationInfo = &appInfo;

#ifdef _DEBUG
	instaceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	instaceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#endif // DEBUG

	checkResult(
		vkCreateInstance(&instaceCreateInfo, nullptr, &instance),
		"failed to create instance!"
	);
}

void VulkanBase::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	window = glfwCreateWindow(windowWidth(), windowHeight(), appTitle(), nullptr, nullptr);

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

bool VulkanBase::checkExtensionSupport()
{
	// 拡張情報の取得
	vector<VkExtensionProperties> props;
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	props.resize(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

	for (const auto& e : extensions)
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
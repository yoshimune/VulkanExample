#include "VulkanBase.h"

using namespace std;

VulkanBase::VulkanBase()
	:window(nullptr)
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
}

void VulkanBase::initInstance()
{
	// validationレイヤーをチェック&有効化
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not avilable!");
	}

	vector<const char*> extensions;
	// アプリケーション情報を初期化
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appTitle();
	appInfo.pEngineName = appTitle();
	appInfo.apiVersion = VK_API_VERSION_1_1;
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	
	// 拡張情報の取得
	vector<VkExtensionProperties> props;
	{
		uint32_t count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		props.resize(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

		for (const auto& v : props)
		{
			extensions.push_back(v.extensionName);
		}
	}
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
	for (const char* layerName : getValidationLayers()) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			std::cerr << layerProperties.layerName << std::endl;

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
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

	// App �I��
	glfwTerminate();
}

void VulkanBase::initialize()
{
	initWindow();
	initInstance();
}

void VulkanBase::initInstance()
{
	// validation���C���[���`�F�b�N&�L����
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not avilable!");
	}

	// �g�����̃T�|�[�g�󋵊m�F
	if (!checkExtensionSupport()) {
		throw std::runtime_error("extensions requested, but not avilable!");
	}

	// �A�v���P�[�V��������������
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
	// �L���ȃ��C���[���X�g���擾
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// �K�v�ȃ��C���[���X�g(validationLayers)��
	// �L���ȃ��C���[���X�g(availableLayers)���r���A
	// �K�v�ȃ��C���[���X�g���S�đ��݂��邩���`�F�b�N
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
	// �g�����̎擾
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
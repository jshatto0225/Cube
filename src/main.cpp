#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifndef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication{
	public:
		void run(){
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}
	private:
		GLFWwindow* window;

		VkInstance instance;

		void initWindow(){
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		}

		void initVulkan(){
			createInstance();
		}

		void createInstance(){
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Cube";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;

			createInfo.enabledLayerCount = 0;

			VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

			if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
				throw std::runtime_error("Failed to create instance");
			}

			if(enableValidationLayers && !checkValidationLayerSupport()){
				throw std::runtime_error("Validation layers requested but not available");
			}

			if(enableValidationLayers){
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();
			} else {
				createInfo.enabledLayerCount = 0;
			}

			auto extensions = getRequiredExtensions();
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();
		}

		std::vector<const char*> getRequiredExtensions(){
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

			if(enableValidationLayers){
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
			return extensions;
		}

		bool checkValidationLayerSupport(){
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for(const char* layerName : validationLayers){
				bool layerFound = false;

				for(const auto& layerProperties : availableLayers){
					if(strcmp(layerName, layerProperties.layerName) == 0){
						layerFound = true;
						break;
					}
				}
				if(!layerFound){
					return false;
				}
			}

			return true;
		}

		void mainLoop(){
			while(!glfwWindowShouldClose(window)){
				glfwPollEvents();
			}
		}

		void cleanup(){
			vkDestroyInstance(instance, nullptr);

			glfwDestroyWindow(window);

			glfwTerminate();
		}
};

int main(){
	HelloTriangleApplication app;

	try {
		app.run();
	} catch (const std::exception& e){
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

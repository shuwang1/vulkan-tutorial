
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <cstring>
#include <optional>
#include <assert.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL  debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData ){

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	if ( messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ){
		std::cerr << messageSeverity << std::endl;
	}

	return VK_FALSE;

}



int main(){
	VkResult result;
        VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO,
                NULL,                       // const void       *pNext
                "Vulkan FFT",               // const char       *pApplicationName
                VK_MAKE_VERSION( 1, 0, 0 ), // uint32_t          applicationVersion
                "Vulkan Template",          // const char       *pEngineName
                VK_MAKE_VERSION( 1, 0, 0 ), // uint32_t          engineVersion
                VK_API_VERSION_1_0          // uint32_t          apiVersion
        };

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		NULL,
		0,
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		debugCallback,
		nullptr // Optional
	};
	VkInstanceCreateInfo instanceCreateInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                &debugUtilsMessengerCreateInfo, //const void*               pNext
                0,                              //VkInstanceCreateFlags     flags
                &appInfo,                       //const VkApplicationInfo  *pApplicationInfo
                0,                              //uint32_t                  enabledLayerCount
                NULL,                           //const char * const       *ppEnabledLayerNames
                0,                              //uint32_t                  enabledExtensionCount
                NULL                            //const char * const       *ppEnabledExtensionNames
        };

	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	        NULL, //pNext
                0,    //VkDeviceQueueCreateFlags  flags;
                0,    //uint32_t                  queueFamilyIndex;
                1,    //uint32_t                  queueCount
                &queuePriority 
	};

	VkPhysicalDeviceFeatures deviceFeatures;
	VkDeviceCreateInfo deviceCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        NULL,
		0,                //VkDeviceCreateFlags             flags;
		1,                //uint32_t                        queueCreateInfoCount;
		&queueCreateInfo, //const VkDeviceQueueCreateInfo*  pQueueCreateInfos;
		0,
        NULL,
		0,
		NULL,
                &deviceFeatures	
	};

	glfwInit();

	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	GLFWwindow* window = glfwCreateWindow( 800, 600, "Vulkan Window", nullptr, nullptr );

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );
	std::cout << glfwExtensionCount << " glfwExtensions supported\n";
    for( int ii=0; ii < glfwExtensionCount; ii++  ){

		    std::cout << '\t' << glfwExtensions[ii] << '\n';

	}

	
	uint32_t extensionCount = 0;
	result = vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );
        assert( result == VK_SUCCESS );
        std::vector<VkExtensionProperties> extensions( extensionCount);
	if( 0 == extensionCount ){
		throw std::runtime_error( "failed to find any extension" );
	}else{
		vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, extensions.data() );
		std::cout << extensionCount << " InstanceExtensions supported\n";
		for ( const auto& extension: extensions ){
			std::cout << '\t' << extension.extensionName << '\n';
		}
	}

	uint32_t layerCount = 0;
	result = vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
	assert( result == VK_SUCCESS );
	std::vector<VkLayerProperties> availableLayers(layerCount);
	const std::vector<const char*> validationLayers ={ "VK_LAYER_KHRONOS_validation" };
	if( 0 == layerCount ){
		throw std::runtime_error( "failed to find layer" );
	}else{
		result = vkEnumerateInstanceLayerProperties( &layerCount, availableLayers.data());
		assert( result == VK_SUCCESS );
        std::cout << layerCount << " layers supported from vkEnumerateInstanceLayerProperties\n";
        for( const char* layerName: validationLayers ){

         	       for( const auto& layerProperties: availableLayers ){
                       
         		       if ( strcmp( layerName, layerProperties.layerName ) == 0 ){
         			       std::cout << '\t' << layerName << "*\n";
         		       }else{
                           std::cout << '\t' <<  layerProperties.layerName << std::endl;
                       }

         	       }
        }
	}


#ifdef NODEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

    /************************************************************************
	 * appInfo, debugUtilsMessengerCreateInfo ->
	 * instanceCreateInfo -> instance -> physicalDevice -> 
	 * queueCreateInfo -> 
	 * deviceCreateInfo -> device
	 *
	 * vkCreateInstance( &instanceCreateInfo, nullptr, &instance )
       	 * vkCreateDevice( physicalDevice, &deviceCreateInfo, nullptr, &device ) 
	 ***********************************************************************/

	std::vector<const char*> glExtensions( glfwExtensions, glfwExtensions + glfwExtensionCount );

	if( enableValidationLayers ){
//#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME    ("VK_EXT_debug_utils")
		glExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
glExtensions.pop_back();
glExtensions.pop_back();
		instanceCreateInfo.enabledLayerCount   = static_cast<uint32_t>( validationLayers.size() );
		instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		instanceCreateInfo.enabledExtensionCount   = static_cast<uint32_t>( glExtensions.size() );
		instanceCreateInfo.ppEnabledExtensionNames = glExtensions.data();
        //deviceCreateInfo.enabledLayerCount   = static_cast<uint32_t>( validationLayers.size() );
        //deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        //deviceCreateInfo.enabledExtensionCount   = static_cast<uint32_t>( glExtensions.size() );
        //deviceCreateInfo.ppEnabledExtensionNames = glExtensions.data();
	}

	std::cout << glExtensions.size() << " glfwExtensions supported\n";
        for( int ii=0; ii < glExtensions.size(); ii ++ ){ 
		std::cout << '\t' << glExtensions.data()[ii] << std::endl;
	}

	VkInstance instance;
	if( vkCreateInstance( &instanceCreateInfo, nullptr, &instance ) != VK_SUCCESS ){
		throw std::runtime_error("vkCreateInstanceInfo failed!");
	}

	uint32_t deviceCount = 0;
	result = vkEnumeratePhysicalDevices( instance, &deviceCount, nullptr );
        assert( result == VK_SUCCESS );
	std::vector<VkPhysicalDevice> devices(deviceCount); 
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties deviceProperties;

	if ( 0 == deviceCount ){
		throw std::runtime_error( "failed to find GPUs with Vulkan support!" );
	}else{
		result = vkEnumeratePhysicalDevices( instance, &deviceCount, devices.data() );
                assert( result == VK_SUCCESS );
		std::optional<uint32_t> deviceIdx;
		for(int ii=0; ii<deviceCount; ii++){
			vkGetPhysicalDeviceProperties(devices[ii], &deviceProperties);
			vkGetPhysicalDeviceFeatures(devices[ii], &deviceFeatures);

			std::cout << '\t' << deviceProperties.deviceName << std::endl;

			switch( deviceProperties.deviceType ){
				case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                                        std::cout << '\t' << "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU" << std::endl;
					break;
				case VK_PHYSICAL_DEVICE_TYPE_CPU:
					std::cout << '\t' << "VK_PHYSICAL_DEVICE_TYPE_CPU" << std::endl;
					break;
				default:
					std::cout << '\t' << deviceProperties.deviceType << std::endl;
			}

			std::cout << '\t' << deviceProperties.limits.maxImageDimension1D <<
				'\t' << deviceProperties.limits.maxImageDimension2D <<
				'\t' << deviceProperties.limits.maxImageDimension3D << std::endl;

			std::cout << '\t' << deviceFeatures.geometryShader << std::endl;
		}

		physicalDevice = devices[0];
	}
	
	
	vkGetPhysicalDeviceProperties( physicalDevice, &deviceProperties );
	vkGetPhysicalDeviceFeatures( physicalDevice, &deviceFeatures );

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyCount, nullptr );
	std::vector<VkQueueFamilyProperties> queueFamilies( queueFamilyCount );
        std::optional<uint32_t> queueFamilyIdx;
	if( 0 == queueFamilyCount ){
		throw std::runtime_error( "failed to find physical device queue family\n" );
	}else{
		vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, &queueFamilyCount, queueFamilies.data() );
		std::cout << queueFamilyCount << " queueFamilies supported\n";
		for(int ii=0; ii<queueFamilyCount; ii++ ){
			std::cout << queueFamilies[ii].queueFlags << '\t' << queueFamilies.data() << std::endl;
			if( queueFamilies[ii].queueFlags & VK_QUEUE_GRAPHICS_BIT ){
				queueFamilyIdx = ii;
			}
		}

		std::cout << "queueCreateInfo.queueFamilyIndex = " << queueFamilyIdx.value() << std::endl;
	        queueCreateInfo.queueFamilyIndex = queueFamilyIdx.value();
	        queueCreateInfo.queueCount       = 1;
	}


    VkDevice device{}; 
	if( vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS ){

		throw std::runtime_error( "failed to create a logical device!" );
	}

	VkQueue graphicsQueue;
	vkGetDeviceQueue( device, queueFamilyIdx.value(), 0, &graphicsQueue );


        glm::mat4 matrix;
	glm::vec4 vec;

	auto test = matrix * vec;

	while( !glfwWindowShouldClose( window )  ){
		glfwPollEvents();
	}


	glfwDestroyWindow( window );

	glfwTerminate();



	return 0;

}

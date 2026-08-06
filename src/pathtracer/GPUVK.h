#include <vulkan/vulkan.h>
#include <vector>
#include <string.h>
#include <iostream>
#include "Image.h"
#include "OutputBuffer.h"
#include "InputBuffer.h"

const int WORKGROUP_SIZE = 8;

// Used for validating return values of Vulkan API calls.
#define VK_CHECK_RESULT(f) 																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        printf("Fatal : VkResult is %d in %s at line %d\n", res,  __FILE__, __LINE__); \
    }																									\
}

class GPUVK {
public:
    GPUVK(int width, int height, int size);
    ~GPUVK();

    void Run(Image& image, unsigned char* data, int size);

private:

    void CreateInstance();
    void FindPhysicalDevice();
    uint32_t GetComputeQueueFamilyIndex();
    void CreateDevice();
    void CreateDescriptorSetLayout();
    void CreateDescriptorSet();
    uint32_t* ReadFile(uint32_t& length, const char* filename);
    void CreateComputePipeline();
    void CreateCommandBuffer();

    int m_Width = 0;
    int m_Height = 0;
    VkInstance instance;
    VkDebugReportCallbackEXT debugReportCallback;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkShaderModule computeShaderModule;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
    VkDescriptorSetLayout descriptorSetLayout;
    VkQueue queue;
    uint32_t queueFamilyIndex;
    OutputBuffer outputBuffer;
    InputBuffer inputBuffer;

};

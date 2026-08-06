#pragma once
#include <vulkan/vulkan.h>
#include "Image.h"

class OutputBuffer {
public:

    void Create(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size);
    void ReadData(Image& image, VkCommandPool pool, VkQueue queue);
    void Destroy();

    VkBuffer Buffer;
    VkDeviceMemory Memory;
    VkDeviceSize Size;

private:
    VkBuffer m_StagingBuffer;
    VkDeviceMemory m_StagingMemory; 
    VkDevice m_Device;

};
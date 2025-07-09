/** Copyright (C) 2021 Robert Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "Graphics_Systems/Vulkan/VulkanHeaders.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/SDL/Window.h"

#include <vector>
#include <set>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_vulkan.h>

namespace enigma {

extern unsigned sdl_window_flags;

void init_sdl_window_bridge_attributes() {
  sdl_window_flags |= SDL_WINDOW_VULKAN;
}

VkInstance instance;

void EnableDrawing(void*) {
  unsigned int extensionCount = 0;
  SDL_Vulkan_GetInstanceExtensions(windowHandle, &extensionCount, nullptr);
  std::vector<const char *> extensionNames(extensionCount);
  SDL_Vulkan_GetInstanceExtensions(windowHandle, &extensionCount, extensionNames.data());

  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "window_name";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo instanceCreateInfo = {};
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pApplicationInfo = &appInfo;
  instanceCreateInfo.enabledLayerCount = 0;
  instanceCreateInfo.ppEnabledLayerNames = NULL;
  instanceCreateInfo.enabledExtensionCount = extensionNames.size();
  instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();
  
  vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

  // Get number of available physical devices, needs to be at least 1
  unsigned int physical_device_count(0);
  vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
  if (physical_device_count <= 0) {
    std::cout << "No physical devices found\n";
    return;
  }

  // Now get the devices and use the first GPU
  std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
  vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());
  VkPhysicalDevice physicalDevice = physical_devices[0];

  // Find the number queues this device supports, we want to make sure that we have a queue that supports graphics commands
  unsigned int family_queue_count(0);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &family_queue_count, nullptr);
  if (family_queue_count == 0) {
    std::cout << "device has no family of queues associated with it\n";
    return;
  }

  // Extract the properties of all the queue families
  std::vector<VkQueueFamilyProperties> queue_properties(family_queue_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &family_queue_count, queue_properties.data());

  // Make sure the family of commands contains an option to issue graphical commands.
  unsigned int queue_node_index = -1;
  for (unsigned int i = 0; i < family_queue_count; i++)
  {
    if (queue_properties[i].queueCount > 0 && queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queue_node_index = i;
      break;
    }
  }

  if (queue_node_index < 0) {
    std::cout << "Unable to find a queue command family that accepts graphics commands\n";
    return;
  }

  // Get the number of available extensions for our graphics card
  uint32_t device_property_count(0);
  if (vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &device_property_count, NULL) != VK_SUCCESS) {
    std::cout << "Unable to acquire device extension property count\n";
    return;
  }
  std::cout << "\nfound " << device_property_count << " device extensions\n";

  // Acquire their actual names
  std::vector<VkExtensionProperties> device_properties(device_property_count);
  if (vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &device_property_count, device_properties.data()) != VK_SUCCESS) {
    std::cout << "Unable to acquire device extension property names\n";
    return;
  }

  // Match names against requested extension
  std::vector<const char*> device_property_names;
  const std::set<std::string>& required_extension_names = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  int count = 0;
  for (const auto& ext_property : device_properties)
  {
      std::cout << count << ": " << ext_property.extensionName << "\n";
      auto it = required_extension_names.find(std::string(ext_property.extensionName));
      if (it != required_extension_names.end())
      {
          device_property_names.emplace_back(ext_property.extensionName);
      }
      count++;
  }

  // Warn if not all required extensions were found
  if (required_extension_names.size() != device_property_names.size())
  {
      std::cout << "not all required device extensions are supported!\n";
      return;
  }

  // Create queue information structure used by device based on the previously fetched queue information from the physical device
  // We create one command processing queue for graphics
  VkDeviceQueueCreateInfo queue_create_info;
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueFamilyIndex = queue_node_index;
  queue_create_info.queueCount = 1;
  std::vector<float> queue_prio = { 1.0f };
  queue_create_info.pQueuePriorities = queue_prio.data();
  queue_create_info.pNext = NULL;
  queue_create_info.flags = NULL;

  // Device creation information
  VkDeviceCreateInfo create_info;
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = 1;
  create_info.pQueueCreateInfos = &queue_create_info;
  create_info.ppEnabledLayerNames = NULL;
  create_info.enabledLayerCount = 0;
  create_info.ppEnabledExtensionNames = device_property_names.data();
  create_info.enabledExtensionCount = static_cast<uint32_t>(device_property_names.size());
  create_info.pNext = NULL;
  create_info.pEnabledFeatures = NULL;
  create_info.flags = NULL;

  // Finally we're ready to create a new device
  VkDevice device;
  VkResult res = vkCreateDevice(physicalDevice, &create_info, nullptr, &device);
  if (res != VK_SUCCESS) {
    std::cout << "failed to create logical device!\n";
    return;
  }

  VkSurfaceKHR presentation_surface;
  if (!SDL_Vulkan_CreateSurface(windowHandle, instance, &presentation_surface)) {
    std::cout << "Unable to create Vulkan compatible surface using SDL\n";
    return;
  }

  // Make sure the surface is compatible with the queue family and gpu
  VkBool32 supported = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queue_node_index, presentation_surface, &supported);
  if (!supported) {
    std::cout << "Surface is not supported by physical device!\n";
    return;
  }

  // Get properties of surface, necessary for creation of swap-chain
  VkSurfaceCapabilitiesKHR surface_properties;
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, presentation_surface, &surface_properties) != VK_SUCCESS) {
    std::cout << "unable to acquire surface capabilities\n";
    return;
  }

  // Get the image presentation mode (synced, immediate etc.)
  VkPresentModeKHR presentation_mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
  uint32_t mode_count(0);
  if(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, presentation_surface, &mode_count, NULL) != VK_SUCCESS) {
    std::cout << "unable to query present mode count for physical device\n";
    return;
  }

  std::vector<VkPresentModeKHR> available_modes(mode_count);
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, presentation_surface, &mode_count, available_modes.data()) != VK_SUCCESS) {
    std::cout << "unable to query the various present modes for physical device\n";
    return;
  }

  supported = false;
  for (auto& mode : available_modes) {
    if (mode == presentation_mode)
      supported = true;
  }
  
  if (!supported) {
    std::cout << "unable to obtain preferred display mode, fallback to FIFO\n";
    presentation_mode = VK_PRESENT_MODE_FIFO_KHR;
  }

  // Get other swap chain related features
  unsigned int swap_image_count = surface_properties.minImageCount + 1;
  if (swap_image_count > surface_properties.maxImageCount)
    swap_image_count = surface_properties.minImageCount;

  // Size of the images
  VkExtent2D swap_image_extent = surface_properties.currentExtent;

  // Get the transform, falls back on current transform when transform is not supported
  VkSurfaceTransformFlagBitsKHR transform = surface_properties.currentTransform;
  if (surface_properties.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

  // Get swapchain image format
  VkSurfaceFormatKHR image_format;
  image_format.format = VK_FORMAT_B8G8R8A8_SRGB;
  image_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

  // Old swap chain
  VkSwapchainKHR swap_chain = NULL;

  // Populate swapchain creation info
  VkSwapchainCreateInfoKHR swap_info;
  swap_info.pNext = nullptr;
  swap_info.flags = 0;
  swap_info.surface = presentation_surface;
  swap_info.minImageCount = swap_image_count;
  swap_info.imageFormat = image_format.format;
  swap_info.imageColorSpace = image_format.colorSpace;
  swap_info.imageExtent = swap_image_extent;
  swap_info.imageArrayLayers = 1;
  swap_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swap_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swap_info.queueFamilyIndexCount = 0;
  swap_info.pQueueFamilyIndices = nullptr;
  swap_info.preTransform = transform;
  swap_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swap_info.presentMode = presentation_mode;
  swap_info.clipped = true;
  swap_info.oldSwapchain = NULL;
  swap_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

  // Create new one
  if (vkCreateSwapchainKHR(device, &swap_info, nullptr, &swap_chain) != VK_SUCCESS) {
    std::cout << "unable to create swap chain\n";
    return;
  }

  // Get image handles from swap chain
  unsigned int image_count(0);
  res = vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr);
  if (res != VK_SUCCESS) {
    std::cout << "unable to get number of images in swap chain\n";
    return;
  }

  std::vector<VkImage> chain_images(image_count);
  if (vkGetSwapchainImagesKHR(device, swap_chain, &image_count, chain_images.data()) != VK_SUCCESS) {
    std::cout << "unable to get image handles from swap chain\n";
    return;
  }

  // Fetch the queue we want to submit the actual commands to
  VkQueue graphics_queue;
  vkGetDeviceQueue(device, queue_node_index, 0, &graphics_queue);
}

void DisableDrawing(void*) {

}

void ScreenRefresh() {

}

} // namespace enigma

namespace enigma_user {

void set_synchronization(bool enable) {

}

} // namespace enigma_user

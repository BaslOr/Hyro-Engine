#include "pch.h"
#include "Platform/Vulkan/VulkanAPI.h"

#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanCommandPool.h"
#include "Platform/Vulkan/VulkanDescriptorPool.h"
#include <backends/imgui_impl_vulkan.h>


namespace Hyro {

	VulkanAPI::VulkanAPI()
	{
		m_CommandBuffers = VulkanCommandPool::AllocateCommandBuffers(VulkanContext::Get().GetMaxFramesInFlight());

		CreateSyncObjects();
	}

	VulkanAPI::~VulkanAPI()
	{
		vkQueueWaitIdle(VulkanDevice::GetPresentationQueue());
        vkDeviceWaitIdle(VulkanDevice::GetVkDevice());

		vkFreeCommandBuffers(
			VulkanDevice::GetVkDevice(),
			VulkanCommandPool::GetVkCommandPool(),
			1,
			m_CommandBuffers.data()
		);

        for (size_t i = 0; i < VulkanContext::Get().GetMaxFramesInFlight(); i++) {
            vkDestroySemaphore(VulkanDevice::GetVkDevice(), m_RenderFinishedSemaphores[i], g_VulkanAllocationCallback);
            vkDestroySemaphore(VulkanDevice::GetVkDevice(), m_ImageAvailableSemaphores[i], g_VulkanAllocationCallback);
            vkDestroyFence(VulkanDevice::GetVkDevice(), m_InFlightFences[i], g_VulkanAllocationCallback);
        }

        VulkanDescriptorPool::Destroy();
        VulkanCommandPool::Destroy();
	}
    
    void VulkanAPI::BeginRenderPass()
    {
        VulkanContext& context = VulkanContext::Get();
        VkDevice device = VulkanDevice::GetVkDevice();
        uint32_t currentFrame = VulkanContext::Get().GetCurrentFrameIndex();

        vkWaitForFences(device, 1, &m_InFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(
            device,
            context.GetSwapchain(),
            UINT64_MAX,
            m_ImageAvailableSemaphores[currentFrame],
            VK_NULL_HANDLE,
            &m_ImageIndex
        );


        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            HYRO_LOG_CORE_ERROR("Swapchain out of date!");
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            HYRO_LOG_CORE_ERROR("Failed to acquire swap chain image!");
        }

        //Only reset the fence if we are submitting work
        vkResetFences(device, 1, &m_InFlightFences[currentFrame]);

        vkResetCommandBuffer(m_CommandBuffers[currentFrame], 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(m_CommandBuffers[currentFrame], &beginInfo);

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a}};
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = context.GetRenderPass();
        renderPassInfo.framebuffer = context.GetVkFramebuffer(m_ImageIndex);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = context.GetSwapchainExtent();
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanAPI::EndRenderPass()
    {
        VulkanContext& context = VulkanContext::Get();
        VkDevice device = VulkanDevice::GetVkDevice();
        uint32_t currentFrame = context.GetCurrentFrameIndex();

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffers[currentFrame]);


        vkCmdEndRenderPass(m_CommandBuffers[currentFrame]);
        vkEndCommandBuffer(m_CommandBuffers[currentFrame]);

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[currentFrame];
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffers[currentFrame];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores[currentFrame];

        vkQueueSubmit(VulkanDevice::GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[currentFrame]);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[currentFrame];
        presentInfo.swapchainCount = 1;

        VkSwapchainKHR swapchain = context.GetSwapchain();
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &m_ImageIndex;

        VkResult result = vkQueuePresentKHR(VulkanDevice::GetPresentationQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            HYRO_LOG_CORE_ERROR("Swapchain out of date");
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            HYRO_LOG_CORE_ERROR("Failed to present swap chain image!");
        }

        VulkanContext::Get().IncreaseImageIndex();
    }

    void VulkanAPI::Submit(Ref<VertexArray> vertexArray, Ref<Material> material, uint32_t count)
    {
        uint32_t currentFrame = VulkanContext::Get().GetCurrentFrameIndex();

        vertexArray->Bind(m_CommandBuffers[currentFrame]);

        material->Bind(m_CommandBuffers[currentFrame]);

        vkCmdDrawIndexed(m_CommandBuffers[currentFrame], count, 1, 0, 0, 0);
    }

    void VulkanAPI::SubmitCubemap(Ref<VertexArray> vertexArray, Ref<Material> material, Ref<Cubemap> cubemap)
    {
        Submit(vertexArray, material, 36);
    }

    void VulkanAPI::SetClearColor(const glm::vec4& color)
	{
        m_ClearColor = color;
	}

	void VulkanAPI::CreateSyncObjects()
	{
        uint32_t maxFramesInFlight = VulkanContext::Get().GetMaxFramesInFlight();

        m_ImageAvailableSemaphores.resize(maxFramesInFlight);
        m_RenderFinishedSemaphores.resize(maxFramesInFlight);
        m_InFlightFences.resize(maxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < maxFramesInFlight; i++) {
            if (vkCreateSemaphore(VulkanDevice::GetVkDevice(), &semaphoreInfo, g_VulkanAllocationCallback, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(VulkanDevice::GetVkDevice(), &semaphoreInfo, g_VulkanAllocationCallback, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(VulkanDevice::GetVkDevice(), &fenceInfo, g_VulkanAllocationCallback, &m_InFlightFences[i]) != VK_SUCCESS) {

                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
	}

}

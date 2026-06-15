#include "pch.h"
#include "Platform/Vulkan/VulkanAPI.h"

#include "Platform/Vulkan/VulkanContext.h"

#include <backends/imgui_impl_vulkan.h>


namespace Hyro {

	VulkanAPI::VulkanAPI(const GraphicsPipelineSettings& settings)
	{
		m_Pipeline = CreateScope<VulkanGraphicsPipeline>(settings);
		VulkanCommandPool::Init();
		m_CommandBuffers = VulkanCommandPool::AllocateCommandBuffers(m_MaxFramesInFlight);

		CreateSyncObjects();
	}

	VulkanAPI::~VulkanAPI()
	{
		vkQueueWaitIdle(VulkanDevice::GetPresentationQueue());

		vkFreeCommandBuffers(
			VulkanDevice::GetVkDevice(),
			VulkanCommandPool::GetVkCommandPool(),
			1,
			m_CommandBuffers.data()
		);

        for (size_t i = 0; i < m_MaxFramesInFlight; i++) {
            vkDestroySemaphore(VulkanDevice::GetVkDevice(), m_RenderFinishedSemaphores[i], g_VulkanAllocationCallback);
            vkDestroySemaphore(VulkanDevice::GetVkDevice(), m_ImageAvailableSemaphores[i], g_VulkanAllocationCallback);
            vkDestroyFence(VulkanDevice::GetVkDevice(), m_InFlightFences[i], g_VulkanAllocationCallback);
        }

	}

    void VulkanAPI::BeginScene()
    {
        VulkanContext& context = VulkanContext::Get();
        VkDevice device = VulkanDevice::GetVkDevice();

        vkWaitForFences(device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(
            device,
            context.GetSwapchain(),
            UINT64_MAX,
            m_ImageAvailableSemaphores[m_CurrentFrame],
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
        vkResetFences(device, 1, &m_InFlightFences[m_CurrentFrame]);

        vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(m_CommandBuffers[m_CurrentFrame], &beginInfo);

        VkClearValue clearColor;
        clearColor = { {m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a} };

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = context.GetRenderPass();
        renderPassInfo.framebuffer = context.GetVkFramebuffer(m_ImageIndex);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = context.GetSwapchainExtent();
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->GetVkPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(context.GetSwapchainExtent().width);
        viewport.height = static_cast<float>(context.GetSwapchainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_CommandBuffers[m_CurrentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = context.GetSwapchainExtent();
        vkCmdSetScissor(m_CommandBuffers[m_CurrentFrame], 0, 1, &scissor);
    }

    void VulkanAPI::EndScene()
    {
        VulkanContext& context = VulkanContext::Get();
        VkDevice device = VulkanDevice::GetVkDevice();

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffers[m_CurrentFrame]);


        vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrame]);
        vkEndCommandBuffer(m_CommandBuffers[m_CurrentFrame]);

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[m_CurrentFrame];
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrame];

        vkQueueSubmit(VulkanDevice::GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrame];
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

        m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;
    }

    void VulkanAPI::Submit(Ref<VertexArray> vertexArray, Ref<Shader> shader, uint32_t count)
    {
        vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->GetVkPipeline());

        vertexArray->Bind(m_CommandBuffers[m_CurrentFrame]);

        vkCmdDraw(m_CommandBuffers[m_CurrentFrame], count, 1, 0, 0);
    }

    void VulkanAPI::SetClearColor(const glm::vec4& color)
	{
        m_ClearColor = color;
	}

	void VulkanAPI::CreateSyncObjects()
	{
        m_ImageAvailableSemaphores.resize(m_MaxFramesInFlight);
        m_RenderFinishedSemaphores.resize(m_MaxFramesInFlight);
        m_InFlightFences.resize(m_MaxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < m_MaxFramesInFlight; i++) {
            if (vkCreateSemaphore(VulkanDevice::GetVkDevice(), &semaphoreInfo, g_VulkanAllocationCallback, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(VulkanDevice::GetVkDevice(), &semaphoreInfo, g_VulkanAllocationCallback, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(VulkanDevice::GetVkDevice(), &fenceInfo, g_VulkanAllocationCallback, &m_InFlightFences[i]) != VK_SUCCESS) {

                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
	}

}

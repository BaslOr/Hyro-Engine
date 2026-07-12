#include "pch.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"

#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Hyro/Core/Core.h"


#include <fstream>
#include <cstdlib>


#ifdef HYRO_PLATFORM_WINDOWS
    #include <windows.h>
#endif


namespace Hyro {
    uint32_t FormatSize(VkFormat format);



	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const std::string& vertexPath, const std::string& fragmentPath)
	{
		CreatePipeline(vertexPath, fragmentPath);

		HYRO_LOG_CORE_INFO("Graphics pipeline created successfully");
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		VkDevice device = VulkanDevice::GetVkDevice();
		vkDestroyPipeline(device, m_Pipeline, g_VulkanAllocationCallback);
		vkDestroyPipelineLayout(device, m_PipelineLayout, g_VulkanAllocationCallback);
		vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, g_VulkanAllocationCallback);
	}

    void VulkanGraphicsPipeline::CreatePipeline(const std::string& vertexPath, const std::string& fragmentPath)
    {

        VkDevice device = VulkanDevice::GetVkDevice();

		auto vertexCode = ReadFile(vertexPath);
		auto fragmentCode = ReadFile(fragmentPath);

        auto vertexSpirV = ShaderCompiler::CompileToSpirv(vertexPath, ShaderStage::Vertex);
        auto fragmentSpirV = ShaderCompiler::CompileToSpirv(fragmentPath, ShaderStage::Fragment);


        VkShaderModule vertShaderModule = CreateShaderModule(vertexSpirV);
        VkShaderModule fragShaderModule = CreateShaderModule(fragmentSpirV);
        
        auto vertexReflection = ShaderReflection::FillReflectionData(ShaderStage::Vertex, vertexSpirV);
        auto fragmentReflection = ShaderReflection::FillReflectionData(ShaderStage::Fragment, fragmentSpirV);

        VkPipelineShaderStageCreateInfo vertexStageInfo{};
        vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexStageInfo.module = vertShaderModule;
        vertexStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragmentStageInfo{};
        fragmentStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentStageInfo.module = fragShaderModule;
        fragmentStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertexStageInfo, fragmentStageInfo };


        auto bindingAndAttributes = GetBindingAndAttributes(vertexReflection);
        VkVertexInputBindingDescription bindingDescription = bindingAndAttributes.first;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions = bindingAndAttributes.second;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        CreateDescriptorSetLayout();

        VkPushConstantRange pushConstants{};
        pushConstants.size = sizeof(PushConstants);
        pushConstants.offset = 0;
        pushConstants.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
     

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstants;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
            HYRO_LOG_CORE_ERROR("Failed to create Pipeline Layout!");
        }


        VkRenderPass renderPass = VulkanContext::Get().GetRenderPass();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_PipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, g_VulkanAllocationCallback, &m_Pipeline) != VK_SUCCESS) {
            HYRO_LOG_CORE_FATAL("Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device, vertShaderModule, g_VulkanAllocationCallback);
        vkDestroyShaderModule(device, fragShaderModule, g_VulkanAllocationCallback);
    }

    void VulkanGraphicsPipeline::CreateDescriptorSetLayout()
    {
        //Should later be reflected from shader

		std::array<VkDescriptorSetLayoutBinding, 2> bindings{};
        bindings[0].binding = 0;
        bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        bindings[0].pImmutableSamplers = nullptr;

        bindings[1].binding = 1;
        bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        bindings[1].descriptorCount = 16;
        bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        bindings[1].pImmutableSamplers = nullptr;
        

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(VulkanDevice::GetVkDevice(), &layoutInfo, g_VulkanAllocationCallback, &m_DescriptorSetLayout) != VK_SUCCESS) {
            HYRO_LOG_CORE_ERROR("Failed to create Descriptor Set Layout!");
        }
    }

    std::pair<VkVertexInputBindingDescription, std::vector<VkVertexInputAttributeDescription>>
        VulkanGraphicsPipeline::GetBindingAndAttributes(const ShaderReflectionData& reflection)
    {
        VkVertexInputBindingDescription binding_description = {};
        binding_description.binding = 0;
        binding_description.stride = 0; 
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
        std::vector<VkVertexInputAttributeDescription> attribute_descriptions;
        attribute_descriptions.reserve(reflection.VertexInputs.size());
        for (size_t i_var = 0; i_var < reflection.VertexInputs.size(); ++i_var) {
            const SpvReflectInterfaceVariable& refl_var = *(reflection.VertexInputs[i_var]);
            // ignore built-in variables
            if (refl_var.decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) {
                continue;
            }
            VkVertexInputAttributeDescription attr_desc{};
            attr_desc.location = refl_var.location;
            attr_desc.binding = binding_description.binding;
            attr_desc.format = static_cast<VkFormat>(refl_var.format);
            attr_desc.offset = 0;  // final offset computed below after sorting.
            attribute_descriptions.push_back(attr_desc);
        }
        // Sort attributes by location
        std::sort(std::begin(attribute_descriptions), std::end(attribute_descriptions),
            [](const VkVertexInputAttributeDescription& a, const VkVertexInputAttributeDescription& b) {
                return a.location < b.location;
            });
        // Compute final offsets of each attribute, and total vertex stride.
        for (auto& attribute : attribute_descriptions) {
            uint32_t format_size = FormatSize(attribute.format);
            attribute.offset = binding_description.stride;
            binding_description.stride += format_size;
        }

        return std::make_pair(binding_description, attribute_descriptions);

    }

    std::string VulkanGraphicsPipeline::ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::ate);
		if (!file.is_open()) {
			HYRO_LOG_CORE_ERROR("Failed to read Shader: " + filepath);
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		
		file.close();

        std::string output(buffer.data());

		return output;
	}

	VkShaderModule VulkanGraphicsPipeline::CreateShaderModule(const std::vector<uint32_t>& code)
	{
		VkShaderModuleCreateInfo moduleInfo{};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize = code.size() * sizeof(uint32_t);
		moduleInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkDevice device = VulkanDevice::GetVkDevice();
		VkShaderModule shaderModule;
		VkCheck(vkCreateShaderModule(device, &moduleInfo, g_VulkanAllocationCallback, &shaderModule));

		return shaderModule;
	}






    //ShaderCompiler
    //Should be abstracted in some kind of Platform class
    std::filesystem::path ShaderCompiler::GetGlslcPath() {
        const char* sdkPath = std::getenv("VULKAN_SDK");
        if (!sdkPath) {
            HYRO_LOG_CORE_ERROR("VULKAN_SDK Umgebungsvariable nicht gesetzt.");
        }
        if (g_CurrentPlatform == PlatformType::Windows)
            return std::filesystem::path(sdkPath) / "Bin" / "glslc.exe";
        else
            return std::filesystem::path(sdkPath) / "bin" / "glslc";
    }

    const char* ShaderCompiler::StageToFlag(ShaderStage stage) {
        switch (stage) {
        case ShaderStage::Vertex:   return "vertex";
        case ShaderStage::Fragment: return "fragment";
        case ShaderStage::Compute:  return "compute";
        }
        HYRO_LOG_CORE_ERROR("Unbekannter ShaderStage.");
    }

    std::vector<uint32_t> ShaderCompiler::CompileToSpirv(const std::filesystem::path& sourcePath, ShaderStage stage) {
        auto tempDir = std::filesystem::temp_directory_path();
        auto outputPath = tempDir / (sourcePath.filename().string() + ".spv");
        auto logPath = tempDir / (sourcePath.filename().string() + ".log");

        std::string cmd = "\"" + GetGlslcPath().string() + "\""
            + " -fshader-stage=" + StageToFlag(stage)
            + " --target-env=vulkan1.3"
            + " \"" + sourcePath.string() + "\""
            + " -o \"" + outputPath.string() + "\"";

#ifdef HYRO_PLATFORM_WINDOWS
        // Log-Datei als Ziel für stdout+stderr des Kindprozesses
        SECURITY_ATTRIBUTES sa{};
        sa.nLength = sizeof(sa);
        sa.bInheritHandle = TRUE;

        HANDLE logHandle = CreateFileA(
            logPath.string().c_str(), GENERIC_WRITE, FILE_SHARE_READ,
            &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

        STARTUPINFOA si{};
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdOutput = logHandle;
        si.hStdError = logHandle;

        PROCESS_INFORMATION pi{};

        // CREATE_NO_WINDOW verhindert das Konsolenfenster-Flackern
        BOOL ok = CreateProcessA(
            nullptr, cmd.data(), nullptr, nullptr, TRUE,
            CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);

        CloseHandle(logHandle);

        if (!ok) {
            HYRO_LOG_CORE_ERROR("glslc konnte nicht gestartet werden.");
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode = 0;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (exitCode != 0) {
            std::ifstream logFile(logPath);
            std::string log((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
            std::filesystem::remove(logPath);
            HYRO_LOG_CORE_ERROR("Shader-Kompilierung fehlgeschlagen (" + sourcePath.string() + "):\n" + log);
        }
#else
        int result = std::system((cmd + " > \"" + logPath.string() + "\" 2>&1").c_str());
        if (result != 0) {
            std::ifstream logFile(logPath);
            std::string log((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
            std::filesystem::remove(logPath);
            HYRO_LOG_CORE_ERROR("Shader-Kompilierung fehlgeschlagen (" + sourcePath.string() + "):\n" + log);
        }
#endif
        std::filesystem::remove(logPath);

        // .spv-Datei einlesen
        std::ifstream file(outputPath, std::ios::binary | std::ios::ate);
        if (!file) {
            HYRO_LOG_CORE_ERROR("SPIR-V Ausgabedatei konnte nicht gelesen werden: " + outputPath.string());
        }
        size_t size = static_cast<size_t>(file.tellg());
        std::vector<uint32_t> spirv(size / sizeof(uint32_t));
        file.seekg(0);
        file.read(reinterpret_cast<char*>(spirv.data()), size);
        file.close();

        std::filesystem::remove(outputPath);
        return spirv;
    }

    ShaderReflectionData ShaderReflection::FillReflectionData(ShaderStage stage, const std::vector<uint32_t>& bytes)
    {
        ShaderReflectionData data{};

        SpvReflectShaderModule module;
        int result = spvReflectCreateShaderModule(sizeof(uint32_t) * bytes.size(), bytes.data(), &module);
        if (result != SPV_REFLECT_RESULT_SUCCESS) {
            HYRO_LOG_CORE_ERROR("Failed to reflect Shader Module!");
        }


        //Query VertexAttributes
        if (stage == ShaderStage::Vertex) {
            uint32_t count = 0;
            result = spvReflectEnumerateInputVariables(&module, &count, nullptr);
            if (result != SPV_REFLECT_RESULT_SUCCESS)
                HYRO_LOG_CORE_ERROR("Failed to enumerate Vertex Inputs!");

            //Theoratically count is allways higher than 0
            if (count != 0) {
                std::vector<SpvReflectInterfaceVariable*> vertexInputs(count);
                result = spvReflectEnumerateInputVariables(&module, &count, vertexInputs.data());
                if (result != SPV_REFLECT_RESULT_SUCCESS)
                    HYRO_LOG_CORE_ERROR("Failed to enumerate Vertex Inputs!");

                data.VertexInputs = std::move(vertexInputs);
            }
        }

        uint32_t count = 0;
        result = spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
        if (result != SPV_REFLECT_RESULT_SUCCESS)
            HYRO_LOG_CORE_ERROR("Failed to enumerate Descriptor Sets!");

        if (count != 0) {
            std::vector<SpvReflectDescriptorSet*> sets(count);
            result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
            if (result != SPV_REFLECT_RESULT_SUCCESS)
                HYRO_LOG_CORE_ERROR("Failed to enumerate Descriptor Sets!");

            data.DescriptorSets = std::move(sets);
        }

        result = spvReflectEnumeratePushConstantBlocks(&module, &count, nullptr);
        if (result != SPV_REFLECT_RESULT_SUCCESS)
            HYRO_LOG_CORE_ERROR("Failed to enumerate Push Constants!");
        if (count != 0) {
            std::vector<SpvReflectBlockVariable*> pushConstants(count);
            result = spvReflectEnumeratePushConstantBlocks(&module, &count, pushConstants.data());
            if (result != SPV_REFLECT_RESULT_SUCCESS)
                HYRO_LOG_CORE_ERROR("Failed to enumerate Push Constants!");

            data.PushConstants = std::move(pushConstants);
        }


        return data;
    }

    static uint32_t FormatSize(VkFormat format) {
        uint32_t result = 0;
        switch (format) {
        case VK_FORMAT_UNDEFINED:
            result = 0;
            break;
        case VK_FORMAT_R4G4_UNORM_PACK8:
            result = 1;
            break;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_R8_UNORM:
            result = 1;
            break;
        case VK_FORMAT_R8_SNORM:
            result = 1;
            break;
        case VK_FORMAT_R8_USCALED:
            result = 1;
            break;
        case VK_FORMAT_R8_SSCALED:
            result = 1;
            break;
        case VK_FORMAT_R8_UINT:
            result = 1;
            break;
        case VK_FORMAT_R8_SINT:
            result = 1;
            break;
        case VK_FORMAT_R8_SRGB:
            result = 1;
            break;
        case VK_FORMAT_R8G8_UNORM:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SNORM:
            result = 2;
            break;
        case VK_FORMAT_R8G8_USCALED:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SSCALED:
            result = 2;
            break;
        case VK_FORMAT_R8G8_UINT:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SINT:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SRGB:
            result = 2;
            break;
        case VK_FORMAT_R8G8B8_UNORM:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SNORM:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_USCALED:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SSCALED:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_UINT:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SINT:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SRGB:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_UNORM:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SNORM:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_USCALED:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SSCALED:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_UINT:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SINT:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SRGB:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8A8_UNORM:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SNORM:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_USCALED:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SSCALED:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_UINT:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SINT:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SRGB:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_UNORM:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SNORM:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_USCALED:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SSCALED:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_UINT:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SINT:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SRGB:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_R16_UNORM:
            result = 2;
            break;
        case VK_FORMAT_R16_SNORM:
            result = 2;
            break;
        case VK_FORMAT_R16_USCALED:
            result = 2;
            break;
        case VK_FORMAT_R16_SSCALED:
            result = 2;
            break;
        case VK_FORMAT_R16_UINT:
            result = 2;
            break;
        case VK_FORMAT_R16_SINT:
            result = 2;
            break;
        case VK_FORMAT_R16_SFLOAT:
            result = 2;
            break;
        case VK_FORMAT_R16G16_UNORM:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SNORM:
            result = 4;
            break;
        case VK_FORMAT_R16G16_USCALED:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SSCALED:
            result = 4;
            break;
        case VK_FORMAT_R16G16_UINT:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SINT:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SFLOAT:
            result = 4;
            break;
        case VK_FORMAT_R16G16B16_UNORM:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SNORM:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_USCALED:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SSCALED:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_UINT:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SINT:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SFLOAT:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16A16_UNORM:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SNORM:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_USCALED:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SSCALED:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_UINT:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SINT:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            result = 8;
            break;
        case VK_FORMAT_R32_UINT:
            result = 4;
            break;
        case VK_FORMAT_R32_SINT:
            result = 4;
            break;
        case VK_FORMAT_R32_SFLOAT:
            result = 4;
            break;
        case VK_FORMAT_R32G32_UINT:
            result = 8;
            break;
        case VK_FORMAT_R32G32_SINT:
            result = 8;
            break;
        case VK_FORMAT_R32G32_SFLOAT:
            result = 8;
            break;
        case VK_FORMAT_R32G32B32_UINT:
            result = 12;
            break;
        case VK_FORMAT_R32G32B32_SINT:
            result = 12;
            break;
        case VK_FORMAT_R32G32B32_SFLOAT:
            result = 12;
            break;
        case VK_FORMAT_R32G32B32A32_UINT:
            result = 16;
            break;
        case VK_FORMAT_R32G32B32A32_SINT:
            result = 16;
            break;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            result = 16;
            break;
        case VK_FORMAT_R64_UINT:
            result = 8;
            break;
        case VK_FORMAT_R64_SINT:
            result = 8;
            break;
        case VK_FORMAT_R64_SFLOAT:
            result = 8;
            break;
        case VK_FORMAT_R64G64_UINT:
            result = 16;
            break;
        case VK_FORMAT_R64G64_SINT:
            result = 16;
            break;
        case VK_FORMAT_R64G64_SFLOAT:
            result = 16;
            break;
        case VK_FORMAT_R64G64B64_UINT:
            result = 24;
            break;
        case VK_FORMAT_R64G64B64_SINT:
            result = 24;
            break;
        case VK_FORMAT_R64G64B64_SFLOAT:
            result = 24;
            break;
        case VK_FORMAT_R64G64B64A64_UINT:
            result = 32;
            break;
        case VK_FORMAT_R64G64B64A64_SINT:
            result = 32;
            break;
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            result = 32;
            break;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            result = 4;
            break;

        default:
            break;
        }
        return result;
    }

}

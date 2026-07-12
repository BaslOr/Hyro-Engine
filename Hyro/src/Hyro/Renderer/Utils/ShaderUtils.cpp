#include "pch.h"
#include "Hyro/Renderer/Utils/ShaderUtils.h"

#ifdef HYRO_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace Hyro {

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////Shader Reflection/////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    VertexLayout ShaderReflection::GetVertexLayout(const std::vector<uint32_t>& vertexSpirV)
    {
        SpvReflectShaderModule module;
        int result = spvReflectCreateShaderModule(sizeof(uint32_t) * vertexSpirV.size(), vertexSpirV.data(), &module);
        if (result != SPV_REFLECT_RESULT_SUCCESS) {
            HYRO_LOG_CORE_ERROR("Failed to reflect Shader Module!");
        }

        uint32_t count = 0;
        result = spvReflectEnumerateInputVariables(&module, &count, nullptr);
        if (result != SPV_REFLECT_RESULT_SUCCESS)
            HYRO_LOG_CORE_ERROR("Failed to enumerate Vertex Inputs!");


        VertexLayout layout{};
        if (count != 0) {
            std::vector<SpvReflectInterfaceVariable*> vertexInputs(count);
            result = spvReflectEnumerateInputVariables(&module, &count, vertexInputs.data());
            if (result != SPV_REFLECT_RESULT_SUCCESS)
                HYRO_LOG_CORE_ERROR("Failed to enumerate Vertex Inputs!");


            std::vector<std::pair<VertexAttributeType, uint32_t>> buffer;
            for (const auto& attribute : vertexInputs) {
                VertexAttributeType type = ReflectTypeToGLType(attribute->format);
                buffer.push_back(std::make_pair(type, attribute->location));
            }

            std::sort(std::begin(buffer), std::end(buffer),
                [](const std::pair<VertexAttributeType, uint32_t>& a, const std::pair<VertexAttributeType, uint32_t>& b) {
                    return a.second < b.second;
                });

            VertexLayout layout{};
            for (const auto& [type, location] : buffer) {
                layout.Push(type);
            }

            return layout;
        }
    }

    ShaderReflectionData ShaderReflection::FillReflectionData(const std::array<std::vector<uint32_t>, 2>& spirVs)
    {
        ShaderReflectionData data{};

        //Create Modules
        std::array<SpvReflectShaderModule, 2> modules;
        for (size_t i = 0; i < spirVs.size(); ++i) {
            int result = spvReflectCreateShaderModule(sizeof(uint32_t) * spirVs[i].size(), spirVs[i].data(), &modules[i]);
            if (result != SPV_REFLECT_RESULT_SUCCESS) {
                HYRO_LOG_CORE_ERROR("Failed to reflect Shader Module!");
            }
        }

        for (size_t i = 0; i < modules.size(); ++i) {

            //Query Descriptors
            uint32_t count = 0;
            spvReflectEnumerateDescriptorSets(&modules[i], &count, nullptr);
            std::vector<SpvReflectDescriptorSet*> sets(count);
            spvReflectEnumerateDescriptorSets(&modules[i], &count, sets.data());

            for (auto set : sets) {

                for (size_t j = 0; j < set->binding_count; ++j) {
                    auto binding = set->bindings[j];
                    ShaderReflectionData::Descriptor descriptorInfo{};

                    descriptorInfo.Binding = binding->binding;
                    descriptorInfo.Set = set->set;
                    descriptorInfo.Count = binding->count;
                    descriptorInfo.Stage = i == 0 ? ShaderStage::Vertex : ShaderStage::Fragment;
                    descriptorInfo.Type = SpvDescriptorTypeToHyroType(binding->descriptor_type);

                    data.Descriptors.push_back(descriptorInfo);
                }
            }

            //Query Push Constants
            spvReflectEnumeratePushConstantBlocks(&modules[i], &count, nullptr);
            std::vector<SpvReflectBlockVariable*> pushConstants(count);
            spvReflectEnumeratePushConstantBlocks(&modules[i], &count, pushConstants.data());

            for (auto block : pushConstants) {
                ShaderReflectionData::PushConstant pushConstant{};

                pushConstant.Offset = block->offset;
                pushConstant.Size = block->size;
                pushConstant.Stage = i == 0 ? ShaderStage::Vertex : ShaderStage::Fragment;

                data.PushConstants.push_back(pushConstant);
            }
        }


        //Destroy Shader modules
        for (auto module : modules) {
            spvReflectDestroyShaderModule(&module);
        }

        return data;
    }

    VertexAttributeType ShaderReflection::ReflectTypeToGLType(SpvReflectFormat format)
    {
        switch (format)
        {
        case SPV_REFLECT_FORMAT_UNDEFINED:
            break;
        case SPV_REFLECT_FORMAT_R16_UINT:
            break;
        case SPV_REFLECT_FORMAT_R16_SINT:
            break;
        case SPV_REFLECT_FORMAT_R16_SFLOAT:
            break;
        case SPV_REFLECT_FORMAT_R16G16_UINT:
            break;
        case SPV_REFLECT_FORMAT_R16G16_SINT:
            break;
        case SPV_REFLECT_FORMAT_R16G16_SFLOAT:
            break;
        case SPV_REFLECT_FORMAT_R16G16B16_UINT:
            break;
        case SPV_REFLECT_FORMAT_R16G16B16_SINT:
            break;
        case SPV_REFLECT_FORMAT_R16G16B16_SFLOAT:
            break;
        case SPV_REFLECT_FORMAT_R16G16B16A16_UINT:
            break;
        case SPV_REFLECT_FORMAT_R16G16B16A16_SINT:
            break;
        case SPV_REFLECT_FORMAT_R16G16B16A16_SFLOAT:
            break;
        case SPV_REFLECT_FORMAT_R32_UINT:
            break;
        case SPV_REFLECT_FORMAT_R32_SINT:
            break;
        case SPV_REFLECT_FORMAT_R32_SFLOAT:
            return VertexAttributeType::FLOAT;
        case SPV_REFLECT_FORMAT_R32G32_UINT:
            break;
        case SPV_REFLECT_FORMAT_R32G32_SINT:
            break;
        case SPV_REFLECT_FORMAT_R32G32_SFLOAT:
            return VertexAttributeType::FLOAT2;
        case SPV_REFLECT_FORMAT_R32G32B32_UINT:
            break;
        case SPV_REFLECT_FORMAT_R32G32B32_SINT:
            break;
        case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:
            return VertexAttributeType::FLOAT3;
        case SPV_REFLECT_FORMAT_R32G32B32A32_UINT:
            break;
        case SPV_REFLECT_FORMAT_R32G32B32A32_SINT:
            break;
        case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:
            return VertexAttributeType::FLOAT4;
        case SPV_REFLECT_FORMAT_R64_UINT:
            break;
        case SPV_REFLECT_FORMAT_R64_SINT:
            break;
        case SPV_REFLECT_FORMAT_R64_SFLOAT:
            break;
        case SPV_REFLECT_FORMAT_R64G64_UINT:
            break;
        case SPV_REFLECT_FORMAT_R64G64_SINT:
            break;
        case SPV_REFLECT_FORMAT_R64G64_SFLOAT:
            break;
        case SPV_REFLECT_FORMAT_R64G64B64_UINT:
            break;
        case SPV_REFLECT_FORMAT_R64G64B64_SINT:
            break;
        case SPV_REFLECT_FORMAT_R64G64B64_SFLOAT:
            break;
        case SPV_REFLECT_FORMAT_R64G64B64A64_UINT:
            break;
        case SPV_REFLECT_FORMAT_R64G64B64A64_SINT:
            break;
        case SPV_REFLECT_FORMAT_R64G64B64A64_SFLOAT:
            break;
        }

        HYRO_LOG_CORE_ERROR("Failed to convert spv reflect format to Vertex Attribute Type");
        return VertexAttributeType::NONE;
    }

    DescriptorType ShaderReflection::SpvDescriptorTypeToHyroType(SpvReflectDescriptorType type)
    {
        switch (type)
        {
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            return DescriptorType::UNIFORM_BUFFER;

        case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
            return DescriptorType::SAMPLER;

        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
            break;

        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
            break;
        }

        HYRO_LOG_CORE_ERROR("Unkown conversion from Spv Reflect Desciptor type to Hyro Descriptor Type!");
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////Shader Compiler///////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
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

    const char* ShaderCompiler::StageToFlag(ShaderStage stage) {
        switch (stage) {
        case ShaderStage::Vertex:   return "vertex";
        case ShaderStage::Fragment: return "fragment";
        case ShaderStage::Compute:  return "compute";
        }
        HYRO_LOG_CORE_ERROR("Unbekannter ShaderStage.");
    }

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

}

#include "pch.h"
#include "Hyro/Renderer/Utils/ShaderUtils.h"

#ifdef HYRO_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace Hyro {

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////Shader Reflection/////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
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

import shutil
import os
import platform
import subprocess
import re
import urllib.request


PREMAKE_VERSION = "5.0.0-beta8"


def DownloadNecessaryDependencies():
    __DownloadPremake()
    __CheckCMake()
    __CheckVulkan()


def __DownloadPremake():

    premake_dir = os.path.join(
        "../vendor",
        "Premake"
    )

    if platform.system() == "Windows":
        premake_binary = os.path.join(
            premake_dir,
            "premake5.exe"
        )
    else:
        premake_binary = os.path.join(
            premake_dir,
            "premake5"
        )

    if os.path.exists(premake_binary):
        print("\tPremake is installed")
        return


    print("\tPremake is not installed")
    print("\tDownloading Premake...")


    os.makedirs(
        premake_dir,
        exist_ok=True
    )


    system = platform.system()


    if system == "Windows":

        url = (
            f"https://github.com/premake/premake-core/releases/"
            f"download/v{PREMAKE_VERSION}/"
            f"premake-{PREMAKE_VERSION}-windows.zip"
        )

        archive = os.path.join(
            premake_dir,
            "premake.zip"
        )

        urllib.request.urlretrieve(
            url,
            archive
        )

        import zipfile

        with zipfile.ZipFile(archive, "r") as zip_ref:
            zip_ref.extractall(premake_dir)

        os.remove(archive)


    elif system == "Linux":

        url = (
            f"https://github.com/premake/premake-core/releases/"
            f"download/v{PREMAKE_VERSION}/"
            f"premake-{PREMAKE_VERSION}-linux.tar.gz"
        )

        archive = os.path.join(
            premake_dir,
            "premake.tar.gz"
        )

        urllib.request.urlretrieve(
            url,
            archive
        )

        subprocess.run(
            [
                "tar",
                "-xzf",
                archive,
                "-C",
                premake_dir
            ],
            check=True
        )

        os.remove(archive)


    else:
        print("[FATAL ERROR]: Unsupported OS for Premake")
        return


    if os.path.exists(premake_binary):
        print("\tPremake installed successfully")
    else:
        print("[FATAL ERROR]: Premake installation failed")



def __CheckCMake():

    path = shutil.which("cmake")

    if path:
        version = __GetVersion(["cmake", "--version"])

        if version:
            print(f"\tCMake is installed (version {version})")

            major, minor = map(
                int,
                version.split(".")[:2]
            )

            if (major == 3 and minor >= 25) or major > 3:
                return

            else:
                print("\tCMake version too old")

        else:
            print("\tCould not detect CMake version")

    else:
        print("\tCMake is not installed")


    __Install("CMake")



def __CheckVulkan():

    vulkan_path = os.environ.get("VULKAN_SDK")
    vulkan_info = shutil.which("vulkaninfo")


    if vulkan_path and vulkan_info:

        version = __ExtractVulkanVersion(
            vulkan_path
        )

        if version:
            print(
                f"\tVulkanSDK is installed (version {version})"
            )

        else:
            print(
                "\tVulkanSDK installed but version unknown"
            )

    else:

        print(
            "\tVulkanSDK is not installed"
        )

        __Install("VulkanSDK")



def __GetVersion(command):

    try:

        result = subprocess.run(
            command,
            capture_output=True,
            text=True,
            check=True
        )


        match = re.search(
            r"\d+\.\d+\.\d+",
            result.stdout
        )

        if match:
            return match.group()


    except subprocess.CalledProcessError:

        return None


    return None



def __ExtractVulkanVersion(path):

    match = re.search(
        r"\d+\.\d+\.\d+",
        path
    )

    if match:
        return match.group()

    return None



def __Install(package):

    print(
        f"Would you like to download {package}? y or n: ",
        end=""
    )

    answer = input()


    if answer.lower() != "y":

        print(
            "Failed to finish setup"
        )

        return False


    print(
        f"Downloading {package}"
    )


    windowsPackageName = {
        "CMake": "Kitware.CMake",
        "VulkanSDK": "KhronosGroup.VulkanSDK"
    }


    linuxPackageName = {
        "CMake": "cmake",
        "VulkanSDK": "vulkan-tools"
    }


    system = platform.system()


    try:


        if system == "Windows":

            if shutil.which("winget"):

                subprocess.run(
                    [
                        "winget",
                        "install",
                        windowsPackageName[package]
                    ],
                    check=True
                )

            else:

                print(
                    "[FATAL ERROR]: winget not available"
                )

                return False



        elif system == "Linux":

            if shutil.which("apt"):

                subprocess.run(
                    [
                        "sudo",
                        "apt",
                        "update"
                    ],
                    check=True
                )


                subprocess.run(
                    [
                        "sudo",
                        "apt",
                        "install",
                        "-y",
                        linuxPackageName[package]
                    ],
                    check=True
                )


            else:

                print(
                    "[FATAL ERROR]: apt not available"
                )

                return False



        else:

            print(
                f"[FATAL ERROR]: Unsupported OS {system}"
            )

            return False



    except subprocess.CalledProcessError:

        print(
            f"[FATAL ERROR]: Failed installing {package}"
        )

        return False



    print(
        f"Finished downloading {package}"
    )

    return True
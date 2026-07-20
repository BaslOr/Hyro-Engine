import os
import subprocess
import shutil
from pathlib import Path
import platform

from Utils import DownloadNecessaryDependencies

debugLog = False

def __DebugLog(message):
    if debugLog:
        print("[Debug] " + str(message))



#Check Platform
if platform.system() == "Darwin":
    print("MacOS is not supported yet")
    print("You need to download and compile external dependencies yourself")
    exit(-1)

#Download External Dependencies that are not supplied by submodules
#For instance Vulkan, CMake, ...
print("Downloading external dependencies")

#Check CMake, Vulkan versions
DownloadNecessaryDependencies()


print("Finished downloading external dependencies\n")


print("Compiling CMake projects")


print("\tCompiling assimp")

os.chdir("./../") # Move into Project root
os.chdir("Hyro/vendor/assimp") # Move into assimp directory

__DebugLog(os.getcwd())


root = os.getcwd()
build_dir = os.path.join(root, "build")
libDir = os.path.join(root, "lib")


if not os.path.exists(build_dir):

    # Set options + compile for Debug + Release
    subprocess.run([
        "cmake",
        "-S", root,
        "-B", build_dir,
        "-DBUILD_SHARED_LIBS=OFF",
        "-DASSIMP_BUILD_ZLIB=ON"
    ], check=True)
    
    subprocess.run([
        "cmake",
        "--build",
        build_dir,
        "--config",
        "Release"
    ], check=True)
    
    subprocess.run([
        "cmake",
        "--build",
        build_dir,
        "--config",
        "Debug"
    ], check=True)


    print("\tFinished Compiling assimp\n")


    print("\tRelocating assimp binaries\n")


    # Create lib directory if it does not exist
    if not os.path.exists(libDir):
        os.makedirs(libDir)
        __DebugLog("Created directory: " + os.path.abspath(libDir))


    buildPath = Path(build_dir)
    libPath = Path(libDir)


    __DebugLog("Searching for libraries in:")
    __DebugLog(buildPath.resolve())


    # Find all .lib files inside build directory
    libs = list(buildPath.rglob("*.lib"))


    if len(libs) == 0:
        print("ERROR: No .lib files found!")
        exit(1)


    __DebugLog("Found libraries:")

    for lib in libs:
        __DebugLog(lib.resolve())


        # Copy only assimp libraries
    for lib in libs:

        # Ignore cmake generated helper libraries etc.
        assimpInName = "assimp" in lib.name.lower()
        zlibInName = "zlib" in lib.name.lower()

        if not assimpInName and not zlibInName:
            __DebugLog("Ignoring: " + lib.name)
            continue


        # Keep Debug and Release separated
        if "Debug" in str(lib):
            targetDir = libPath / "Debug"

        elif "Release" in str(lib):
            targetDir = libPath / "Release"

        else:
            __DebugLog("Could not determine configuration for: " + lib.name)
            continue


        if not targetDir.exists():
            targetDir.mkdir(parents=True)
            __DebugLog("Created directory: " + str(targetDir))


        # Rename library to a stable name
        # Example:
        # assimp-vc143-mt.lib  -> assimp.lib
        # assimp-vc143-mtd.lib -> assimp.lib
        targetName = ""
        if assimpInName:
            targetName = "assimp.lib"
        elif zlibInName:
            targetName = "zlib.lib"
        target = targetDir / targetName


        shutil.copy2(lib, target)


        __DebugLog("Copied and renamed:" + str(lib) + " -> " + str(target))
else: 
    print("\tAssimp was already compiled")




print("Finished compiling CMake projects\n")
print("Finished set up successfully\n")

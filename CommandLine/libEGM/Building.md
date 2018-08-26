vcpkg install pugixml:x64-windows rapidjson:x64-windows yaml-cpp:x64-windows
cmake -G "Visual Studio 15 2017 Win64" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
mkdir build
cd build/
cmake --build . --target install --config MinSizeRel
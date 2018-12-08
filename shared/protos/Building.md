vcpkg install protobuf:x64-windows-static grpc:x64-windows-static
mkdir build
cd build/
cmake -G "Visual Studio 15 2017 Win64" -Dprotobuf_generate_IMPORT_DIRS="C:\vcpkg\installed\x64-windows-static\include" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
cmake --build . --target install --config MinSizeRel
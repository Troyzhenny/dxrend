# This is a renderer i'm building to learn graphics programming

### Tech stack
- DirectX 11
- Win32
- Cmake
- C++

To get started building the project, ensure you have cmake installed and optionally Ninja.

### Quick note (for people using Zed)
If you're using zed as well (this probably also pertain to vscode users, But Visual Studio IDE users can skip this step)
you'll need to add this line in CMakeLists.txt
This just helps clangd know where all your files and folders are.
```
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

<br>

### Get Started (project root)
```
# For Debug
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build-debug

# For Release
cmake -B build-release -DCMAKE_BUILD_TYPE=release -G Ninja
cmake --build build-release
```

### If you're not using ninja
```
# For Debug
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug

# For Release
cmake -B build-release -DCMAKE_BUILD_TYPE=release
cmake --build build-release
```

### Copy compile commands file
inside your build folder you should see a file named "compile_commands.json"
copy it to project root.

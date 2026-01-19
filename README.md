# This is a rendering engine i'm building to learn graphics programming
to start with this project you need to ensure you have cmake installed and also Ninja (optional) to build the project.

### Quick note (for people using Zed)
If you're using zed as well (this probably also pertain to vscode users, But Visual Studio IDE users can skip this step)
you'll need to add this line in CMakeLists.txt
This just helps clangd know where all your files and folders are.
```
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```


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

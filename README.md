# A rendering project i created to learn graphics programming
to start you need to ensure you have cmake installed and also Ninja because this is what is used to build the project 
(you don't have to use ninja you can use the default provided in Visual Studio but 
that is not what i'm using - i'm also using Zed Editor not Visual Studio).

### Quick note (for people using Zed)
If you're using zed as well (this probably also pertain to vscode users, not sure)
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

### Copy commands file (if you're using Visual Studio you can skip this)
inside your build folder you should see a file named "compile_commands.json"
copy it to project root.

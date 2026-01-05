# A rendering project i created to learn graphics programming
to start you need to ensure you have cmake installed and also Ninja because this is what is used to build the project 
(you don't have to use ninja you can use the default provided in Visual Studio but 
that is not what i'm using - i'm also using Zed Editor not Visual Studio).

### Quick note (for people using Zed)
If you're using zed as well (this probably also pertain to vscode users, not sure)
you'll need to add this line in CMakeLists.txt
```
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```
This just helps clangd know where all your files and folders are.


### Get Started (project root)
```
  mkdir build
  cd build
  cmake -G Ninja ..
  ninja

```

### Copy commads file (if you're using Visual Studio you can skip this)
inside your build folder you should see a file named "compile_commands.json"
run this command to copy it to project root ``` copy compile_commands.json .. ```

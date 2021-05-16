# Plug-in Base Library

## Summary

Combine libraries to get a plug-in base. This library is still experimental and to be used in order to try things out.

### Dependency map

```
plugin-base
+-- aligned-allocator
+-- audio-modules
|   +-- fx-collection
|   |   +-- dsp-tool-box
|   +-- param-tool-box 
+-- googletest
+-- kompositum
+-- vst3sdk
```

## Building the project

Execute the following commands on cli.

```
git clone https://www.github.com/hansen-audio/plugin-base.git
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../plugin-base
cmake --build .
```

### CMake Generators

CMake geneartors for all platforms.

* Linux: ```cmake -DCMAKE_BUILD_TYPE=[Debug|Release] ...```

* macOS: ```cmake -GXcode ...```

* Windows 10: ```cmake -G"Visual Studio 16 2019" -A x64 ...```

## License

Copyright 2021 Hansen Audio

Licensed under the MIT: https://mit-license.org/

# GEN

## Intro

GEN is standing for `Graphic Engine`. This is my practice project after learning the courses from [Learn Open GL](https://learnopengl.com/).


## Usages

```shell
mkdir build && cd build
conan install .. -s build_type=Debug
cmake ..
start GEN.sln
```

This project use [conan](https://docs.conan.io/en/latest/using_packages/workflows.html#multi-configuration) as the c++ project manager.
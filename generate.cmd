if not exist "build" mkdir build
cd build
conan install .. -s build_type=Debug
cmake ..
start GEN.sln

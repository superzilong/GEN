

find_package(glfw3 REQUIRED CONFIG)
find_package(glad REQUIRED CONFIG)
find_package(stb REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)

FILE(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
FILE(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(GEN ${sources} ${headers})

list(APPEND allFiles ${sources} ${headers})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${allFiles})

target_link_libraries(GEN glad::glad)
target_link_libraries(GEN glfw::glfw)
target_link_libraries(GEN glm::glm)
target_link_libraries(GEN stb::stb)
target_link_libraries(GEN spdlog::spdlog)
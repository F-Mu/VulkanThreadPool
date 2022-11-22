cmake_minimum_required(VERSION 3.23)
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(Vulkan_Thread_Pool)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-autolink")

include_directories(/opt/homebrew/include ./include)
# glfw的库目录路径
link_directories(/opt/homebrew/Cellar/glfw/3.3.8/lib)
# Vulkan的库目录
include_directories(/Users/muwu/VulkanSDK/1.3.224.1/macOS/include/)
link_directories(/Users/muwu/VulkanSDK/1.3.224.1/macOS/lib)

#file(GLOP_RECURSE SRC_DIR_LIST "*.cpp" "*.c")

set(librariesList
        GLFW
        vulkan
        )

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
#add_executable(VulkanLearning main.cpp crp_swap_chain.cpp crp_swap_chain.hpp crp_model.hpp crp_model.cpp crp_game_obejct.hpp crp_renderer.hpp crp_renderer.cpp simple_render_system.hpp simple_render_system.cpp crp_camera.hpp crp_camera.cpp keyboard_movement_controller.cpp keyboard_movement_controller.hpp crp_utils.cpp crp_utils.hpp crp_game_object.cpp crp_buffer.cpp crp_buffer.hpp crp_frame_info.cpp crp_frame_info.hpp crp_descriptors.cpp crp_descriptors.hpp crp_device.cpp crp_window.cpp first_app.cpp crp_pipeline.cpp)
target_link_libraries(${PROJECT_NAME} ${librariesList})

find_program(GLSL_VALIDATOR glslangValidator HINTS
        ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}
        /usr/bin
        /usr/local/bin
        ${VULKAN_SDK_PATH}/Bin
        ${VULKAN_SDK_PATH}/Bin32
        $ENV{VULKAN_SDK}/Bin/
        $ENV{VULKAN_SDK}/Bin32/
        )

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/shaders/*.frag"
        "${PROJECT_SOURCE_DIR}/shaders/*.vert"
        )

foreach (GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)
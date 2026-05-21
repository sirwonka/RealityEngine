cmake_minimum_required(VERSION 3.15)
project(RealityEngine VERSION 1.0)

# Forcing modern memory standards
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Fetch GLFW (Hardware API)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Fetch Dear ImGui (Docking Branch for Enterprise Layouts)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

# Fetch GLM (Linear Algebra Matrix Mathematics)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Locate the system's raw OpenGL APIs
find_package(OpenGL REQUIRED)

# ImGui requires explicit source compilation
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Compile executable with ImGui sources injected
add_executable(RealityEngine main.cpp ${IMGUI_SOURCES})

# Include headers for RealityEngine (including GLM)
target_include_directories(RealityEngine PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends ${glm_SOURCE_DIR})

# Link the final payload
target_link_libraries(RealityEngine PRIVATE glfw OpenGL::GL)

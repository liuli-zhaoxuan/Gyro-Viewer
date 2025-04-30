set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(trackballCamera main.cpp)

target_link_libraries(trackballCamera PRIVATE OpenGL::GL GLEW::GLEW glfw glm::glm)
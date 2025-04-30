# 设置 C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找需要的包
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# 生成可执行文件
add_executable(demo main.cpp)

# 链接OpenGL, GLEW, GLFW
target_link_libraries(demo PRIVATE OpenGL::GL GLEW::GLEW glfw)

# 添加glm头文件
target_link_libraries(demo PRIVATE glm::glm)   #  再加这一行！
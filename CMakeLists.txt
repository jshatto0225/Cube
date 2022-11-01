cmake_minimum_required(VERSION 3.12)
project(Cube VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} 
	PUBLIC
		glm 
	PRIVATE
		glfw
		${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} 
	PUBLIC
		glm
	PRIVATE
		glfw
		${Vulkan_LIBRARIES}
)

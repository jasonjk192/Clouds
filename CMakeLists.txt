cmake_minimum_required (VERSION 3.21)

project ("Clouds")

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS FALSE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

include(FetchContent)

#-----------------------------
# External libraries from git repos
#-----------------------------
# glfw
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.6)
FetchContent_MakeAvailable(glfw)
#-----------------------------

#-----------------------------
# GLM
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)
add_compile_definitions(GLM_FORCE_LEFT_HANDED)
#-----------------------------

#-----------------------------
# GLAD
add_subdirectory("CMake Subfolders/GLAD")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
#-----------------------------

#-----------------------------
# stb_image
FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG origin/master)
FetchContent_MakeAvailable(stb)
#-----------------------------

#-----------------------------
# assimp
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
set(ASSIMP_BUILD_FBX_IMPORTER FALSE)
set(ASSIMP_BUILD_BLEND_IMPORTER FALSE)
set(ASSIMP_NO_EXPORT TRUE)
set(ASSIMP_BUILD_TESTS FALSE)
FetchContent_Declare(assimp GIT_REPOSITORY https://github.com/assimp/assimp.git GIT_TAG 5.2.0)
FetchContent_MakeAvailable(assimp)
#-----------------------------

#-----------------------------
# Source files
#-----------------------------

set(PROJECT_SOURCES_WITHOUT_MAIN
	""
	)

add_executable(${PROJECT_NAME} src/main.cpp ${PROJECT_SOURCES_WITHOUT_MAIN})
#-----------------------------

#-----------------------------
# Include files
#-----------------------------

set(PROJECT_INCLUDES
	"include;"
	"shaders;"
	"${FETCHCONTENT_BASE_DIR}/stb-src;"
	)
	
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDES})
#-----------------------------

#-----------------------------
# Libraries
#-----------------------------

set(PROJECT_LIBRARIES
	"glfw;"
	"glm;"
	"glad;"
	"assimp;"
  )

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARIES})
cmake_minimum_required(VERSION 3.16)

project(ogldev_tutorials)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

# configure_file(configuration/root_directory.h.in configuration/root_directory.h)
# include_directories(${CMAKE_BINARY_DIR}/configuration)

# external dependencies
include(CPM)
set(BUILD_TESTING OFF)
CPMAddPackage(
	NAME glfw
	GIT_REPOSITORY git@github.com:glfw/glfw.git
	GIT_TAG 3.3.8
)
# assimp
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
CPMAddPackage(
	NAME assimp
	GIT_REPOSITORY git@github.com:assimp/assimp.git
	GIT_TAG v5.3.0
)
# We manually download GLM into our source directory so the user can access
# glm through 'glm/...'. There is no build step since this is a header only
# library.
CPMAddPackage(
	NAME glm
	GIT_REPOSITORY git@github.com:g-truc/glm.git
	GIT_TAG 0.9.9.8
	DOWNLOAD_ONLY True
)
include_directories(${glm_SOURCE_DIR}/)

CPMAddPackage(
	NAME meshoptimizer
	GIT_REPOSITORY git@github.com:zeux/meshoptimizer.git
	GIT_TAG v0.20
)


include_directories(${CMAKE_SOURCE_DIR}/include)

# first create relevant static libraries required for other projects
add_library(GLAD src/glad.c)
set(LIBS ${LIBS} GLAD glfw assimp meshoptimizer)

add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)


add_subdirectory(src/)



# macro(makeLink src dest target)
#   add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
# endmacro()

# set (CHAPTERS
# 	1.getting_started
# 	2.intermediate_opengl)
   
# set(1.getting_started  )

# set(2.intermediate_opengl
# 	31.selecting3d)

#   function(create_project_from_sources chapter demo)
# 	  file(GLOB SOURCE
# 			  "src/${chapter}/${demo}/*.h"
# 			  "src/${chapter}/${demo}/*.cpp"
# 			  "src/${chapter}/${demo}/*.vs"
# 			  "src/${chapter}/${demo}/*.fs"
# 			  "src/${chapter}/${demo}/*.tcs"
# 			  "src/${chapter}/${demo}/*.tes"
# 			  "src/${chapter}/${demo}/*.gs"
# 			  "src/${chapter}/${demo}/*.cs"
# 	  )
# 	  if (demo STREQUAL "")
# 		  SET(replaced "")
# 		  string(REPLACE "/" "_" replaced ${chapter})
# 		  set(NAME ${replaced})
# 	  else()
# 		  set(NAME "${chapter}__${demo}")
# 	  endif()
# 	  add_executable(${NAME} ${SOURCE})
# 	  target_link_libraries(${NAME} ${LIBS})
# 	  if(MSVC)
# 		  target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
# 		  target_link_options(${NAME} PUBLIC /ignore:4099)
# 	  endif(MSVC)
# 	  if(WIN32)
# 		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
# 		  set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/Debug")
# 	  elseif(UNIX AND NOT APPLE)
# 		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
# 	  elseif(APPLE)
# 		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
# 		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${chapter}")
# 		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${chapter}")
# 	  endif(WIN32)
# 	  # copy shader files to build directory
# 	  file(GLOB SHADERS
# 			   "src/${chapter}/${demo}/*.vs"
# 			   # "src/${chapter}/${demo}/*.frag"
# 			   "src/${chapter}/${demo}/*.fs"
# 			   "src/${chapter}/${demo}/*.tcs"
# 			   "src/${chapter}/${demo}/*.tes"
# 			   "src/${chapter}/${demo}/*.gs"
# 			   "src/${chapter}/${demo}/*.cs"
# 	  )
# 	  # copy dlls
# 	  file(GLOB DLLS "dlls/*.dll")
# 	  foreach(SHADER ${SHADERS})
# 		  if(WIN32)
# 			  # configure_file(${SHADER} "test")
# 			  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
# 			  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
# 		  elseif(UNIX AND NOT APPLE)
# 			  file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
# 		  elseif(APPLE)
# 			  # create symbolic link for *.vs *.fs *.gs
# 			  get_filename_component(SHADERNAME ${SHADER} NAME)
# 			  makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${chapter}/${SHADERNAME} ${NAME})
# 		  endif(WIN32)
# 	  endforeach(SHADER)
# 	  # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
# 	  if(MSVC)
# 		  configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
# 	  endif(MSVC)
#   	endfunction()
  
# 	# then create a project file per tutorial
# 	foreach(CHAPTER ${CHAPTERS})
# 		foreach(DEMO ${${CHAPTER}})
# 			create_project_from_sources(${CHAPTER} ${DEMO})
# 		endforeach(DEMO)
# 	endforeach(CHAPTER)

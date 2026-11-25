cmake_minimum_required(VERSION 3.20)
project(Synmux VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(-O3 -flto -DNDEBUG)
  add_link_options(-flto)

  if(WIN32)
    add_link_options(-static -fuse-ld=lld -Wl,-s)
  elseif(APPLE)
    add_link_options(-Wl,-x)
  else()
    add_link_options(-Wl,-s)
  endif()

endif()

add_executable(synmux
  src/main.cpp
  src/core/ToolPaths.cpp
)

target_include_directories(synmux PRIVATE src)

# Symlink to runtime/tools after build
if(WIN32)
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/runtime/tools" NATIVE_TOOLS_SRC)
  add_custom_command(TARGET synmux POST_BUILD
    COMMAND cmd /c if not exist "tools" mklink /J "tools" "${NATIVE_TOOLS_SRC}"
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:synmux>"
  )
else()
  add_custom_command(TARGET synmux POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    "${CMAKE_SOURCE_DIR}/runtime/tools"
    "$<TARGET_FILE_DIR:synmux>/tools"
  )  
endif()
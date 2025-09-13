cmake_minimum_required(VERSION 3.22)
project(qnx-raspi-setup-util VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)
FetchContent_MakeAvailable(ftxui)

set(HEADERS
  config-editor.h
  first-run-utils.h
  setup-utils.h
)
set(SOURCES
  config-editor.cpp
  first-run-utils.cpp
  qnx-raspi-setup-util.cpp
  setup-utils.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

# do not append any suffix as we are targeting QNX
set(CMAKE_EXECUTABLE_SUFFIX ".qnx")

# QNX_PROCESSOR is inherited from the QNX toolchain file
install(TARGETS ${PROJECT_NAME} DESTINATION nto/${QNX_PROCESSOR}/bin)

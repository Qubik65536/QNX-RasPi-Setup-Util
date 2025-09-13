cmake_minimum_required(VERSION 3.22)
project(qnx-raspi-setup-util VERSION 0.1.0 LANGUAGES CXX)

set(SOURCES qnx-raspi-setup-util.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
# do not append any suffix as we are targeting QNX
set(CMAKE_EXECUTABLE_SUFFIX ".qnx")

# QNX_PROCESSOR is inherited from the QNX toolchain file
install(TARGETS ${PROJECT_NAME} DESTINATION nto/${QNX_PROCESSOR}/bin)

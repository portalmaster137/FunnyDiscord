cmake_minimum_required(VERSION 3.28)

project(HelloWorld)

set(CMAKE_CXX_STANDARD 17)

## We have dotenv-cpp and DPP as submodules in include/
## Which we will include in our project

add_subdirectory(include/dotenv-cpp)
add_subdirectory(include/DPP)

## We will include the include/ directory in our project 
## So that we can include the headers from the submodules

include_directories(include)

## We will add the source files to our project

file(GLOB SOURCES "src/*.cpp")

## We will add the executable to our project

add_executable(HelloWorld ${SOURCES})

## We will link the libraries to our project

target_link_libraries(HelloWorld PRIVATE dpp dotenv)
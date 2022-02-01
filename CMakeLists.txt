cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(DDSCodesys)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

message(STATUS "Configuring publisher/subscriber codesys project...")
file(GLOB DDS_DATAEXCHANGE_SOURCES_CXX "src/dataTypeCxx/*.cxx")

include_directories(header header/dataTypeHeader)

add_executable(${PROJECT_NAME}Publisher 
    src/CodesysPublisher.cpp 
    src/PubListener.cpp 
    src/SharedMemory.cpp
    ${DDS_DATAEXCHANGE_SOURCES_CXX})

target_link_libraries(${PROJECT_NAME}Publisher 
    fastrtps 
    fastcdr 
    rt 
    )

add_executable(${PROJECT_NAME}Subscriber 
    src/CodesysSubscriber.cpp 
    src/SubListener.cpp 
    src/SharedMemory.cpp
    ${DDS_DATAEXCHANGE_SOURCES_CXX})
    
target_link_libraries(${PROJECT_NAME}Subscriber 
    fastrtps 
    fastcdr
    rt
    )

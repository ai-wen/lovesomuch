cmake_minimum_required(VERSION 3.0.0)
if (POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif (POLICY CMP0091)

project(dm-core)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE on)

if(NOT CMAKE_BUILD_TYPE)    
    #set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")        
    message(STATUS "===============CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()


if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# 检查是否支持 SSSE3
include(CheckCXXSourceRuns)
set(CMAKE_REQUIRED_FLAGS "-march=native")
check_cxx_source_runs(
    " #include <immintrin.h>
    int main() {
        __m128i a = _mm_setzero_si128();
        __m128i b = _mm_shuffle_epi8(a, a);
        return 0; 
    }" 
HAVE_SSSE3)

if(HAVE_SSSE3)
  message("This machine supports SSSE3.")
  #支持时使用Hyperscan
else()
  message("This machine does not support SSSE3.")
  #RE2
endif()


option(WITH_NLP "baidu lac c++ nlp " OFF)
option(WITH_HYPERSCAN "a high-performance, open source regex matching library from Intel" OFF)

if (WITH_NLP) 
    add_compile_definitions(NLP) 
    #aux_source_directory(src/lac NLP_SRC)
    #set(NLP_INC "${CMAKE_SOURCE_DIR}/src/lac/include")
    #set(NLP_LIB "${CMAKE_SOURCE_DIR}/3rd/fluid_inference_install_dir")
endif()
if (WITH_HYPERSCAN) 
    add_compile_definitions(HYPERSCAN)   
    #aux_source_directory(src/lac NLP_SRC)
endif()


if (WIN32)
    set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin/win/64)
elseif (CMAKE_COMPILER_IS_GNUCC)
    set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin/linux/64)
elseif(APPLE)
    set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin/mac/64)
endif (WIN32)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}/${CMAKE_BUILD_TYPE} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}/${CMAKE_BUILD_TYPE} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}/${CMAKE_BUILD_TYPE} )

#[[
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR} )

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUT_DIR}/${CMAKE_BUILD_TYPE} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUT_DIR}/${CMAKE_BUILD_TYPE} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUT_DIR}/${CMAKE_BUILD_TYPE} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
]]


add_executable(test src/main.cpp ${NLP_SRC})
set_property(TARGET test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # -MT/-MTd 

if(CMAKE_GENERATOR MATCHES "MinGW")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    #链接gcc-g++静态库
    target_link_libraries(test libgcc.a)
    target_link_libraries(test libstdc++.a)
    #链接网络库（如需要）
    #target_link_libraries(test ws2_32)
    #链接线程库（必须放到最后）
    target_link_libraries(test libpthread.a)
endif()

target_link_libraries(test
    PUBLIC
    #${REGULAR_LIBS}
    #${NLP_LIBS}
)

target_include_directories(test 
    PUBLIC
    #${REGULAR_INC}
    #${NLP_INC}
)

#add_subdirectory(./src/app)
# add_subdirectory(./src/init)
# 添加编译依赖，先生成app库，再生成init
# add_dependencies(init app)       


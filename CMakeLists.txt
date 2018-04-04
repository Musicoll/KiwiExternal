cmake_minimum_required(VERSION 3.1)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(Kiwi_External.cmake)

project(kiwi_externals CXX)
set_kiwi_external_path("${PROJECT_SOURCE_DIR}/libs/")
add_kiwi_external(external_kawa kawa ${PROJECT_SOURCE_DIR}/externals/kawa.cpp)
add_kiwi_external(external_koala koala ${PROJECT_SOURCE_DIR}/externals/koala.cpp)

set(TEST_SOURCES
${PROJECT_SOURCE_DIR}/Kiwi_External.hpp
${PROJECT_SOURCE_DIR}/Kiwi_Loader.hpp
test/test.cpp)
source_group(src FILES ${TEST_SOURCES})
add_executable(kiwi_loader_test ${TEST_SOURCES})
target_include_directories(kiwi_loader_test PRIVATE ${PROJECT_SOURCE_DIR})
else("${CMAKE_SYSTEM}" MATCHES "Linux")
	target_link_libraries(kiwi_loader_test ${CMAKE_DL_LIBS})
endif()
set_target_properties(kiwi_loader_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/libs/")
set_target_properties(kiwi_loader_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/libs/")
set_target_properties(kiwi_loader_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/libs/")

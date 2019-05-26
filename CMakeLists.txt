cmake_minimum_required (VERSION 3.1)
project (nng_event_loop CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set output paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

find_package(nng CONFIG REQUIRED)

file(GLOB SRCS
	"src/*.cpp"
	)

file(GLOB HDRS
	"include/nng_event_loop/*.hpp"
	)

add_library(${PROJECT_NAME} ${SRCS} ${HDRS})
target_link_libraries(${PROJECT_NAME} nng::nng)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	$<INSTALL_INTERFACE:include/nng_event_loop>
	)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/nng_event_loop>
	$<INSTALL_INTERFACE:include/nng_event_loop>
	)

add_executable("test_eventloop" test_eventloop.cpp)
target_link_libraries(test_eventloop nng_event_loop)


add_executable(test_publisher test_publisher.cpp)
target_link_libraries(test_publisher nng_event_loop)

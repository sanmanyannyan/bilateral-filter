cmake_minimum_required(VERSION 3.10)

add_executable(bench bench.cc ../src/bilateral-filter.cc)
include_directories(../inc)


# for OpenCV
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
	target_include_directories(bench PUBLIC ${OpenCV_INCLUDE_DIRS})
	target_link_libraries(bench ${OpenCV_LIBS})
endif()


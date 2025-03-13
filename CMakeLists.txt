cmake_minimum_required(VERSION 3.5)
project(oradar_ros)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_BUILD_TYPE RELEASE)

add_definitions(-O3)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/package_ros2.xml ${CMAKE_CURRENT_SOURCE_DIR}/package.xml COPYONLY)
add_definitions(-DROS2_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/package_ros2.xml ${CMAKE_CURRENT_SOURCE_DIR}/package.xml COPYONLY)

set(CMAKE_CXX_STANDARD 14)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED) 
# include_directories(${rclcpp_INCLUDE_DIRS})



##################################
########  Build and Install  #####
##################################

set(ORADAR_SDK_DIR "sdk")
add_subdirectory(${ORADAR_SDK_DIR})

include_directories(
  "${ORADAR_SDK_DIR}/include"
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(oradar_scan  ${CMAKE_CURRENT_SOURCE_DIR}/src/oradar_scan_node.cpp)
add_executable(oradar_pointcloud  ${CMAKE_CURRENT_SOURCE_DIR}/src/oradar_pointcloud_node.cpp)


#Ros2#

ament_target_dependencies(oradar_scan rclcpp sensor_msgs)
target_link_libraries(oradar_scan ord_sdk pthread)

ament_target_dependencies(oradar_pointcloud rclcpp sensor_msgs)
target_link_libraries(oradar_pointcloud ord_sdk pthread)

install(TARGETS oradar_scan oradar_pointcloud
DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch rviz2
DESTINATION share/${PROJECT_NAME}/
)

ament_package()




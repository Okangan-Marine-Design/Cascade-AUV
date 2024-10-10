cmake_minimum_required(VERSION 3.8)
project(mapping)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cascade_msgs REQUIRED)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_INSTALL_PREFIX}/share/cascade_msgs")
find_package(cv_bridge REQUIRED)
find_package(message_filters REQUIRED)
find_package(image_transport REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(mapping_node
  src/mapping_node.cpp
)
add_executable(visualizer 
    src/visualizer.cpp)

add_executable(serialization_test
    src/serialization_test.cpp)

add_executable(costmap_node
    src/cost_map_node.cpp)

target_include_directories(serialization_test PUBLIC include)

ament_target_dependencies(mapping_node rclcpp sensor_msgs geometry_msgs cv_bridge message_filters tf2 tf2_ros image_transport tf2_geometry_msgs OpenCV cascade_msgs)
target_include_directories(mapping_node PUBLIC include)

ament_target_dependencies(visualizer rclcpp cascade_msgs glfw3 OpenGL tf2 tf2_geometry_msgs)
target_link_libraries(visualizer ${OPENGL_LIBRARIES} glfw)
target_include_directories(visualizer PUBLIC include)

ament_target_dependencies(costmap_node rclcpp sensor_msgs geometry_msgs cv_bridge message_filters tf2 tf2_ros image_transport tf2_geometry_msgs OpenCV cascade_msgs)
target_include_directories(costmap_node PUBLIC include)


install(TARGETS
  mapping_node
  visualizer
  serialization_test
  costmap_node
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

#ROS2 CLI commands
## List of relevant ros2 cli commands i've found

have to run in workspace to start up package: 
source install/local_setup.bash

have to add plugin to PATH: 
export GZ_SIM_SYSTEM_PLUGIN_PATH=~/Desktop/projects/current/robosub/ros/src/plug/build

ROS2 Gazebo topic bridge:
ros2 run ros_gz_bridge parameter_bridge /TOPIC@ROS_MSG@GZ_MSG

ros2 run ros_gz_bridge parameter_bridge /box/cmd_vel@geometry_msgs/msg/Twist@gz.msgs.Twist /camera/left@sensor_msgs/msg/Image@gz.msgs.Image /camera/right@sensor_msgs/msg/Image@gz.msgs.Image

ros2 run ros_gz_bridge parameter_bridge /box/cmd_vel@geometry_msgs/msg/Twist@gz.msgs.Twist /camera/rgb@sensor_msgs/msg/Image@gz.msgs.Image /camera/depth@sensor_msgs/msg/Image@gz.msgs.Image


ros2 topic pub /box/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.7, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}" -r 60

ros2 run orbslam3 stereo ORBvoc.txt gazebo.yaml false

For some reason fixes some builds?: colcon build --symlink-install

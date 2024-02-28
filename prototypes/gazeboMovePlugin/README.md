# Twist Ros2_to_GZ_SIM v1.0
---
This Plugin is designed to allow you to move a gz model using simple Twist commands directly from ros2 using a ros_gz bridge

## Installation and Use Instructions

### Copy code into some directory (probably in your ros2 package)
```bash
cp ~/your_Github_Dir/gazeboMovePlugin ~/your_Ros2_pkg/gazeboMovePlugin
```

### export directory to GZ_SIM_PLUGIN Path
```bash
export GZ_SIM_SYSTEM_PLUGIN_PATH=`pwd`/build
```
### add plugin to your models sdf file
```sdf
<plugin filename="movePlugin" name="move_Plugin::movePlugin">
</plugin>
```

### change model and topic name
```c++
movePlugin.cpp:

std::string topic = "/box/cmd_vel", modelName = "box";
->
std::string topic = "/your_name/cmd_vel", modelName = "your_name";
```

### create build folder and compile
```bash
mkdir build
cd build
cmake ..
make
```

### run GZ_ROS2 Topic Bridge:
```bash
ros2 run ros_gz_bridge parameter_bridge /box/cmd_vel@geometry_msgs/msg/Twist@gz.msgs.Twist
```

### publish to ros topic via cli after launching gz sim 
```bash
ros2 topic pub /box/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 1.8}}" -r 60
```

Now you can publish Twist Messages on your ros2 topic, which will move the gazebo model!


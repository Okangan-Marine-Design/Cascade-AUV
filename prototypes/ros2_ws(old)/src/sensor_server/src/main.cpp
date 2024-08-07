#include "rclcpp/rclcpp.hpp"
#include "robo_messages/srv/rgbd.hpp"
#include "robo_messages/msg/rgbd.hpp"
#include "robo_messages/srv/stereo.hpp"
#include "robo_messages/srv/depth.hpp"
#include "robo_messages/srv/imu.hpp"
#include "robo_messages/srv/stereo2_rgbd.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/header.hpp"
#include <memory>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <cv_bridge/cv_bridge.hpp>

using namespace std::chrono_literals;
using namespace cv;

std::shared_ptr<rclcpp::Node> node;
bool depth_received=false;

sensor_msgs::msg::Image recent_front_right_cam;
sensor_msgs::msg::Image recent_front_left_cam;
sensor_msgs::msg::Image recent_front_cam_rgb;
sensor_msgs::msg::Image recent_front_cam_depth;
sensor_msgs::msg::Image recent_bottom_cam;
sensor_msgs::msg::Imu recent_imu;

rclcpp::Client<robo_messages::srv::Stereo2RGBD>::SharedPtr Stereo2RGBD_client;

rclcpp::Publisher<robo_messages::msg::RGBD>::SharedPtr front_rgbd_publisher;

rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr front_cam_rgb_sub;
rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr front_cam_depth_sub;
rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr front_right_cam_sub;
rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr front_left_cam_sub;
rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr bottom_cam_sub;
rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr depth; 

//rclcpp::Subscription<???>::SharedPtr hydrophone; 
//TODO: figure out what kind of data we get from hydrophones

void call_RGBD_processing(){
    auto request = std::make_shared<robo_messages::srv::Stereo2RGBD::Request>();
    
    request->left=recent_front_left_cam;
    request->right=recent_front_right_cam;

    auto result = Stereo2RGBD_client->async_send_request(request); 
    while(!result.valid()){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "waiting on Stereo2RGBD service");
    }
        
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "result is valid");
        auto temp=*result.get(); 
        //recent_front_cam_rgb=temp.rgb;
        //recent_front_cam_depth=temp.depth;
        //temporarily commented out for simulation purposes
        auto message = robo_messages::msg::RGBD();
        message.rgb=recent_front_cam_rgb;
        message.depth=recent_front_cam_depth;
        front_rgbd_publisher->publish(message);//publishes to /rgbd every time processing is complete
}

void front_rgb_subscription_callback(const sensor_msgs::msg::Image &msg){
    recent_front_cam_rgb=msg;
    if(depth_received){
        auto message = robo_messages::msg::RGBD();
        message.rgb=recent_front_cam_rgb;
        message.depth=recent_front_cam_depth;
        front_rgbd_publisher->publish(message);
    }
}
void front_depth_subscription_callback(const sensor_msgs::msg::Image &msg){
    recent_front_cam_depth=msg;
    depth_received=true;
}
void front_right_cam_subscription_callback(const sensor_msgs::msg::Image &msg){
    recent_front_right_cam=msg;
}
void front_left_cam_subscription_callback(const sensor_msgs::msg::Image &msg){
    recent_front_left_cam=msg;
    //call_RGBD_processing();
    //TODO: remove processing node? move code into the server?
}

void bottom_cam_subscription_callback(const sensor_msgs::msg::Image &msg){
    recent_bottom_cam=msg;
}
void imu_subscription_callback(const sensor_msgs::msg::Imu &msg){
    recent_imu=msg;
    //TODO: research smoothing/filtering for imu data
    //will this even be an issue with underwater movement? will there be significant data noise coming from motors?
}

void depth_subscription_callback(const std_msgs::msg::Float32 &msg){
}
void front_rgbd_service_callback(const std::shared_ptr<robo_messages::srv::RGBD::Request> request,
    std::shared_ptr<robo_messages::srv::RGBD::Response>      response){
    response->rgb = recent_front_cam_rgb;
    response->depth = recent_front_cam_depth;
}
void front_stereo_service_callback(const std::shared_ptr<robo_messages::srv::Stereo::Request> request,
    std::shared_ptr<robo_messages::srv::Stereo::Response>      response){
    response->left = recent_front_left_cam;
    response->right = recent_front_right_cam;
}
void bottom_cam_service_callback(const std::shared_ptr<robo_messages::srv::Stereo::Request> request,
    std::shared_ptr<robo_messages::srv::Stereo::Response>      response){
    //this will use primitive ROS2 img type since its a mono camera for sure
}
void imu_service_callback(const std::shared_ptr<robo_messages::srv::Imu::Request> request,
    std::shared_ptr<robo_messages::srv::Imu::Response>      response){
}

void depth_service_callback(const std::shared_ptr<robo_messages::srv::Depth::Request> request,
    std::shared_ptr<robo_messages::srv::Depth::Response>      response){
}
int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    node = rclcpp::Node::make_shared("sensor_server");
    
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sensor_server launched!");
    std_msgs::msg::Header empty_header;
    cv::Mat empty = cv::Mat(480, 640, CV_8UC3, Scalar(86, 120, 255));

    cv_bridge::CvImage emptyCV = cv_bridge::CvImage(empty_header,std::string("RGB"),empty);
    emptyCV.toImageMsg(recent_front_left_cam);
    emptyCV.toImageMsg(recent_front_right_cam);
    emptyCV.toImageMsg(recent_front_cam_rgb);
    emptyCV.toImageMsg(recent_front_cam_depth);
    emptyCV.toImageMsg(recent_bottom_cam);

    //creation of front facing RGBD data subscription and service
    //subscriptions used only in simulator, no real camera can reliably provide direct rbgd data underwater because of IF light attenuation 
    rclcpp::Service<robo_messages::srv::RGBD>::SharedPtr front_rgbd_service = 
        node->create_service<robo_messages::srv::RGBD>("front_rgbd", &front_rgbd_service_callback);
    front_cam_rgb_sub = node->create_subscription<sensor_msgs::msg::Image>("/camera/front/rgb",10, &front_rgb_subscription_callback);
    front_cam_depth_sub = node->create_subscription<sensor_msgs::msg::Image>("/camera/front/depth",10,&front_depth_subscription_callback);
    front_rgbd_publisher = node->create_publisher<robo_messages::msg::RGBD>("/front_rgbd", rclcpp::QoS(10));
    Stereo2RGBD_client = node->create_client<robo_messages::srv::Stereo2RGBD>("stereo2rgbd");//use name of service being requested from

    while (!Stereo2RGBD_client->wait_for_service(1s)) {//checking if the service exists
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for Stereo2RGBD service. Exiting.");
            return 0;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Stereo2RGBD service not responding, trying again...");
    }

    //image services and subscriptions
    rclcpp::Service<robo_messages::srv::Stereo>::SharedPtr front_stereo_service = 
        node->create_service<robo_messages::srv::Stereo>("front_stereo", &front_stereo_service_callback);
    rclcpp::Service<robo_messages::srv::Stereo>::SharedPtr bottom_cam_service = 
        node->create_service<robo_messages::srv::Stereo>("bottom_cam", &bottom_cam_service_callback);

    front_right_cam_sub = node->create_subscription<sensor_msgs::msg::Image>("/camera/front/right",10, &front_right_cam_subscription_callback);
    front_left_cam_sub = node->create_subscription<sensor_msgs::msg::Image>("/camera/front/left",10,&front_left_cam_subscription_callback);
    bottom_cam_sub = node->create_subscription<sensor_msgs::msg::Image>("/camera/bottom",10,&bottom_cam_subscription_callback);

    //imu subscription and service
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub =
        node->create_subscription<sensor_msgs::msg::Imu>("/imu",10, &imu_subscription_callback);
    rclcpp::Service<robo_messages::srv::Imu>::SharedPtr imu_service = 
        node->create_service<robo_messages::srv::Imu>("imu", &imu_service_callback);

    //TODO: add hydrophone subscription
    //TODO: add hydrophone service

    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr depth_sub =
        node->create_subscription<std_msgs::msg::Float32>("/depth",10, &depth_subscription_callback);
    rclcpp::Service<robo_messages::srv::Depth>::SharedPtr depth_service = 
        node->create_service<robo_messages::srv::Depth>("depth", &depth_service_callback);

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "All subscriptions and services started. Ready to send sensor data.");
    rclcpp::executors::MultiThreadedExecutor exec;
    exec.add_node(node);
    exec.spin();
    return 0;
}

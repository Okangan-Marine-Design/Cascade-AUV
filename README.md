# Marine Design Software

## Overview

This repository contains the software for the Cascade AUV

## ROS2

We use ROS2 to facilitate internal communication between all the programs involved in our sub. We have aimed for a modular format, which may look a little confusing at first visually, however it allows for each part of the system to be isolated and tested easily, whether it be in simulation or just unit testing. 

![ROS Graph](/diagrams/rosgraph5.png)

## Automated Planning
PyTrees

## 3D Mapping
Sparse Voxel Grid implemented using Bonxai
Intel D455

## Object Detection
YoloV9 object detection

## Navigation
3 part system
nav, motion planner, motor_cortex

## Control Systems
PID etc etc

## Esp Bridge

The ESP Bridge is a program that runs on the ESP32. It is responsible for interfacing with the sensors and controlling the motors and LEDs. The ESP Bridge is written in C++ and is built using the PlatformIO framework.



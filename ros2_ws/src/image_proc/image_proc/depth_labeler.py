import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from geometry_msgs.msg import PoseStamped
from cascade_msgs.msg import ImageWithPose
from cv_bridge import CvBridge, CvBridgeError
from message_filters import ApproximateTimeSynchronizer, Subscriber
import numpy as np
import cv2

class DepthLabelerNode(Node):
    def __init__(self):
        super().__init__("depth_labeler")
        self.bridge = CvBridge()
        queue_size = 20
        acceptable_delay = 0.06  # seconds
        tss = ApproximateTimeSynchronizer(
            [Subscriber(self, Image, "/depth_map"),
             Subscriber(self, Image, "/labeled_image"),
             Subscriber(self, PoseStamped, "/pose")],
            queue_size,
            acceptable_delay)
        tss.registerCallback(self.synced_callback)
        self.publisher_ = self.create_publisher(ImageWithPose, '/semantic_depth_with_pose', 10)

    def synced_callback(self, depth_msg, label_msg, pose_msg):
        try:
            depth_image = self.bridge.imgmsg_to_cv2(depth_msg, desired_encoding="passthrough")
            label_image = self.bridge.imgmsg_to_cv2(label_msg, desired_encoding="passthrough")
        except CvBridgeError as e:
            self.get_logger().error(f"Failed to convert images: {e}")
            return

        # Assuming label_image has two channels: class and confidence
        class_image = label_image[:, :, 0]
        confidence_image = label_image[:, :, 1]

        # Create a new image with an extra dimension to store class and confidence
        height, width = depth_image.shape
        combined_image = np.zeros((height, width, 3), dtype=np.float32)
        combined_image[:, :, 0] = depth_image

        # Create a copy of class and confidence images to avoid modifying the originals
        labeled_class_image = np.zeros_like(class_image)
        labeled_confidence_image = np.zeros_like(confidence_image)

        # Process each unique class in the class_image
        unique_classes = np.unique(class_image)
        for detected_class in unique_classes:
            if detected_class == 0:
                continue  # Assuming class 0 is the background or an invalid class

            # Create a mask for the current class
            class_mask = (class_image == detected_class)

            # Get the depth values within the mask
            depth_patch = depth_image[class_mask]

            if depth_patch.size == 0:
                continue  # Skip if no pixels are found for this class

            # Find the minimum and maximum depth values within the mask
            min_depth = np.min(depth_patch)

            # Calculate the threshold depth value
            threshold_depth = min_depth * 1.1 
            # Find the pixels within the desired depth range
            depth_mask = (depth_image >= min_depth) & (depth_image <= threshold_depth) & class_mask

            # Label the pixels within the depth range
            labeled_class_image[depth_mask] = detected_class
            labeled_confidence_image[depth_mask] = 1.0  # Set confidence to 1.0

        combined_image[:, :, 1] = labeled_class_image
        combined_image[:, :, 2] = labeled_confidence_image

        cv2.imshow('Class Image', labeled_class_image)
        cv2.imshow('Depth Image', depth_image)
        cv2.imshow('Combined Image', combined_image)
        cv2.waitKey(1)

        # Convert the combined image back to ROS Image message
        try:
            combined_msg = self.bridge.cv2_to_imgmsg(combined_image, encoding="32FC3")
        except CvBridgeError as e:
            self.get_logger().error(f"Failed to convert combined image: {e}")
            return

        msg = ImageWithPose()
        msg.image = combined_msg
        msg.pose = pose_msg.pose
        msg.header.stamp = self.get_clock().now().to_msg()
        self.publisher_.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = DepthLabelerNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()


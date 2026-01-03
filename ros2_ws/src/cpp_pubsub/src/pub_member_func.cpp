#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include <opencv2/opencv.hpp>

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0)
  {     
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("image_topic", 10);
    // 2. Open Camera (0 is usually the webcam)
    // This pipeline string mimics exactly what worked for you in the terminal
    std::string pipeline = "v4l2src device=/dev/video0 ! videoconvert ! appsink";

    // Open using the GSTREAMER backend
    cap_.open(pipeline, cv::CAP_GSTREAMER);

    if(!cap_.isOpened()) {
        // This should NOT happen now since the CLI worked
        RCLCPP_ERROR(this->get_logger(), "GStreamer pipeline failed to open!");
    }
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
      image_timer_ = this->create_wall_timer(
      100ms, std::bind(&MinimalPublisher::image_timer_callback, this));
  }

  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count_++); 
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }

  void image_timer_callback()
  {
    cv::Mat frame;
    cap_ >> frame;
    if (frame.empty()) {
      RCLCPP_WARN(this->get_logger(), "Empty frame captured");
      return;
    }
    // 5. Convert OpenCV -> ROS Message
    std_msgs::msg::Header header;
    header.stamp = this->get_clock()->now();
    header.frame_id = "camera_frame";
    auto msg = cv_bridge::CvImage(header, "bgr8", frame).toImageMsg();
    image_publisher_->publish(*msg);
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr image_timer_;
  size_t count_;
  cv::VideoCapture cap_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0; 
}
/**
 *  @file   MultiCameraDriverSim.hpp
 *  @date   2020-05-20
 *  @author Sungkyu Kang
 *  @author hyunseok Yang
 *  @brief
 *        ROS2 Multi Camera Driver class for simulator
 *  @remark
 *  @warning
 *       LGE Advanced Robotics Laboratory
 *         Copyright(C) 2019 LG Electronics Co., LTD., Seoul, Korea
 *         All Rights are Reserved.
 */
#ifndef _MultiCameraDriverSim_H_
#define _MultiCameraDriverSim_H_

#include "driver_sim/driver_sim.hpp"
#include <image_transport/image_transport.h>
#include <camera_info_manager/camera_info_manager.h>
#include <sensor_msgs/msg/camera_info.hpp>
#include <protobuf/images_stamped.pb.h>
#include <protobuf/camerasensor.pb.h>

class MultiCameraDriverSim : public DriverSim
{
public:
  MultiCameraDriverSim();
  virtual ~MultiCameraDriverSim();

private:
  virtual void Initialize();
  virtual void Deinitialize();
  virtual void UpdateData();

  void GetCameraSensorMessage(const std::string camera_name);
  void InitializeCameraInfoMessage(const std::string camera_name, const std::string frame_id);

private:
  std::string m_hashKeySub;

  // buffer from simulation
  gazebo::msgs::ImagesStamped m_pbBuf;

  // message for ROS2 communictaion
  sensor_msgs::msg::Image msg_img;

  // Camera sensor info buffer from simulator
  gazebo::msgs::CameraSensor m_pbBufCameraSensorInfo;

  // Camera info publishers.
  std::vector<rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr> pubCamerasInfo;

  // Camera info managers
  std::vector<std::shared_ptr<camera_info_manager::CameraInfoManager>> cameraInfoManager;

  // Image publisher
  std::vector<image_transport::Publisher> pubImages;
};
#endif
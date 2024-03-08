/**
 *  @file   segmentation_camera.cpp
 *  @date   2024-03-01
 *  @author hyunseok Yang
 *  @brief
 *        ROS2 Segmentation Camera class for simulator
 *  @remark
 *  @warning
 *      LGE Advanced Robotics Laboratory
 *      Copyright(C) 2024 LG Electronics Co., LTD., Seoul, Korea
 *      All Rights are Reserved.
 *      SPDX-License-Identifier: MIT
 */

#include "cloisim_ros_camera/segmentation_camera.hpp"

using namespace std;

namespace cloisim_ros
{
SegmentationCamera::SegmentationCamera(
    const rclcpp::NodeOptions &options_,
    const string node_name,
    const string namespace_)
    : CameraBase(options_, node_name, namespace_)
{
  Start();
}

SegmentationCamera::SegmentationCamera(
    const string node_name,
    const string namespace_)
    : SegmentationCamera(rclcpp::NodeOptions(), node_name, namespace_)
{
}

SegmentationCamera::~SegmentationCamera()
{
  // DBG_SIM_INFO("Delete SegmentationCamera");
  Stop();
}

void SegmentationCamera::InitializeCameraData()
{
  uint16_t portData;
  get_parameter_or("bridge.Data", portData, uint16_t(0));

  const auto hashKeyData = GetTargetHashKey("Data");

  auto data_bridge_ptr = CreateBridge();
  if (data_bridge_ptr != nullptr)
  {
    data_bridge_ptr->Connect(zmq::Bridge::Mode::SUB, portData, hashKeyData);
    AddPublisherThread(data_bridge_ptr, bind(&SegmentationCamera::PublishData, this, std::placeholders::_1));
  }

  pub_labelinfo_ = create_publisher<vision_msgs::msg::LabelInfo>("label_info", rclcpp::QoS(rclcpp::KeepLast(1)).transient_local());

  DBG_SIM_INFO("SegmentationCamera hashKey: data(%s)", hashKeyData.c_str());
}

void SegmentationCamera::PublishData(const string &buffer)
{
  if (!pb_seg_.ParseFromString(buffer))
  {
    DBG_SIM_ERR("!!!Parsing error, size(%d)", buffer.length());
    return;
  }

  SetTime(pb_seg_.image_stamped().time());

  vision_msgs::msg::LabelInfo msg_label_info;

  // pb_seg_.clas

  pub_labelinfo_->publish(msg_label_info);

  // send camera image
  CameraBase::PublishData(pb_seg_.image_stamped());
}
}  // namespace cloisim_ros
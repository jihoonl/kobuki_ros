/**
 * @file /kobuki_node/include/kobuki_node/kobuki_node.hpp
 *
 * @brief Wraps the kobuki driver in a ROS node
 *
 * @date 10/04/2012
 **/

/*****************************************************************************
 ** Ifdefs
 *****************************************************************************/

#ifndef KOBUKI_NODE_HPP_
#define KOBUKI_NODE_HPP_

/*****************************************************************************
 ** Includes
 *****************************************************************************/

#include <string>
#include <boost/shared_ptr.hpp>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <device_comms/JointCommand.h>
#include <device_comms/JointState.h>
#include <kobuki_comms/SensorData.h>
#include <ecl/sigslots.hpp>
#include <kobuki_driver/kobuki.hpp>

/*****************************************************************************
 ** Namespaces
 *****************************************************************************/

namespace kobuki
{
class KobukiNode
{
public:
  /*********************
   ** C&D
   **********************/
  KobukiNode(std::string& node_name);
  ~KobukiNode();

  bool init(ros::NodeHandle& nh);

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:

  void advertiseTopics(ros::NodeHandle& nh);
  void subscribeTopics(ros::NodeHandle& nh);

  void publishTransform(const geometry_msgs::Quaternion &odom_quat);
  void publishOdom(const geometry_msgs::Quaternion &odom_quat, const ecl::linear_algebra::Vector3d &pose_update_rates);

  Kobuki kobuki;

  /*********************
   ** Variables
   **********************/
  std::string name; // name of the ROS node

  // Continuously published messages
  geometry_msgs::TransformStamped odom_trans;
  nav_msgs::Odometry odom;
  ecl::Pose2D<double> pose;

  const std::string wheel_left_name;
  const std::string wheel_right_name;

  std::string odom_frame;
  std::string base_frame;
  bool publish_tf;

  /*********************
   ** Ros Comms
   **********************/
  ros::Publisher ir_data_publisher, dock_ir_data_publisher, inertia_data_publisher, imu_data_publisher,
                 cliff_data_publisher, current_data_publisher, magnet_data_publisher, hw_data_publisher,
                 fw_data_publisher, time_data_publisher, st_gyro_data_publisher, eeprom_data_publisher,
                 gp_input_data_publisher, joint_state_publisher, odom_publisher, wheel_left_state_publisher,
                 wheel_right_state_publisher, sensor_data_publisher;
  ros::Subscriber wheel_left_command_subscriber, wheel_right_command_subscriber, velocity_command_subscriber,
                  kobuki_command_subscriber, enable_subscriber, disable_subscriber;

  ecl::Slot<> slot_wheel_state, slot_sensor_data, slot_ir, slot_dock_ir, slot_inertia, slot_cliff, slot_current, slot_magnet, slot_hw, slot_fw, slot_time,
              slot_st_gyro, slot_eeprom, slot_gp_input;
  ecl::Slot<const std::string&> slot_debug, slot_info, slot_warn, slot_error;
  ecl::Signal<const device_comms::JointCommand> sig_joint_command;
  tf::TransformBroadcaster odom_broadcaster;
  sensor_msgs::JointState joint_states;

  /*********************
   ** SigSlots
   **********************/
  void publishWheelState();
  void publishSensorData();
  void publishIRData();
  void publishDockIRData();
  void publishInertiaData();
  void publishCliffData();
  void publishCurrentData();
  void publishMagnetData();
  void publishHWData();
  void publishFWData();
  void publishTimeData();
  void publishStGyroData();
  void publishEEPROMData();
  void publishGpInputData();
  void subscribeJointCommandLeft(const device_comms::JointCommand);
  void subscribeJointCommandRight(const device_comms::JointCommand);
  void subscribeVelocityCommand(const geometry_msgs::TwistConstPtr);
  void subscribeKobukiCommand(const kobuki_comms::CommandConstPtr);

  /*********************
   ** Ros Logging
   **********************/
  void rosDebug(const std::string &msg)
  {
    ROS_DEBUG_STREAM(msg);
  }
  void rosInfo(const std::string &msg)
  {
    ROS_INFO_STREAM(msg);
  }
  void rosWarn(const std::string &msg)
  {
    ROS_WARN_STREAM(msg);
  }
  void rosError(const std::string &msg)
  {
    ROS_ERROR_STREAM(msg);
  }

  void enable(const std_msgs::StringConstPtr msg)
  {
    kobuki.run();
    ROS_INFO_STREAM("kobuki enabled.");
  }
  ;

  void disable(const std_msgs::StringConstPtr msg)
  {
    kobuki.stop();
    ROS_INFO_STREAM("kobuki disable.");
  }
  ;
};

} // namespace kobuki

#endif /* KOBUKI_NODE_HPP_ */
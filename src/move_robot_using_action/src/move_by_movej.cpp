
//使用sppedj的方式，因为直接对关节进行操作的话，机械臂可能会进入奇异点，所以这种方法有时候不行。
//而且没有采用类的方式，代码看起来相对比较乱。

#include "ros/ros.h"
#include "sstream"
#include "cmath"
#include "moveit/move_group_interface/move_group.h"
#include "moveit/robot_model_loader/robot_model_loader.h"
#include "moveit/robot_model/robot_model.h"
#include "moveit/robot_state/robot_state.h"
#include "geometry_msgs/Pose.h"
#include "std_msgs/String.h"
#include <control_msgs/FollowJointTrajectoryAction.h>
#include "control_msgs/FollowJointTrajectoryActionGoal.h"
#include <actionlib/client/simple_action_client.h>
#include "emg/IMU_sEMG.h"
#define PI 3.1415926

using namespace std;
typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> Client;
emg::IMU_sEMG emg_data;
int callback_flag = 0;
int emg_data_flag = 0;
double time_interval = 0.5;

//定义全局函数
void subCallback(const emg::IMU_sEMGConstPtr& msg);
std::string combinemessage(const std::vector<double>& joint_velocities, double acc = 0.6 , double t = time_interval);//加速度需要调整
std::string cal_hdh_message(const std::vector<double>& joint_values_now);
std::string double2string(double a);

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "move_by_movej");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh;

    //定义和moveit相关的变量
    moveit::planning_interface::MoveGroup group("manipulator");
    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    moveit::core::RobotState kinematic_state(*group.getCurrentState());
    const robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("manipulator");

    //建立一个action的客户端，一方面是为了给follow_joint_trjactory发送指令，另外一方面来验证UR的驱动是否已经建立了
    Client ac("follow_joint_trajectory", true);
    if (!ac.waitForServer(ros::Duration(2.0)))
    {
        ROS_ERROR("Could not connect to action server");
        return false;
    }
    else
    {
        ROS_INFO("Connecte with UR10 Now!");
    }

    //让机械臂每次初始化的过程中都能运行到一个固定的位置，在这里是提前预设的一个位置look
    group.setMaxVelocityScalingFactor(0.3);
    group.setMaxAccelerationScalingFactor(0.2);
    group.setNamedTarget("look");
    group.move();
    ros::Duration(1).sleep();

    //测试moveit中各个坐标系
    ROS_INFO_STREAM("The pose reference frame is "<<group.getPoseReferenceFrame());
    ROS_INFO_STREAM("The planning frame is "<<group.getPlanningFrame());
    ROS_INFO_STREAM("The end link is "<<group.getEndEffectorLink());
    ROS_INFO_STREAM("Current Pose are"<<group.getCurrentPose("tool0").pose);

    //定义和位置相关的变量
    geometry_msgs::Pose pose;
    std::vector<double> joint_values_now,joint_values_before;

    joint_values_now.resize(6);
    joint_values_before.resize(6);
    std::vector<double> joint_velocities;
    joint_velocities.resize(6);

    pose = group.getCurrentPose("tool0").pose;
    kinematic_state.setFromIK(joint_model_group,pose);
    kinematic_state.copyJointGroupPositions(joint_model_group,joint_values_now);
    ROS_INFO_STREAM("The current joint values are:");
    for(auto joint_value:joint_values_now) ROS_INFO_STREAM(joint_value);

    joint_values_before = joint_values_now;
    //开始接受EMG数据
    
    ros::Subscriber emg_data_sub = nh.subscribe("emg_data", 1000, subCallback);
    ros::Publisher speed_pub = nh.advertise<std_msgs::String>("ur_speed_pub", 1000);
    ros::Publisher hdh_message_pub = nh.advertise<std_msgs::String>("haodehong_message", 1000);
    std_msgs::String String_speed;
    std_msgs::String hdh_msg;
    while(ros::ok())
    {
        if(callback_flag == 0)
        {
            ROS_WARN("WAITING FOR EMG DATA");
            ros::Duration(time_interval).sleep();//假设臂带按照2Hz的频率发送数据
            continue;
        }
        else
        {
            if(emg_data_flag == 1)
            {
                emg_data_flag = 0;
                pose.position.x = pose.position.x + emg_data.IMU_datax;
                pose.position.y = pose.position.y + emg_data.IMU_datay;
                pose.position.z = pose.position.z + emg_data.IMU_dataz;
                kinematic_state.setFromIK(joint_model_group,pose);
                kinematic_state.copyJointGroupPositions(joint_model_group,joint_values_now);
                for(auto i = 0 ; i < 6 ; i ++)
                {
                    joint_velocities[i] = (joint_values_now[i] - joint_values_before[i])/time_interval;
                }
                joint_values_before = joint_values_now;
                String_speed.data = combinemessage(joint_velocities);
                hdh_msg.data = cal_hdh_message(joint_values_now);
                hdh_message_pub.publish(hdh_msg);
                speed_pub.publish(String_speed);
                ROS_INFO("THE PROGRAMM IS RUNNING");
            }    
        }
    }   
    return 0;
}
void subCallback(const emg::IMU_sEMGConstPtr& msg)
{
    emg_data.IMU_datax = msg->IMU_datax;
    emg_data.IMU_datay = msg->IMU_datay;
    emg_data.IMU_dataz = msg->IMU_dataz;
    if(callback_flag == 0) callback_flag = 1;
    emg_data_flag = 1;
}
std::string combinemessage(const std::vector<double>& joint_velocities, double acc , double t)
{
    std::string string_speed;
    string_speed = "speedj([";
    string_speed = string_speed + double2string(joint_velocities[0]);
    string_speed = string_speed + ",";
    string_speed = string_speed + double2string(joint_velocities[1]);
    string_speed = string_speed + ",";
    string_speed = string_speed + double2string(joint_velocities[2]);
    string_speed = string_speed + ",";
    string_speed = string_speed + double2string(joint_velocities[3]);
    string_speed = string_speed + ",";
    string_speed = string_speed + double2string(joint_velocities[4]);
    string_speed = string_speed + ",";
    string_speed = string_speed + double2string(joint_velocities[5]);
    string_speed = string_speed + "],";
    string_speed = string_speed + "a=";
    string_speed = string_speed + double2string(acc);
    string_speed = string_speed + ",";
    string_speed = string_speed + "t=";
    string_speed = string_speed + double2string(t);
    string_speed = string_speed + ")";
    return string_speed;
}
std::string cal_hdh_message(const std::vector<double>& joint_values_now)
{
    std::vector<double> joint_values;
    joint_values = joint_values_now;
    for(int i = 0 ; i < 6 ; i ++)
    {
        joint_values[i] = joint_values[i]*180/PI;
    }
    std::string hdh_msg;
    hdh_msg = double2string(joint_values[0]) + ",";
    hdh_msg = hdh_msg + double2string(joint_values[1]) + ",";
    hdh_msg = hdh_msg + double2string(joint_values[2]) + ",";
    hdh_msg = hdh_msg + double2string(joint_values[3]) + ",";
    hdh_msg = hdh_msg + double2string(joint_values[4]) + ",";
    hdh_msg = hdh_msg + double2string(joint_values[5]);
    return hdh_msg;
}
std::string double2string(double input)
{
    std::string string_temp;
    stringstream stream;
    stream<<input;
    string_temp = stream.str();
    return string_temp;
}

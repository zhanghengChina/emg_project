
// 这个是和加上肌电信号配套的代码。
// UR上的demo就是运行这个节点

#include "ros/ros.h"
#include "emg/imu.h"
#include "armlet_imu/IMU.h"
#include "emg/IMU_sEMG.h"
ros::Time time_init;

const double beishu = 1.0;

int emg_flag = 0;
ros::Subscriber sub;
ros::Publisher pub;


void callback(const armlet_imu::IMU::ConstPtr& msg)
{
    if(emg_flag == 0) emg_flag = 1;

    emg::IMU_sEMG emg_msg;

    emg_msg.IMU_datax = msg->data[0];
    emg_msg.IMU_datay = msg->data[1];
    emg_msg.IMU_dataz = msg->data[2];
    emg_msg.sEMG_data = 0;

    ROS_INFO_STREAM("ORIGINAL DATA IS\n"<<emg_msg);
    ROS_INFO_STREAM("Total time is "<<(ros::Time::now() - time_init));
    emg_msg.IMU_datay/=beishu;
    emg_msg.IMU_datax/=beishu;
    emg_msg.IMU_dataz/=beishu;
    pub.publish(emg_msg);
    time_init = ros::Time::now();
    
    
}
int main(int argc, char *argv[])
{
    ros::init(argc, argv, "ReceivingNew");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh;
    pub = nh.advertise<emg::IMU_sEMG>("emg_data", 1000);
    sub = nh.subscribe<armlet_imu::IMU>("IMU_data",10,callback);
    ROS_INFO("RECEIVING EMG DATA!");
    ros::Duration(1.0).sleep();
    while(ros::ok())
    {
        if(emg_flag == 0)
        {
            ROS_WARN("NO EMG DATA!");
            ros::Duration(0.5).sleep();
        }
    }
    spinner.stop();
    return 0;
}

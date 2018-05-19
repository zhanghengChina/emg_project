
// 和新松机器人配合的代码
// 接收zzm的左右臂的IMU数据，并且组合成一个消息类型发送给控制机器人运动的节点
// 数据格式：机器人左臂在前，右臂在后

#include "ros/ros.h"
#include "emg/imu.h"
#include "armlet_imu/IMU.h"
#include "emg/imu.h"

const double beishu = 1.0;

int emg_flag = 0;
int emg_flag_right = 0;
ros::Subscriber sub;
ros::Subscriber right_sub;

ros::Publisher pub;
emg::imu global_imu_data;


void right_callback(const armlet_imu::IMU::ConstPtr& msg)
{
    if(emg_flag_right == 0) emg_flag_right = 1;
    global_imu_data.data[3] = msg->data[0];
    global_imu_data.data[4] = msg->data[1];
    global_imu_data.data[5] = msg->data[2];
}

void callback(const armlet_imu::IMU::ConstPtr& msg)
{
    if(emg_flag == 0) emg_flag = 1;
    global_imu_data.data[0] = msg->data[0];
    global_imu_data.data[1] = msg->data[1];
    global_imu_data.data[2] = msg->data[2];
}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "Receiving_sinsun_robot");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh;

    global_imu_data.data.resize(6);

    pub = nh.advertise<emg::imu>("imu_data", 1000);

    sub = nh.subscribe<armlet_imu::IMU>("IMU_data_left",10,callback);
    right_sub = nh.subscribe<armlet_imu::IMU>("IMU_data_right",10,right_callback);
    ROS_INFO("RECEIVING EMG DATA!");
    ros::Duration(1.0).sleep();

    ros::Rate loop_rate(30);
    while (ros::ok())
    {
        if(emg_flag == 0)
        {
            ROS_WARN("There is no imu data of the left arm!");
            ros::Duration(0.5).sleep();
        }
        if(emg_flag_right == 0)
        {
            ROS_WARN("There is no imu data of the right arm!");
            ros::Duration(0.5).sleep();
        }
        ROS_INFO("Receiving imu data");
        pub.publish(global_imu_data);
        ros::spinOnce();
        loop_rate.sleep();
    }
    spinner.stop();
    return 0;
}

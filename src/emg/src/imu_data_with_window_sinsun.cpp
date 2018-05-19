
// 和新松机器人配合的代码
// 接收zzm的左右臂的IMU数据，并且组合成一个消息类型发送给控制机器人运动的节点
// 数据格式：机器人左臂在前，右臂在后
// 为了增加机器人运动的平滑性，window_process函数起到了对数据平滑的作用，窗长是10个数据，每次滑动一个数据。
// zzm发送的数据频率在50Hz左右，所以理论上的延时在0.2+pid算法的延时。

#include "ros/ros.h"
#include "emg/imu.h"
#include "armlet_imu/IMU.h"
#include "Eigen/Geometry"
#include "emg/imu.h"

const double beishu = 1.0;
const int window_size = 10;
const int window_step = 1;

int emg_flag = 0;
int emg_flag_right = 0;
ros::Subscriber sub;
ros::Subscriber right_sub;

ros::Publisher pub;
emg::imu global_imu_data;
std::vector<emg::imu> global_imu_data_vector;

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

void window_process()
{
    
    for(int i = 0 ; i < window_size - 1 ; i ++)
    {
        global_imu_data_vector[i] = global_imu_data_vector[i + 1];
    }
    global_imu_data_vector[window_size - 1] = global_imu_data;
    
    Eigen::Vector3d left = {0,0,0}, right = {0,0,0};
    for(int i = 0 ; i < window_size ; i ++)
    {
        left[0] += global_imu_data_vector[i].data[0];
        left[1] += global_imu_data_vector[i].data[1];
        left[2] += global_imu_data_vector[i].data[2];
        right[0] +=global_imu_data_vector[i].data[3];
        right[1] +=global_imu_data_vector[i].data[4];
        right[2] +=global_imu_data_vector[i].data[5];
    }

    global_imu_data.data[0] = left[0]/window_size;
    global_imu_data.data[1] = left[1]/window_size;
    global_imu_data.data[2] = left[2]/window_size;
    global_imu_data.data[3] = right[0]/window_size;
    global_imu_data.data[4] = right[1]/window_size;
    ROS_INFO("Processing");
    global_imu_data.data[5] = right[2]/window_size;
    
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "imu_data_with_window_sinsun");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh;

    global_imu_data.data.resize(6);

    global_imu_data_vector.resize(window_size);
    for(int i = 0 ; i < window_size; i ++)
    {
        global_imu_data_vector[i].data.resize(6);
    }

    pub = nh.advertise<emg::imu>("imu_data", 1000);

    sub = nh.subscribe<armlet_imu::IMU>("IMU_data_left",10,callback);
    right_sub = nh.subscribe<armlet_imu::IMU>("IMU_data_right",10,right_callback);
    ROS_INFO("RECEIVING EMG DATA!");
    ros::Duration(1.0).sleep();
    int count = 0;

    ros::Rate loop_rate(50);
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

        if(count < window_size)
        {
            global_imu_data_vector[count] = global_imu_data;
            ROS_INFO("Receiving imu data");
            count ++;
        }
        else
        {
            window_process();
            ROS_INFO("Receiving imu data");
            pub.publish(global_imu_data);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    spinner.stop();
    return 0;
}


// 这个节点的作用是读取指定位置下的txt的离线数据，并且将数据发送给实际的机器人
// 当时的作用是想验证在上位机给机器人人为的发送笛卡尔空间速度指令，观察其运动效果。
// 实际效果显示机器人可以很好的完成上位机发送的速度指令，基本上可以排除系统中机械臂的问题。

#include "ros/ros.h"
#include "iostream"
#include "fstream"
#include "sstream"
#include "string"
#include "iomanip"
#include "cassert"
#include "emg/IMU_sEMG.h"
#include "emg/imu.h"

using namespace std;
//实际的txt文件的行数
const int lines = 1000;

ros::Publisher pub2;
ros::Publisher pub;
float IMU_dataxx[lines];
float IMU_datayy[lines];
float IMU_datazz[lines];
int sEMG_data_flag[lines];
            

class emg_processing
{
public:
    emg_processing(ros::NodeHandle& nh)
    {
        pub = nh.advertise<emg::IMU_sEMG>("emg_data",1000);
        //pub2 = nh.advertise
        readtxt("/home/leon/emg_project/src/emg/data/line.txt");
        sendmessage(IMU_dataxx,lines,IMU_datayy,lines,IMU_datazz,lines,sEMG_data_flag,lines);
    }
private:
    void readtxt(string);
    void sendmessage(float*,int,float*,int,float*,int,int*,int = 0);
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "reading_file");
    ros::NodeHandle nh;
    emg_processing emg(nh); 
    ros::spin();
    return 0;
}

void emg_processing::sendmessage(float* IMU_dataxx,int nx,float* IMU_datayy,int ny,float* IMU_datazz,int nz,int* sEMG_data_flag,int nflag)
{
    emg::IMU_sEMG emgdata;
    int count = 0;
    ros::Rate loop_rate(100);
    while (ros::ok())
    {
        if(count == nx)
        {
            break;
        }
        emgdata.IMU_datax = IMU_dataxx[count];
        emgdata.IMU_datay = IMU_datayy[count];
        emgdata.IMU_dataz = IMU_datazz[count];
        //emgdata.sEMG_data = sEMG_data_flag[count];
        pub.publish(emgdata);
        ROS_INFO_STREAM("RECEIVED EMG DATA IS \n"<<emgdata);
        count++;
        ros::spinOnce();
        loop_rate.sleep();
    }
}

//逐行读入数据，并且将数据转化成double型
void emg_processing::readtxt(string file)
{
    emg::IMU_sEMG emgdata;
    ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
    ROS_INFO("Reading files done!");
    string s;
    int count = 0;//计算实际的数值与读取的数值行数是否相同
    while(getline(infile,s))
    {
        istringstream istr(s);
        float x,y,z,flag;
        istr>>x>>y>>z;
        IMU_dataxx[count] = x;
        IMU_datayy[count] = y;
        IMU_datazz[count] = z;
        count++;  
    }
    ROS_INFO_STREAM("Totla lines are "<<count);
    ROS_INFO("Sending EMG Data to topic /emg_data done!");
    ROS_INFO("You can use these data to control the robot!!!");
    infile.close();             //关闭文件输入流 
}

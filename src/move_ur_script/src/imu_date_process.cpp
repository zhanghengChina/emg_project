
//这个节点的作用是读取指定位置下的txt的离线数据，并处理

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
const int lines = 37372;
float IMU_dataxx[lines];
float IMU_datayy[lines];
float IMU_datazz[lines];
            

class emg_processing
{
public:
    emg_processing(ros::NodeHandle& nh)
    {
        readtxt("/home/leon/emg_project/src/move_ur_script/motion.txt");
    }
private:
    void readtxt(string);
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "imu_data_process");
    ros::NodeHandle nh;
    emg_processing emg(nh); 
    ros::spin();
    return 0;
}

//逐行读入数据，并且将数据转化成double型
void emg_processing::readtxt(string file)
{
    using namespace std;
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
        // istr>>x>>y>>z;
        // IMU_dataxx[count] = x;
        // IMU_datayy[count] = y;
        // IMU_datazz[count] = z;
        count++;  
        if(count == 23)
            count = 0;
        if(count == 12 || count == 13 || count == 14)
         {
             cout<<s<<endl;
         }
         
    }
/*    ROS_INFO_STREAM("Totla lines are "<<count);
    ROS_INFO("Sending EMG Data to topic /emg_data done!");
    ROS_INFO("You can use these data to control the robot!!!");*/
    infile.close();             //关闭文件输入流 
}

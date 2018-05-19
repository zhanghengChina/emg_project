//产生一些指定形式的轨迹
#include "ros/ros.h"
using namespace std;
int main(int argc, char *argv[])
{
    ros::init(argc, argv, "generate_line");
    ros::NodeHandle nh;
    double x0 = 0;
    double y0 = 0;
    double z0 = 0;
    double z;
    double length_interval = 0.01;
    double times = 1000;
    for(int i = 0 ; i < times; i ++)
    {
        z = z0 + -0.3/25*(length_interval*i)*(length_interval*i)+0.12*(length_interval*i);
        cout<<x0<<" "<<y0<<" "<<z<<endl;
    }
/*    for(int i = 0; i < times ; i ++)
    {
        std::cout<<(x-length_interval*i)<<" "<<y<<" "<<z<<std::endl;
    }*/
    // length_interval = -0.001;
    // z = z + length_interval*300;
    // for(int i = 0; i < times ; i ++)
    // {
    //     std::cout<<x<<" "<<y<<" "<<(z-length_interval*i)<<std::endl;
    // }
    return 0;
}
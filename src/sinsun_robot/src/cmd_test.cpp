
// 测试机器人的控制指令
#include "ros/ros.h"
#include "string"
#include <Eigen/Geometry>  
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include "sinsun_robot/robot_state.h"
#include "std_msgs/String.h"

using namespace std;

std::string double2string(double &input);
std::string combine(Eigen::Vector3d &vel_left, Eigen::Vector3d &vel_right);
std::string convert(string& input);


int main(int argc, char *argv[])  
{  
    ros::init(argc, argv, "socket_com_test");
    ros::NodeHandle nh;

    int client_sockfd;  
    int len;  
    struct sockaddr_in remote_addr; //服务器端网络地址结构体  
    char buf[BUFSIZ];  //数据传送的缓冲区  
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零  
    remote_addr.sin_family=AF_INET; //设置为IP通信  
    remote_addr.sin_addr.s_addr=inet_addr("192.168.1.5");//服务器IP地址    192.168.8.102
    remote_addr.sin_port=htons(2000); //服务器端口号  
 
    if((client_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)  
    {  
        perror("socket");  
        return 1;  
    }  
    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)  
    {  
        perror("connect");  
        return 1;  
    }  

    ROS_INFO_STREAM("Connected to server"); 

    string input = "1.2255e-03";
    string output = convert(input);
    cout<<"shuchu: "<<output<<endl;

    double shuru = 9e-05;
    string ddddd = double2string(shuru);

    cout<<"hfjdksa:"<<ddddd<<endl;



    std::string msg;
    // 测试机器人的旋转矩阵等
    double rl[9] = {-0.6275,0.4394,-0.6428,-0.6910,0.0663,0.7198,0.3589,0.8958,0.2620};
    double rr[9] = {0.6275,0.4394,0.6428,-0.6910,-0.0663,0.7198,0.3589,-0.8958,0.2620};
    Eigen::Matrix<double,3,3>  rotation_base_to_left_arm, rotation_base_to_right_arm;
    rotation_base_to_left_arm<<rl[0],rl[1],rl[2],rl[3],rl[4],rl[5],rl[6],rl[7],rl[8];
    rotation_base_to_right_arm<<rr[0],rr[1],rr[2],rr[3],rr[4],rr[5],rr[6],rr[7],rr[8];

    ROS_INFO_STREAM("Rotation from base to left arm is:\n "<<rotation_base_to_left_arm);
    ROS_INFO_STREAM("The determinant of the rotation(left) is\n"<<rotation_base_to_left_arm.determinant());
    ROS_INFO_STREAM("Rotation from base to right arm is:\n"<<rotation_base_to_right_arm);
    ROS_INFO_STREAM("The determinant of the rotation(right) is:\n"<<rotation_base_to_right_arm.determinant());

    Eigen::Vector3d vel_left_in_base, vel_left_in_left_arm;
    Eigen::Vector3d vel_right_in_base, vel_right_in_right_arm;
    vel_left_in_base = {0,0,0};
    vel_right_in_base = {0,0.01,0};

    vel_left_in_left_arm = rotation_base_to_left_arm*vel_left_in_base;
    vel_right_in_right_arm = rotation_base_to_right_arm*vel_right_in_base;

    cout<<"vel_left\n"<<vel_left_in_left_arm<<endl;
    cout<<"vel_right\n"<<vel_right_in_right_arm<<endl;


    //ROS_INFO_STREAM("The speed of the robot is \n"<<vel_in_left_arm);
    msg = combine(vel_left_in_left_arm,vel_right_in_right_arm);

    ROS_INFO_STREAM("Sending Msg Is "<<msg);

    strcpy(buf,msg.c_str());
    send(client_sockfd,buf,strlen(buf),0);
    ros::Duration(3.0).sleep();

    msg = "speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);";
    strcpy(buf,msg.c_str());
    send(client_sockfd,buf,strlen(buf),0);
    ros::Duration(0.5).sleep();
    close(client_sockfd);//关闭套接字  
    return 0;  
}  




std::string combine(Eigen::Vector3d &vel_left, Eigen::Vector3d &vel_right)
{
    std::string msg, mmm, nnn;
    msg = msg + "speedl([";
    for(int i = 0 ; i < 3 ; i ++)
    {
        
        if(fabs(vel_left[i])<1e-4)
        {
            
            if(fabs(vel_left[i])<1e-6)
            {
                vel_left[i] = 0;
            }
            else
            {
                mmm = double2string(vel_left[i]);
                nnn = convert(mmm);
            }
        }
        else
        {
            nnn = double2string(vel_left[i]);
        }

        msg = msg + nnn;
        msg = msg + ",";
    }
    msg = msg + "0,0,0],2,0);$$$speedl([";
    for(int i = 0 ; i < 3 ; i ++)
    {
        if(fabs(vel_right[i])<1e-4)
        {
            if(fabs(vel_right[i])<1e-6)
            {
                vel_right[i] = 0;
            }
            else
            {
                mmm = double2string(vel_right[i]);
                nnn = convert(mmm);
            }
        }
        else
        {
            nnn = double2string(vel_right[i]);
        }
        msg = msg + nnn;
        msg = msg + ",";
    }
    msg = msg + "0,0,0],2,0);";
    return msg;
}

std::string double2string(double &input)
{
    std::string string_temp;
    std::stringstream stream;
    stream<<input;
    string_temp = stream.str();
    return string_temp;
}



string convert(string& input)
{
    int dot_flag = 1000;
    int e_flag = 1000;
    int minus_flag = 1000;

    for(int i = 0; i < input.length(); i ++)
    {
        if(input[i] == '.')
            dot_flag = i;
        if(input[i] == 'e')
            e_flag = i;
        if(input[i] == '-' && i == 0)
            minus_flag = i;
    }
    


    char a = input[input.length()-1];
    int count = int(a) - 48;

    string output;
    if(minus_flag == 0)
    {
        output = "-";
    }
    if(1)
    {
        output = output + "0.";
        for(int i = 1 ; i < count ; i ++)
        {
            output = output + "0";
        }
        output = output + input[dot_flag-1];
        for(int i = dot_flag + 1; i < e_flag ; i ++)
        {
            output = output + input[i];
        }
    }
    return output;
}
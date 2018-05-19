
// 从新松机器人获取信息，包括关节角度，末端位置等信息
// 发送出去的末端位置都是经过旋转变换之后的了。

#include "ros/ros.h"
#include "sinsun_robot/robot_state.h"
#include "std_msgs/String.h"
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <Eigen/Geometry>  


using namespace std;
#define PI 3.14159265354
typedef struct
{
    double currentFlangePostion[6];
    double currentFlangePostionrRight[6];
    double axisCurrentAngle[7];
    double axisCurrentAngleRight[7];
    bool externalIORead[8];
    char robotCurrentState;
} Status_Struct;

std::string combine(double *axisCurrentAngle,int n, double *axisCurrentAngleRight,int m);
std::string double2string(double &input);

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
    remote_addr.sin_addr.s_addr=inet_addr("192.168.1.5");//服务器IP地址  
    remote_addr.sin_port=htons(2001); //服务器端口号  
 
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
    Status_Struct robot_state;
    printf("connected to server\n");  
    
    ros::Publisher pub = nh.advertise<std_msgs::String>("hdh_msgs", 1000);
    ros::Publisher pub2 = nh.advertise<sinsun_robot::robot_state>("robot_state", 1000);
    sinsun_robot::robot_state robot_flange_pose;


    double rl[9] = {-0.6275,0.4394,-0.6428,-0.6910,0.0663,0.7198,0.3589,0.8958,0.2620};
    double rr[9] = {0.6275,0.4394,0.6428,-0.6910,-0.0663,0.7198,0.3589,-0.8958,0.2620};
    Eigen::Matrix<double,3,3>  rotation_base_to_left_arm, rotation_base_to_right_arm;
    rotation_base_to_left_arm<<rl[0],rl[1],rl[2],rl[3],rl[4],rl[5],rl[6],rl[7],rl[8];
    rotation_base_to_right_arm<<rr[0],rr[1],rr[2],rr[3],rr[4],rr[5],rr[6],rr[7],rr[8];
    ROS_INFO_STREAM("Rotation from base to left arm is:\n "<<rotation_base_to_left_arm);
    ROS_INFO_STREAM("The determinant of the rotation(left) is\n"<<rotation_base_to_left_arm.determinant());
    ROS_INFO_STREAM("Rotation from base to right arm is:\n"<<rotation_base_to_right_arm);
    ROS_INFO_STREAM("The determinant of the rotation(right) is:\n"<<rotation_base_to_right_arm.determinant());
    Eigen::Vector3d pos_left_in_base, pos_left_in_left_arm;
    Eigen::Vector3d pos_right_in_base, pos_right_in_right_arm;
    
    ros::Rate loop_rate(50);
    while (ros::ok())
    {
        // 将原始的数据清除
        robot_flange_pose.currentFlangePostion.clear();
        robot_flange_pose.currentFlangePostionRight.clear();
        robot_flange_pose.axisCurrentAngle.clear();
        robot_flange_pose.axisCurrentAngleRight.clear();

        len=recv(client_sockfd,&robot_state,BUFSIZ,0);//接收服务器端信息 
        cout<<"左臂关节位置"<<endl;
        for(int i = 0 ; i < 7 ; i ++)
        {
            cout<<robot_state.axisCurrentAngle[i]<<" ";
            robot_flange_pose.axisCurrentAngle.push_back(robot_state.axisCurrentAngle[i]);
        }
        cout<<endl<<endl;
        
        cout<<"左臂末端位置"<<endl;
        for(int i = 0 ; i < 6 ; i ++)
        {
            cout<<robot_state.currentFlangePostion[i]<<" ";
            // robot_flange_pose.currentFlangePostion.push_back(robot_state.currentFlangePostion[i]/1000);
        }

        cout<<"右臂关节位置"<<endl;
        for(int i = 0 ; i < 7 ; i ++)
        {
            cout<<robot_state.axisCurrentAngleRight[i]<<" ";
            robot_flange_pose.axisCurrentAngleRight.push_back(robot_state.axisCurrentAngleRight[i]);
        }
        cout<<endl<<endl;
        
        cout<<"右臂末端位置"<<endl;
        for(int i = 0 ; i < 6 ; i ++)
        {
            cout<<robot_state.currentFlangePostionrRight[i]<<" ";
            // robot_flange_pose.currentFlangePostionRight.push_back(robot_state.currentFlangePostionrRight[i]/1000);
        }

        if(1)
        {
            // 这样传送的数据x,y,z就是相对于整个机器人的基坐标系而言的了
            pos_left_in_left_arm[0] = robot_state.currentFlangePostion[0]/1000;
            pos_left_in_left_arm[1] = robot_state.currentFlangePostion[1]/1000;
            pos_left_in_left_arm[2] = robot_state.currentFlangePostion[2]/1000;
            pos_left_in_base = rotation_base_to_left_arm.inverse()*pos_left_in_left_arm;
            robot_flange_pose.currentFlangePostion.push_back(pos_left_in_base[0]);
            robot_flange_pose.currentFlangePostion.push_back(pos_left_in_base[1]);
            robot_flange_pose.currentFlangePostion.push_back(pos_left_in_base[2]);
            robot_flange_pose.currentFlangePostion.push_back(0);
            robot_flange_pose.currentFlangePostion.push_back(0);
            robot_flange_pose.currentFlangePostion.push_back(0);


            pos_right_in_right_arm[0] = robot_state.currentFlangePostionrRight[0]/1000;
            pos_right_in_right_arm[1] = robot_state.currentFlangePostionrRight[1]/1000;
            pos_right_in_right_arm[2] = robot_state.currentFlangePostionrRight[2]/1000;
            pos_right_in_base = rotation_base_to_right_arm.inverse()*pos_right_in_right_arm;

            robot_flange_pose.currentFlangePostionRight.push_back(pos_right_in_base[0]);
            robot_flange_pose.currentFlangePostionRight.push_back(pos_right_in_base[1]);
            robot_flange_pose.currentFlangePostionRight.push_back(pos_right_in_base[2]);
            robot_flange_pose.currentFlangePostionRight.push_back(0);
            robot_flange_pose.currentFlangePostionRight.push_back(0);
            robot_flange_pose.currentFlangePostionRight.push_back(0);
        }

        cout<<endl<<endl;
        std_msgs::String ros_msg;
        ros_msg.data = combine(robot_state.axisCurrentAngle,7,robot_state.axisCurrentAngleRight,7);
        pub.publish(ros_msg);
        pub2.publish(robot_flange_pose);
        ros::spinOnce();
        loop_rate.sleep();
    }
    ros::Duration(0.5).sleep();
    close(client_sockfd);//关闭套接字  
    return 0;  
}
std::string combine(double *axisCurrentAngle,int n, double *axisCurrentAngleRight,int m)
{
    std::string msg;
    for(int left = 0 ; left < n; left++)
    {
        msg = msg + double2string(axisCurrentAngle[left]);
        msg = msg + ",";
    }
    for(int right = 0 ; right < m ; right ++)
    {
        if(right == m-1)
        {
            msg = msg + double2string(axisCurrentAngleRight[right]);
        }
        else
        {
            msg = msg + double2string(axisCurrentAngleRight[right]);
            msg = msg + ",";
        }
    }
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
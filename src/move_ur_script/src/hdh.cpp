
//将实时的机器人的关节数据发送给郝德宏

#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include "moveit/move_group_interface/move_group.h"
#include "moveit/robot_model_loader/robot_model_loader.h"
#include "moveit/robot_model/robot_model.h"
#include "moveit/robot_state/robot_state.h"
#include "geometry_msgs/Pose.h"
#define PI 3.14159265354

std::string combine(std::vector<double>& joint_values); 
std::string double2string(double &input);


int main(int argc, char *argv[])  
{  
    ros::init(argc, argv, "hdh");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh;
    
    moveit::planning_interface::MoveGroup group("manipulator");
    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    moveit::core::RobotState kinematic_state(*group.getCurrentState());
    const robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("manipulator");

    geometry_msgs::Pose pose;
    std::vector<double> joint_values;
    joint_values.resize(6);
    // ros::Rate loop_rate(1);
    // while (ros::ok())
    // {
    //     pose = group.getCurrentPose(group.getEndEffectorLink()).pose;
    //     ROS_INFO_STREAM("Current Pose is "<<pose);
    //     ros::spinOnce();
    //     loop_rate.sleep();
    // }
    

    int client_sockfd;  
    int len;  
    struct sockaddr_in remote_addr; //服务器端网络地址结构体  
    char buf[BUFSIZ];  //数据传送的缓冲区  
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零  
    remote_addr.sin_family=AF_INET; //设置为IP通信  
    remote_addr.sin_addr.s_addr=inet_addr("192.168.8.103");//服务器IP地址  
    remote_addr.sin_port=htons(9695); //服务器端口号  
 
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
    std::string msg;

    ros::Rate loop_rate(50);
    while (ros::ok())
    {
        pose = group.getCurrentPose(group.getEndEffectorLink()).pose;
        kinematic_state.setFromIK(joint_model_group,pose);
        kinematic_state.copyJointGroupPositions(joint_model_group,joint_values);
        ROS_INFO_STREAM("current Pose"<<pose);

        msg = combine(joint_values);
        ROS_INFO_STREAM("SENDING MSG IS "<<msg);
        strcpy(buf,msg.c_str());
        len=send(client_sockfd,buf,strlen(buf),0);
        ros::spinOnce();
        loop_rate.sleep();
    }
    close(client_sockfd);//关闭套接字  
    return 0;  
}  

std::string combine(std::vector<double>& joint_values)
{
    for(int i = 0 ; i < 6 ; i ++)
    {
        joint_values[i] = joint_values[i]*180/PI;
    }
    std::string msg;
    msg = double2string(joint_values[0]) + ",";
    msg = msg + double2string(joint_values[1]) + ",";
    msg = msg + double2string(joint_values[2]) + ",";
    msg = msg + double2string(joint_values[3]) + ",";
    msg = msg + double2string(joint_values[4]) + ",";
    msg = msg + double2string(joint_values[5]);
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
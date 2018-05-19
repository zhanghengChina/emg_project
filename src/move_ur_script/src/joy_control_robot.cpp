
//通过手柄控制机械臂

#include <stdio.h>  
#include "math.h"
#include "sstream"
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include "ros/ros.h"
#include "string"
#include "moveit/move_group_interface/move_group.h"
#include "moveit/robot_model_loader/robot_model_loader.h"
#include "moveit/robot_model/robot_model.h"
#include "moveit/robot_state/robot_state.h"
#include "geometry_msgs/Pose.h"
#include "std_msgs/String.h"
#include <control_msgs/FollowJointTrajectoryAction.h>
#include "control_msgs/FollowJointTrajectoryActionGoal.h"
#include <actionlib/client/simple_action_client.h>
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"



typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> Client;

class move_robot
{//定义一个和运动相关的类，主要用来获取当前的位恣
public:
    move_robot():group("manipulator"),robot_model_loader("robot_description"),kinematic_model(robot_model_loader.getModel()),
                kinematic_state(*group.getCurrentState()),joint_model_group(kinematic_model->getJointModelGroup("manipulator"))
    {
        Client ac("follow_joint_trajectory", true);
        if (!ac.waitForServer(ros::Duration(2.0)))
        {
            ROS_ERROR("Could not connect to action server");
            exit(-1);
        }
        else
        {
            ROS_INFO("Connecte with UR10 Now!");
        }
        group.setMaxVelocityScalingFactor(0.3);
        group.setMaxAccelerationScalingFactor(0.2);
        group.setNamedTarget("look");
        group.move();
        ros::Duration(0.5).sleep();

    }
    moveit::planning_interface::MoveGroup group;
    robot_model_loader::RobotModelLoader robot_model_loader;
    robot_model::RobotModelPtr kinematic_model;
    moveit::core::RobotState kinematic_state;
    const robot_state::JointModelGroup* joint_model_group;

};

class TCPIPAPI
{//定义一个tcpip类，主要是用来做socket的初始化，用来发送数据接受数据   
public:
    TCPIPAPI()
    {
        memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零  
        remote_addr.sin_family=AF_INET; //设置为IP通信  
        remote_addr.sin_addr.s_addr=inet_addr("192.168.0.105");//服务器IP地址  
        remote_addr.sin_port=htons(30003); //服务器端口号 
        if((client_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)  
        {  
            perror("socket");  
            exit(-1);  
        } 
        if((client_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)  
        {  
            perror("socket");  
            exit(-1);
        }
        if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)  
        {  
            perror("connect");  
            exit(-1);  
        }  
        ROS_INFO("CONNECTED TO SERVER");
    }
    void close_socket()
    {
        close(client_sockfd);
        ROS_INFO("SOCKET CLOSED!");
    } 
    int send_msg(std::string& msg)
    {
        strcpy(buf,msg.c_str());
        len = send(client_sockfd,buf,strlen(buf),0);
        return len;
    }
    char* recv_msg(void)
    {
        len = recv(client_sockfd,buf,BUFSIZ,0);
        return buf;
    }
    //全局变量
    int client_sockfd;  
    int len;  
    struct sockaddr_in remote_addr; //服务器端网络地址结构体  
    char buf[BUFSIZ];  //数据传送的缓冲区  
    

};

class Joy
{
public:
    Joy()
    {
        eef_vel.resize(6);
        eef_vel = {0,0,0,0,0,0};
        acc = 1.0;
        default_vel = 0.1;
        sub = nh.subscribe("joy", 1000, &Joy::subCallback,this);
        ros::Duration(1.0).sleep();
        joy_msg.buttons.resize(12);
        joy_msg.axes.resize(7);
    }
    void start()
    {
        while(ros::ok())
        {
            //x方向
            if(joy_msg.buttons[0] == 1 && joy_msg.buttons[2] == 0)
            {
                eef_vel[0] = default_vel;
            }
            else
            {
                if(joy_msg.buttons[2] == 1 && joy_msg.buttons[0] == 0)
                {
                    eef_vel[0] = -default_vel;
                }
                else
                {
                    eef_vel[0] = 0;
                }
            }

            //y方向
            if(joy_msg.buttons[1] == 1 && joy_msg.buttons[3] == 0)
            {
                eef_vel[1] = default_vel;
            }
            else
            {
                if(joy_msg.buttons[3] == 1 && joy_msg.buttons[1] == 0)
                {
                    eef_vel[1] = -default_vel;
                }
                else
                {
                    eef_vel[1] = 0;
                }
            }

            //z方向
            if(joy_msg.buttons[4] == 1 && joy_msg.buttons[5] == 0)
            {
                eef_vel[2] = default_vel;
            }
            else
            {
                if(joy_msg.buttons[5] == 1 && joy_msg.buttons[4] == 0)
                {
                    eef_vel[2] = -default_vel;
                }
                else
                {
                    eef_vel[2] = 0;
                }
            }

            move_msg = combinemsg(eef_vel,acc);
            if(eef_vel[0]==0&&eef_vel[1]==0&&eef_vel[2]==0)
            {
                move_msg = "stopl(0.5)\n";
            }
            std::cout<<move_msg<<std::endl;
            tcpip.send_msg(move_msg);
            ros::Duration(0.1).sleep();
        }
    }
private:
    void subCallback(const sensor_msgs::Joy::ConstPtr& msg)
    {
        joy_msg = *msg;
    }
    std::string combinemsg(std::vector<double> &velocity, double &acc)
    {
        double time2move = 0.2;//给机器人的运动时间多一点，但是实际上并不会运动那么多就会被下一条指令覆盖
        std::string move_msg;
        move_msg = "speedl([";
        move_msg = move_msg + double2string(velocity[0]) + ",";
        move_msg = move_msg + double2string(velocity[1]) + ",";
        move_msg = move_msg + double2string(velocity[2]) + ",";
        move_msg = move_msg + double2string(velocity[3]) + ",";
        move_msg = move_msg + double2string(velocity[4]) + ",";
        move_msg = move_msg + double2string(velocity[5]) + "]";
        move_msg = move_msg + ",";
        move_msg = move_msg + double2string(acc) + ",";
        move_msg = move_msg + double2string(time2move) + ")";
        move_msg = move_msg + "\n";
        return move_msg; 
    }

    std::string double2string(double &input)
    {
        std::string string_temp;
        std::stringstream stream;
        stream<<input;
        string_temp = stream.str();
        return string_temp;
    }

    ros::Subscriber sub;
    ros::NodeHandle nh;
    TCPIPAPI tcpip;
    move_robot move;
    std::vector<double> eef_vel;
    sensor_msgs::Joy joy_msg;
    double acc;
    double default_vel;
    std::string move_msg;
};

int main(int argc, char *argv[])  
{  
    ros::init(argc, argv, "JoyControlRobot");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    Joy joy; 
    joy.start();
    return 0;  
}  

//通过ur_script的函数来直接在笛卡尔空间规划，效果比较好

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
#include "emg/IMU_sEMG.h"
#include "std_msgs/Float64.h"
#include "move_ur_script/joint_value_target.h"



static geometry_msgs::Pose pose_now,pose_target_true,pose_target,pose_reference;
typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> Client;
static emg::IMU_sEMG global_msg;

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
        // group.setMaxAccelerationScalingFactor(0.2);
        // group.setNamedTarget("arm_band_pose"); 
        group.setNamedTarget("arm_pose");   // 拍摄VR时候的位置
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

//定义一个和臂带相关的类,这个类是使用过程中最主要的类
class emg_class
{
public:
    emg_class()
    {
        emg_data_flag = 0;
        max_acc = 1.2;
        eef_vel.resize(6);
        max_vel.resize(3);
        pid.resize(3);
        //指定关节角速度极限
        pid = {5,5,5};
        max_vel = {0.8,0.8,0.8};
    }
    void start();
private:
    void subCallback(const emg::IMU_sEMG::ConstPtr& msg)
    {

/*        double beishu = 2.0;
        if(emg_data_flag == 0) 
        {
            //相当于这个数据是一个总的参考点
            global_msg.IMU_datax = msg->IMU_datax;
            global_msg.IMU_datay = msg->IMU_datay;
            global_msg.IMU_dataz = msg->IMU_dataz;
            emg_data_flag = 1;
        }
        pose_target_true.position.x = pose_reference.position.x - (msg->IMU_datax - global_msg.IMU_datax)/beishu;
        pose_target_true.position.y = pose_reference.position.y - (msg->IMU_datay - global_msg.IMU_datay)/beishu;
        pose_target_true.position.z = pose_reference.position.z + (msg->IMU_dataz - global_msg.IMU_dataz)/beishu;*/

        double beishu = 3.0;
        if(emg_data_flag == 0) 
        {
            //相当于这个数据是一个总的参考点
            global_msg.IMU_datax = msg->IMU_datax;
            global_msg.IMU_datay = msg->IMU_datay;
            global_msg.IMU_dataz = msg->IMU_dataz;
            emg_data_flag = 1;
        }
        double delta_x = (msg->IMU_datax - global_msg.IMU_datax)/beishu;
        double delta_y = (msg->IMU_datay - global_msg.IMU_datay)/beishu;
        pose_target_true.position.x = pose_reference.position.x + delta_y;
        pose_target_true.position.y = pose_reference.position.y - delta_x;
        pose_target_true.position.z = pose_reference.position.z + (msg->IMU_dataz - global_msg.IMU_dataz)/beishu;




/*        double beishu = 2.5;
        if(emg_data_flag == 0) 
        {
            //相当于这个数据是一个总的参考点
            global_msg.IMU_datax = msg->IMU_datax;
            global_msg.IMU_datay = msg->IMU_datay;
            global_msg.IMU_dataz = msg->IMU_dataz;
            emg_data_flag = 1;
        }
        double delta_x = (msg->IMU_datay - global_msg.IMU_datay)/beishu;
        double delta_y = (msg->IMU_datax - global_msg.IMU_datax)/beishu;
        pose_target_true.position.x = pose_reference.position.x + delta_x;
        pose_target_true.position.y = pose_reference.position.y + delta_y;
        pose_target_true.position.z = pose_reference.position.z + (msg->IMU_dataz - global_msg.IMU_dataz)/beishu;*/
        //因为测试的时候z方向是反的。所以暂时先加一个负号
        //相当于把之前的增量加在上面了
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
    //计算速度的一个函数

    ros::Subscriber sub;//监听emg数据

    //发送理论的关节数据
    ros::Publisher  pub; // 目标位置
    ros::Publisher  pub2;//当前位置
    ros::NodeHandle nh;
    TCPIPAPI tcpip;
    move_robot move;

    int emg_data_flag;
    std::vector<double> eef_vel;
    double max_acc;
    std::vector<double> max_vel;
    std::vector<double> pid;
};


void emg_class::start()
{
    pub = nh.advertise<move_ur_script::joint_value_target>("pose_target", 1000);
    pub2 = nh.advertise<move_ur_script::joint_value_target>("pose_now", 1000);

    pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;
    pose_reference = pose_target = pose_target_true = pose_now;
    ROS_INFO_STREAM("Init Pose Is "<<pose_now);

    std::string move_msg;//发送关于ur_script的消息

    

    //定义两个用来显示关节角度的消息。应该还需要修改
    move_ur_script::joint_value_target pose_target_topic;
    move_ur_script::joint_value_target pose_now_topic;

    for(int i = 0 ; i < 6 ; i ++)
    {
        eef_vel[i] = 0;
    }

    sub = nh.subscribe<emg::IMU_sEMG>("emg_data", 1000, &emg_class::subCallback,this);//1000是缓冲区的大小
    ros::Duration(1.0).sleep();

    ros::Time time_init = ros::Time::now();
    //Test the pid value of every direction
    /*while(ros::ok())
    {
        pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;
        ros::Duration interval = ros::Time::now() - time_init;
        double interval2double = interval.toSec();

        pose_target.position.y = pose_reference.position.y + 0.1*sin(interval2double);
        
        eef_vel[0] = 5*(pose_target.position.x - pose_now.position.x);
        eef_vel[1] = 5*(pose_target.position.y - pose_now.position.y);
        eef_vel[2] = 5*(pose_target.position.z - pose_now.position.z);
        for(int i = 0 ; i < 3; i ++)
        {
            if(eef_vel[i]>0.8)
                eef_vel[i] = 0.8;
            if(eef_vel[i]<(-0.8))
                eef_vel[i] = -0.8;
        }
        eef_vel[3] = eef_vel[4] = eef_vel[5] = 0;

        pose_target_topic.joint_value.clear();
        pose_now_topic.joint_value.clear();
        pose_now_topic.joint_value.push_back(pose_now.position.x);
        pose_now_topic.joint_value.push_back(pose_now.position.y);
        pose_now_topic.joint_value.push_back(pose_now.position.z);
        pose_target_topic.joint_value.push_back(pose_target.position.x);
        pose_target_topic.joint_value.push_back(pose_target.position.y);
        pose_target_topic.joint_value.push_back(pose_target.position.z);
        pub.publish(pose_target_topic);
        pub2.publish(pose_now_topic);

        //Sending ur_script message
        move_msg = combinemsg(eef_vel,max_acc);
        ROS_INFO_STREAM("Sending msg "<<move_msg);
        tcpip.send_msg(move_msg);
        ros::Duration(0.05).sleep();
    }*/

    while(ros::ok())//让机械臂运动的主要循环
    {
        if(emg_data_flag == 1)
        {
            ROS_INFO_STREAM("Current Pose"<<pose_now);
            ROS_INFO_STREAM(" Pose"<<pose_target);
            while(ros::ok())//防止下面程序执行完之后emg数据还没更新
            {
                double error_x = pose_target.position.x - pose_target_true.position.x;
                double error_y = pose_target.position.y - pose_target_true.position.y;
                double error_z = pose_target.position.z - pose_target_true.position.z;
                if(fabs(error_x)>0.0000005||fabs(error_y)>0.0000005||fabs(error_z) > 0.0000005) 
                {
                    ROS_INFO("Reveiving New Data Now!");
                    //这个循环一般都不会死掉
                    break;
                }
            }
            //update new pose
            pose_target = pose_target_true;
            if(move.kinematic_state.satisfiesBounds())
            {
                pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;

                eef_vel[0] = pid[0] * (pose_target.position.x - pose_now.position.x);
                eef_vel[1] = pid[1] * (pose_target.position.y - pose_now.position.y);
                eef_vel[2] = pid[2] * (pose_target.position.z - pose_now.position.z);
                for(int i = 0 ; i < 3; i ++)
                {
                    if(eef_vel[i]>max_vel[i])
                        eef_vel[i] = max_vel[i];
                    if(eef_vel[i]<(-max_vel[i]))
                        eef_vel[i] = -max_vel[i];
                }

                //publish target pose and current pose
                pose_target_topic.joint_value.clear();
                pose_now_topic.joint_value.clear();
                pose_now_topic.joint_value.push_back(pose_now.position.x);
                pose_now_topic.joint_value.push_back(pose_now.position.y);
                pose_now_topic.joint_value.push_back(pose_now.position.z);
                pose_target_topic.joint_value.push_back(pose_target.position.x);
                pose_target_topic.joint_value.push_back(pose_target.position.y);
                pose_target_topic.joint_value.push_back(pose_target.position.z);
                pub.publish(pose_target_topic);
                pub2.publish(pose_now_topic);

                //Sending ur_script message
                move_msg = combinemsg(eef_vel,max_acc);
                ROS_INFO_STREAM("Sending msg "<<move_msg);
                tcpip.send_msg(move_msg);
            }
        }
        else
        {
            ROS_WARN("WAITING FOR EMG DATA!");
            ros::Duration(0.2).sleep();
        }
    }
}

int main(int argc, char *argv[])  
{  
    ros::init(argc, argv, "cpptcpip");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    emg_class EMG;   
    EMG.start(); 
    return 0;  
}  
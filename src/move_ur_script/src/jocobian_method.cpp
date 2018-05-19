
//目前这个数据还存在问题，不清楚是思路问题还是中间处理的细节的问题

#include <stdio.h>  
#include "math.h"
#include "sstream"
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include "ros/ros.h"
#include "string"
#include <Eigen/Core>  
#include <Eigen/Geometry>  
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
        group.setMaxAccelerationScalingFactor(0.2);
        group.setNamedTarget("look");
        group.move();
        ros::Duration(2).sleep();

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
        remote_addr.sin_addr.s_addr=inet_addr("192.168.1.101");//服务器IP地址  
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
        error = 0;
        emg_data_flag = 0;
        max_acc = 1.4;
        joint_values_now.resize(6);
        joint_values_target.resize(6);
        joint_velocities_now.resize(6);
        eef_velocities.resize(6);
        max_vel.resize(6);
        max_vel_eef.resize(3);
        pid.resize(6);
        //指定关节角速度极限
        pid = {3.5,3.5,3.5,3.5,4,4.5};
        max_vel = {2.09,2.09,3.14,3.14,3.14,3.14};
        max_vel_eef = {0.3 , 0.3 , 0.3};
    }
    void start();
private:
    void subCallback(const emg::IMU_sEMG::ConstPtr& msg)
    {
        if(emg_data_flag == 0) 
        {
            //相当于这个数据是一个总的参考点
            global_msg.IMU_datax = msg->IMU_datax;
            global_msg.IMU_datay = msg->IMU_datay;
            global_msg.IMU_dataz = msg->IMU_dataz;
            emg_data_flag = 1;
        }
        pose_target_true.position.x = pose_reference.position.x + (msg->IMU_datax - global_msg.IMU_datax);
        pose_target_true.position.y = pose_reference.position.y + (msg->IMU_datay - global_msg.IMU_datay);
        pose_target_true.position.z = pose_reference.position.z + (msg->IMU_dataz - global_msg.IMU_dataz);
        //相当于把之前的增量加在上面了
    }
    std::string combinemsg(std::vector<double> &velocity, double &acc)
    {
        double time2move = 0.5;//给机器人的运动时间多一点，但是实际上并不会运动那么多就会被下一条指令覆盖
        std::string move_msg;
        move_msg = "speedj([";
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
    void GetProperSpeed2();

    ros::Subscriber sub;//监听emg数据

    //发送理论的关节数据
    ros::Publisher  pub; // 关节位置
    ros::Publisher  pub2;//关节速度
    ros::NodeHandle nh;
    TCPIPAPI tcpip;
    move_robot move;

    double error;
    int emg_data_flag;
    std::vector<double> joint_values_now;
    std::vector<double> joint_values_target;
    std::vector<double> joint_velocities_now;
    std::vector<double> eef_velocities;
    double max_acc;
    std::vector<double> max_vel;
    std::vector<double> max_vel_eef;
    std::vector<double> pid;
};


void emg_class::start()
{
    pub = nh.advertise<move_ur_script::joint_value_target>("pose_target", 1000);
    pub2 = nh.advertise<move_ur_script::joint_value_target>("joint_value_target", 1000);

    pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;
    pose_reference = pose_now; pose_target = pose_now;  pose_target_true = pose_now;
    ROS_INFO_STREAM("Init Pose Is "<<pose_now);

    std::string move_msg;//发送关于ur_script的消息
    ros::Time time_init = ros::Time::now();

    //定义两个用来显示关节角度的消息。应该还需要修改
    move_ur_script::joint_value_target pose_target_topic;
    move_ur_script::joint_value_target joint_value_topic;

    Eigen::Vector3d workspace_error;
    double error_norm;
    Eigen::MatrixXd jacobian_now;
    Eigen::MatrixXd jacobian_inverse;
    Eigen::Matrix<double,6,1> eef_velocities_matrix;
    Eigen::Matrix<double,6,1> joint_velocities_matrix;

    for(int i = 0 ; i < 6 ; i ++)
    {
        joint_velocities_now[i] = 0;
    }

    sub = nh.subscribe<emg::IMU_sEMG>("emg_data", 1000, &emg_class::subCallback,this);//1000是缓冲区的大小
    ros::Duration(1.0).sleep();

    //获取Jacobian矩阵
    // Eigen::MatrixXd jacobian = move.kinematic_state.getJacobian(move.joint_model_group);
    // ROS_INFO_STREAM("Jacobian matrix is \n"<<jacobian);

    while(ros::ok())//让机械臂运动的主要循环
    {
        if(emg_data_flag == 1)
        {
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
            //更新目标位恣
            pose_target = pose_target_true;
            pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;
            jacobian_now = move.kinematic_state.getJacobian(move.joint_model_group);
            workspace_error<<(pose_target.position.x - pose_now.position.x),(pose_target.position.y - pose_now.position.y),(pose_target.position.z - pose_now.position.z);
            error_norm = workspace_error.norm();

            eef_velocities[0] = max_vel_eef[0] * workspace_error[0]/error_norm;
            eef_velocities[1] = max_vel_eef[1] * workspace_error[1]/error_norm;
            eef_velocities[2] = max_vel_eef[2] * workspace_error[2]/error_norm;

            //末端的速度

            eef_velocities[3] = eef_velocities[4] = eef_velocities[5] = 0;
            //角速度保持恒定值
            eef_velocities_matrix<<eef_velocities[0],eef_velocities[1],eef_velocities[2],eef_velocities[3],eef_velocities[4],eef_velocities[5];
            jacobian_now = move.kinematic_state.getJacobian(move.joint_model_group);
            // jacobian_inverse = jacobian_now.transpose()*(jacobian_now*jacobian_now.transpose()).inverse();
            // joint_velocities_matrix = jacobian_inverse*eef_velocities_matrix;
            joint_velocities_matrix = jacobian_now.inverse() *eef_velocities_matrix;                        
            ROS_INFO_STREAM("The initial joint velocities are "<<joint_velocities_matrix);

            for(int i = 0 ; i < 6 ; i ++)
            {
                joint_velocities_now[i] = joint_velocities_matrix[i];

                if(joint_velocities_now[i]>max_vel[i])
                {
                    joint_velocities_now[i] = max_vel[i];
                }
                if(joint_velocities_now[i]<(-max_vel[i]))
                {
                    joint_velocities_now[i] = -max_vel[i];
                }
            }
            move_msg = combinemsg(joint_velocities_now,max_acc);
            tcpip.send_msg(move_msg);
            ROS_INFO_STREAM("Sending message is "<<move_msg);

            pose_target_topic.joint_value.clear();
            joint_value_topic.joint_value.clear();

            // joint_value_topic.joint_value.push_back(joint_values_target[0]);
            // joint_value_topic.joint_value.push_back(joint_values_target[1]);
            // joint_value_topic.joint_value.push_back(joint_values_target[2]);

            pose_target_topic.joint_value.push_back(pose_target.position.x);
            pose_target_topic.joint_value.push_back(pose_now.position.z);
            //只是测试了Z方向的
            pose_target_topic.joint_value.push_back(pose_target.position.z);
            pub.publish(pose_target_topic);
            pub2.publish(joint_value_topic);


            // move.kinematic_state.setFromIK(move.joint_model_group,pose_target);
            // move.kinematic_state.copyJointGroupPositions(move.joint_model_group,joint_values_target);



            // pose_target = pose_target_true;
            // pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;
            // move.kinematic_state.setFromIK(move.joint_model_group,pose_now);
            // move.kinematic_state.copyJointGroupPositions(move.joint_model_group,joint_values_now);
            // //当前的关节角度

            // move.kinematic_state.setFromIK(move.joint_model_group,pose_target);
            // move.kinematic_state.copyJointGroupPositions(move.joint_model_group,joint_values_target);
            // //目标关节角度

            // //PID调整模块，得到的是各个关节的速度
            // for(int i = 0 ; i < 6 ; i ++)
            // {
            //     joint_velocities_now[i] = pid[i]*(joint_values_target[i] - joint_values_now[i]);
            //     if(joint_velocities_now[i]>max_vel[i])
            //     {
            //         joint_velocities_now[i] = max_vel[i];
            //     }
            //     if(joint_velocities_now[i]<(-max_vel[i]))
            //     {
            //         joint_velocities_now[i] = -max_vel[i];
            //     }
            // }

            // pose_target_topic.joint_value.clear();
            // joint_value_topic.joint_value.clear();

            // joint_value_topic.joint_value.push_back(joint_values_target[0]);
            // joint_value_topic.joint_value.push_back(joint_values_target[1]);
            // joint_value_topic.joint_value.push_back(joint_values_target[2]);

            // pose_target_topic.joint_value.push_back(pose_target.position.x);
            // pose_target_topic.joint_value.push_back(pose_now.position.z);
            // //只是测试了Z方向的
            // pose_target_topic.joint_value.push_back(pose_target.position.z);
            // pub.publish(pose_target_topic);
            // pub2.publish(joint_value_topic);
            // move_msg = combinemsg(joint_velocities_now,max_acc);
            // ROS_INFO_STREAM("Sending msg "<<move_msg);

            // tcpip.send_msg(move_msg);
            //time_init = ros::Time::now();
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

// void emg_class::GetProperSpeed2()
// {
//     double delta[3];
//     pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;
//     pose_target = pose_target_true;

//     delta[0] = pose_target.position.x - pose_now.position.x;
//     delta[1] = pose_target.position.y - pose_now.position.y;
//     delta[2] = pose_target.position.z - pose_now.position.z;
//     for(int i = 0 ; i < 3 ; i ++)
//     {
//         acc[i] = (delta[i]-velocity_before[i]*time_interval)/(time_interval*acc_time-0.5*acc_time*acc_time);
//         velocity_now[i] = velocity_before[i] + acc[i]*acc_time;
//         acc_norm = acc_norm + acc[i]*acc[i];
//     }
//     acc_norm = sqrt(acc_norm);
//     // ROS_INFO_STREAM("acc[0] is "<<acc[0]<<"\n"<<"acc[1] is "<<acc[1]<<"\n"<<"acc[2] is "<<acc[2]);
//     // ROS_INFO_STREAM("vx is "<<velocity_now[0]<<"\n"<<"vy is "<<velocity_now[1]<<"\n"<<"vz is "<<velocity_now[2]);
//     //可能存在的问题是有很多数据严重有偏离
//     ros::Time star_time = ros::Time::now();
//     while(ros::ok())
//     {
//         if(acc_norm > max_acc||fabs(velocity_now[0])>max_vel||fabs(velocity_now[1])>max_vel||fabs(velocity_now[2])>max_vel)
//         {
//             acc_norm = 0;
//             delta[0]/=1.2;
//             delta[1]/=1.2;
//             delta[2]/=1.2;
//             for(int i = 0 ; i < 3 ; i ++)
//             {
//                 acc[i] = (delta[i]-velocity_before[i]*time_interval)/(time_interval*acc_time-0.5*acc_time*acc_time);
//                 velocity_now[i] = velocity_before[i] + acc[i]*acc_time;
//                 acc_norm = acc_norm + acc[i]*acc[i];
//             };
//             acc_norm = sqrt(acc_norm);
//         }
//         else
//         {
//             pose_now = move.group.getCurrentPose(move.group.getEndEffectorLink()).pose;
//             // pose_target_true.position.x = pose_target_true.position.x - global_msg.IMU_datax + delta[0];
//             // pose_target_true.position.y = pose_target_true.position.y - global_msg.IMU_datay + delta[1];
//             // pose_target_true.position.z = pose_target_true.position.z - global_msg.IMU_dataz + delta[2];

// //这句话可能有问题
//             pose_target_true.position.x = pose_now.position.x + delta[0];
//             pose_target_true.position.y = pose_now.position.y + delta[1];
//             pose_target_true.position.z = pose_now.position.z + delta[2];
//             ROS_INFO("Calculator finished!");
//             break;
//         }
//         if((ros::Time::now() - star_time) > ros::Duration(0.05))
//         {
//             break;
//         }
//     }
//     velocity_before = velocity_now;
// }

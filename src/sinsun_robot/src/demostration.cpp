// 这个节点的目的是为了最终控制机器人
// 因为传输过来的位置信息都是在基坐标系下面表示，除了最后转换速度的时候转换到了左右臂的坐标系下面，其余都是相对于基坐标系

#include "ros/ros.h"
#include <Eigen/Geometry>  
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include "emg/imu.h"
#include "move_ur_script/joint_value_target.h"
#include "sinsun_robot/robot_state.h"
#include "geometry_msgs/Pose.h"
#include "sensor_msgs/Joy.h"
using namespace std;

class TCPIPAPI
{//定义一个tcpip类，主要是用来做socket的初始化，用来发送数据接受数据   
public:
    TCPIPAPI()
    {
        memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零  
        remote_addr.sin_family=AF_INET; //设置为IP通信  
        remote_addr.sin_addr.s_addr=inet_addr("192.168.1.5");//服务器IP地址  
        remote_addr.sin_port=htons(2000); //服务器端口号 
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
private:
    ros::Subscriber imu_sub;                    // 监听IMU数据
    ros::Subscriber robot_state_sub;            // 监听机器人信息
    ros::Subscriber joy_sub;                    // 监听手柄信息
    ros::Publisher  target_pub;                 // 发送机器人末端目标位置
    ros::Publisher  current_pub;                // 发送机器人当前位置，当然通过robot_state_sub接受的消息也能得到这个
    ros::NodeHandle nh;

    TCPIPAPI tcpip;

    int emg_data_flag;                          // 接收到IMU信息的标志
    int robot_state_flag;                       // 接收到机器人信息的标注
    sensor_msgs::Joy joy_msg;

    double max_acc;                             // 末端最大加速度，新松工程师：5m/s^2
    std::vector<double> eef_vel_left;           // 左臂末端速度
    std::vector<double> eef_vel_right;          // 右臂末端速度
    std::vector<double> max_vel;                // 两个臂末端最大速度，新松工程师：1m/s
    std::vector<double> pid_left;               // 左臂笛卡尔空间的PID参数，还是基于每个臂的基座而言
    std::vector<double> pid_right;              // 右臂笛卡尔空间坐标系

    // 定义和机器人位置和IMU反馈信息
    sinsun_robot::robot_state global_dual_robot_state;                                                      // 机器人的状态信息
    geometry_msgs::Pose pose_now_left,pose_target_true_left,pose_target_left,pose_reference_left;           // 左臂机器人的末端位置
    geometry_msgs::Pose pose_now_right,pose_target_true_right,pose_target_right,pose_reference_right;       // 右臂机器人的末端位置
    emg::imu global_msg;                                                                               // 接收到的IMU信息


    // 和机器人相关的变换
    std::vector<double> rl, rr;                                                                             // 旋转矩阵
    Eigen::Matrix<double,3,3>  rotation_base_to_left_arm, rotation_base_to_right_arm;                       // 基座到左臂和右臂的旋转变换
    Eigen::Vector3d delta_left_in_base, delta_left_in_left_arm;                                             // 左臂末端位移增量，在基坐标系下的表示，在自身坐标系下面表示
    Eigen::Vector3d delta_right_in_base, delta_right_in_right_arm;                                          // 右臂末端位移增量，在基坐标系下的表示，在自身坐标系下面表示

public:
    emg_class()
    {
        emg_data_flag = 0;
        robot_state_flag = 0;

        max_acc = 2;

        eef_vel_left.resize(6);
        eef_vel_right.resize(6);
        max_vel.resize(3);
        pid_left.resize(3);
        pid_right.resize(3);
        global_msg.data.resize(6);

        joy_msg.buttons.resize(12);
        joy_msg.axes.resize(7);

        //指定关节角速度极限
        pid_left = {5,5,5};
        pid_right = {5,5,5};
        max_vel = {0.3,0.3,0.3};

        rl.resize(9);
        rr.resize(9);
        rl = {-0.6275,0.4394,-0.6428,-0.6910,0.0663,0.7198,0.3589,0.8958,0.2620};
        rr = {0.6275,0.4394,0.6428,-0.6910,-0.0663,0.7198,0.3589,-0.8958,0.2620};
        rotation_base_to_left_arm<<rl[0],rl[1],rl[2],rl[3],rl[4],rl[5],rl[6],rl[7],rl[8];
        rotation_base_to_right_arm<<rr[0],rr[1],rr[2],rr[3],rr[4],rr[5],rr[6],rr[7],rr[8];
        ROS_INFO_STREAM("Rotation from base to left arm is:\n "<<rotation_base_to_left_arm);
        ROS_INFO_STREAM("The determinant of the rotation(left) is\n"<<rotation_base_to_left_arm.determinant());
        ROS_INFO_STREAM("Rotation from base to right arm is:\n"<<rotation_base_to_right_arm);
        ROS_INFO_STREAM("The determinant of the rotation(right) is:\n"<<rotation_base_to_right_arm.determinant());
    }
    void start();
private:

    void joy_subCallback(const sensor_msgs::Joy::ConstPtr& msg)
    {
        joy_msg = *msg;
    }

    //机器人状态信息的回调函数
    void robot_state_subcallback(const sinsun_robot::robot_stateConstPtr& robot_state)
    {
        if(robot_state_flag == 0) robot_state_flag = 1;
        global_dual_robot_state = *robot_state;
    }

    // IMU信息的回调函数
    void subCallback(const emg::imu::ConstPtr& msg)
    {
        double beishu = 5;
        double beishu_right = 5;
        if(emg_data_flag == 0) 
        {
            //相当于这个数据是一个总的参考点
            global_msg = *msg;
            emg_data_flag = 1;
        } 

        // pose_target_true_left.position.x = pose_reference_left.position.x - (msg->data[1] - global_msg.data[1])/beishu;
        // pose_target_true_left.position.y = pose_reference_left.position.y + (msg->data[0] - global_msg.data[0])/beishu;
        // pose_target_true_left.position.z = pose_reference_left.position.z + (msg->data[2] - global_msg.data[2])/beishu;

        // // 只测试一个臂
        // pose_target_true_right.position.x = pose_reference_right.position.x - (msg->data[4] - global_msg.data[4])/beishu;
        // pose_target_true_right.position.y = pose_reference_right.position.y + (msg->data[3] - global_msg.data[3])/beishu;
        // pose_target_true_right.position.z = pose_reference_right.position.z + (msg->data[5] - global_msg.data[5])/beishu;


        pose_target_true_left.position.x = pose_reference_left.position.x - (msg->data[4] - global_msg.data[4])/beishu_right;
        pose_target_true_left.position.y = pose_reference_left.position.y + (msg->data[3] - global_msg.data[3])/beishu_right;
        pose_target_true_left.position.z = pose_reference_left.position.z + (msg->data[5] - global_msg.data[5])/beishu_right;

        pose_target_true_right.position.x = pose_reference_right.position.x - (msg->data[1] - global_msg.data[1])/beishu;
        pose_target_true_right.position.y = pose_reference_right.position.y + (msg->data[0] - global_msg.data[0])/beishu;
        pose_target_true_right.position.z = pose_reference_right.position.z + (msg->data[2] - global_msg.data[2])/beishu;
        ROS_INFO("Receiving imu data");
    }

    // 将科学计数法表示成普通的字符串形式
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

    // 消息组合函数
    std::string combinemsg(std::vector<double> &velocity,std::vector<double> velocity_right, double &acc)
    {
        double time2move = 0;
        std::string move_msg, mmm, nnn;
        move_msg = "speedl([";

        for(int i = 0 ; i < 6 ; i ++)
        {
            if(fabs(velocity[i])<1e-4)
            {
                if(fabs(velocity[i])<1e-6)
                {
                    velocity[i] = 0;
                    nnn = double2string(velocity[i]);
                }
                else
                {
                    mmm = double2string(velocity[i]);
                    nnn = convert(mmm);
                } 
            }
            else
            {
                nnn = double2string(velocity[i]);
            }

            if(i == 5)
            {
                move_msg = move_msg + nnn;
                move_msg = move_msg + "],";
            }
            else
            {
                move_msg = move_msg + nnn;
                move_msg = move_msg + ",";
            }
        }

        move_msg = move_msg + double2string(acc) + ",";
        move_msg = move_msg + double2string(time2move) + ");$$$speedl([";

        for(int i = 0 ; i < 6 ; i ++)
        {
            if(fabs(velocity_right[i])<1e-4)
            {
                if(fabs(velocity_right[i])<1e-6)
                {
                    velocity_right[i] = 0;
                    nnn = double2string(velocity_right[i]);
                }
                else
                {
                    mmm = double2string(velocity_right[i]);
                    nnn = convert(mmm);
                } 
            }
            else
            {
                nnn = double2string(velocity_right[i]);
            }

            if(i == 5)
            {
                move_msg = move_msg + nnn;
                move_msg = move_msg + "],";
            }
            else
            {
                move_msg = move_msg + nnn;
                move_msg = move_msg + ",";
            }
        }        
        move_msg = move_msg + double2string(acc) + ",";
        move_msg = move_msg + double2string(time2move);
        move_msg = move_msg + ");";
        return move_msg; 
    }

    // double类型的消息转换成字符的函数
    std::string double2string(double &input)
    {
        std::string string_temp;
        std::stringstream stream;
        stream<<input;
        string_temp = stream.str();
        return string_temp;
    }

    // 将从机器人接收到的double类型的机器人消息转换成ros类型的位置消息
    void pose_transform(sinsun_robot::robot_state& robot_state, geometry_msgs::Pose& left_pose, geometry_msgs::Pose& right_pose)
    {
        left_pose.position.x = robot_state.currentFlangePostion[0];
        left_pose.position.y = robot_state.currentFlangePostion[1];
        left_pose.position.z = robot_state.currentFlangePostion[2];
        left_pose.orientation.x = left_pose.orientation.y = left_pose.orientation.z = 0;
        left_pose.orientation.w = 1;

        right_pose.position.x = robot_state.currentFlangePostionRight[0];
        right_pose.position.y = robot_state.currentFlangePostionRight[1];
        right_pose.position.z = robot_state.currentFlangePostionRight[2];
        right_pose.orientation.x = right_pose.orientation.y = right_pose.orientation.z = 0;
        right_pose.orientation.w = 1;
    }
};




// 最重要的一个函数，所有的逻辑关系都在这个函数里面实现
void emg_class::start()
{
    robot_state_sub = nh.subscribe<sinsun_robot::robot_state>("robot_state",1000,&emg_class::robot_state_subcallback,this);
    imu_sub = nh.subscribe<emg::imu>("imu_data", 1000, &emg_class::subCallback,this);

    joy_sub = nh.subscribe("joy", 1000, &emg_class::joy_subCallback,this);

    target_pub = nh.advertise<move_ur_script::joint_value_target>("pose_target", 1000);
    current_pub = nh.advertise<move_ur_script::joint_value_target>("pose_now", 1000);
    ros::Duration(1.0).sleep();
    // 睡眠1s，让发布和接收在master上完成注册
    // 防止机器人没有连接而没有机器人的信息
    while(ros::ok())
    {
        if(robot_state_flag == 1)
            break;
        else
        {
            ROS_ERROR("No Robot Message");
            ros::Duration(1.0).sleep();
        }
    }

    // 下面三句初始化几个位置坐标
    pose_transform(global_dual_robot_state,pose_now_left,pose_now_right);
    pose_reference_left = pose_target_left = pose_target_true_left = pose_now_left;
    pose_reference_right = pose_target_right = pose_target_true_right = pose_now_right;

    std::string move_msg;//发送关于ur_script的消息

    move_ur_script::joint_value_target pose_target_topic;
    move_ur_script::joint_value_target pose_now_topic;

    // move_msg = "movej([47.2,-4.62,-10.14,-85.35,-10.36,44.13,28.28],20,100);$$$movej([-27.26,-16.71,-8.45,91.44,20.84,-25.57,-81.19],20,100);";
    // tcpip.send_msg(move_msg);
    // ros::Duration(5.0).sleep();

    for(int i = 0 ; i < 6 ; i ++)
    {
        eef_vel_left[i] = 0;
        eef_vel_right[i] = 0;
    }

    //ros::Time time_init = ros::Time::now();
    //Test the pid value of every direction

    // while(ros::ok())
    // {
    //     ROS_INFO("waiting");
    //     ros::Duration(1.0).sleep();
    // }
    /*while(ros::ok())
    {
        pose_transform(global_dual_robot_state,pose_now_left,pose_now_right);
        ros::Duration interval = ros::Time::now() - time_init;
        double interval2double = interval.toSec();

        pose_target_left.position.z = pose_reference_left.position.z - 0.05*sin(interval2double);
        
        delta_left_in_base[0] = pose_target_left.position.x - pose_now_left.position.x;
        delta_left_in_base[1] = pose_target_left.position.y - pose_now_left.position.y;
        delta_left_in_base[2] = pose_target_left.position.z - pose_now_left.position.z;

        delta_left_in_left_arm = rotation_base_to_left_arm * delta_left_in_base;

        // 测试PID
        eef_vel_left[0] = 5*delta_left_in_left_arm[0];
        eef_vel_left[1] = 5*delta_left_in_left_arm[1];
        eef_vel_left[2] = 5*delta_left_in_left_arm[2];
        for(int i = 0 ; i < 3; i ++)
        {
            if(eef_vel_left[i]>max_vel[i])
                eef_vel_left[i] = max_vel[i];
            if(eef_vel_left[i]<(-max_vel[i]))
                eef_vel_left[i] = -max_vel[i];
        }

        //publish target pose and current pose
        pose_target_topic.joint_value.clear();
        pose_now_topic.joint_value.clear();

        pose_now_topic.joint_value.push_back(pose_now_left.position.x);
        pose_now_topic.joint_value.push_back(pose_now_left.position.y);
        pose_now_topic.joint_value.push_back(pose_now_left.position.z);
        pose_now_topic.joint_value.push_back(pose_now_right.position.x);
        pose_now_topic.joint_value.push_back(pose_now_right.position.y);
        pose_now_topic.joint_value.push_back(pose_now_right.position.z);


        pose_target_topic.joint_value.push_back(pose_target_left.position.x);
        pose_target_topic.joint_value.push_back(pose_target_left.position.y);
        pose_target_topic.joint_value.push_back(pose_target_left.position.z);
        pose_target_topic.joint_value.push_back(pose_target_right.position.x);
        pose_target_topic.joint_value.push_back(pose_target_right.position.y);
        pose_target_topic.joint_value.push_back(pose_target_right.position.z);


        target_pub.publish(pose_target_topic);
        current_pub.publish(pose_now_topic);

        //Sending ur_script message
        move_msg = combinemsg(eef_vel_left,eef_vel_right,max_acc);
        ROS_INFO_STREAM("Sending msg:"<<move_msg);
        tcpip.send_msg(move_msg);
        ros::Duration(0.02).sleep();


        // 如果按键按下，机器人将会发送停止指令并且关闭网口通信
        if(joy_msg.buttons[1] == 1)
        {
            // move_msg = "speedl([0,0,0,0,0,0],1,1);$$$speedl([0,0,0,0,0,0],1,1);";
            // tcpip.send_msg(move_msg);
            // ros::Duration(0.5).sleep();
            // tcpip.close_socket();

            move_msg = "movej([47.2,-4.62,-10.14,-85.35,-10.36,44.13,28.28],20,100);$$$movej([-27.26,-16.71,-8.45,91.44,20.84,-25.57,-81.19],20,100);";
            tcpip.send_msg(move_msg);
            ros::Duration(0.5).sleep();
            tcpip.close_socket();
        }
        else
        {
            ROS_INFO("No Joy Control");
        }
        
    }*/

    Eigen::Vector3d eef_vel_left_before, eef_vel_right_before;


    while(ros::ok())//让机械臂运动的主要循环
    {
        if(emg_data_flag == 1)
        {
            while(ros::ok())
            {
                // pose_target可以理解成上一次的目标值，pose_target_true是更新之后的目标值（在IMU的callback更新），判断两者之间的数据差来判断数据有没有更新
                double error_x_left = pose_target_left.position.x - pose_target_true_left.position.x;
                double error_y_left = pose_target_left.position.y - pose_target_true_left.position.y;
                double error_z_left = pose_target_left.position.z - pose_target_true_left.position.z;
                double error_x_right = pose_target_right.position.x - pose_target_true_right.position.x;
                double error_y_right = pose_target_right.position.y - pose_target_true_right.position.y;
                double error_z_right = pose_target_right.position.z - pose_target_true_right.position.z;

                if(fabs(error_x_left)>0.0000005 ||fabs(error_y_left)>0.0000005 ||fabs(error_z_left) > 0.0000005||
                   fabs(error_x_right)>0.0000005||fabs(error_y_right)>0.0000005||fabs(error_z_right)>0.0000005) 
                {
                    ROS_INFO("Reveiving New Data Now!");
                    break;
                }
            }
            //更新数据

            pose_target_left = pose_target_true_left;
            pose_target_right = pose_target_true_right;


            if(1)
            {
                pose_transform(global_dual_robot_state,pose_now_left,pose_now_right);

                delta_left_in_base[0] = pose_target_left.position.x - pose_now_left.position.x;
                delta_left_in_base[1] = pose_target_left.position.y - pose_now_left.position.y;
                delta_left_in_base[2] = pose_target_left.position.z - pose_now_left.position.z;
                delta_left_in_left_arm = rotation_base_to_left_arm * delta_left_in_base;

                eef_vel_left[0] = pid_left[0] * delta_left_in_left_arm[0];
                eef_vel_left[1] = pid_left[1] * delta_left_in_left_arm[1];
                eef_vel_left[2] = pid_left[2] * delta_left_in_left_arm[2];

                delta_right_in_base[0] = pose_target_right.position.x - pose_now_right.position.x;
                delta_right_in_base[1] = pose_target_right.position.y - pose_now_right.position.y;
                delta_right_in_base[2] = pose_target_right.position.z - pose_now_right.position.z;
                delta_right_in_right_arm = rotation_base_to_right_arm * delta_right_in_base;

                eef_vel_right[0] = pid_right[0] * delta_right_in_right_arm[0];
                eef_vel_right[1] = pid_right[1] * delta_right_in_right_arm[1];
                eef_vel_right[2] = pid_right[2] * delta_right_in_right_arm[2];

                for(int i = 0 ; i < 3; i ++)
                {
                    if(fabs(eef_vel_left[i]) > max_vel[i])
                    {
                        eef_vel_left[i] = eef_vel_left_before[i];

                        // 防止eef_vel_left_before过大

                        if (eef_vel_left[i] > max_vel[i] )
                        {
                            eef_vel_left[i] = max_vel[i];
                        }
                        if (eef_vel_left[i] < -max_vel[i])
                        {
                            eef_vel_left[i] = -max_vel[i];
                        }
                    }
                    if(fabs(eef_vel_right[i]) > max_vel[i])
                    {
                        eef_vel_right[i] = eef_vel_right_before[i];

                        if (eef_vel_right[i] > max_vel[i] )
                        {
                            eef_vel_right[i] = max_vel[i];
                        }
                        if (eef_vel_right[i] < -max_vel[i])
                        {
                            eef_vel_right[i] = -max_vel[i];
                        }
                    }
                }

                //publish target pose and current pose
                pose_target_topic.joint_value.clear();
                pose_now_topic.joint_value.clear();

                pose_now_topic.joint_value.push_back(pose_now_left.position.x);
                pose_now_topic.joint_value.push_back(pose_now_left.position.y);
                pose_now_topic.joint_value.push_back(pose_now_left.position.z);
                pose_now_topic.joint_value.push_back(pose_now_right.position.x);
                pose_now_topic.joint_value.push_back(pose_now_right.position.y);
                pose_now_topic.joint_value.push_back(pose_now_right.position.z);


                pose_target_topic.joint_value.push_back(pose_target_left.position.x);
                pose_target_topic.joint_value.push_back(pose_target_left.position.y);
                pose_target_topic.joint_value.push_back(pose_target_left.position.z);
                pose_target_topic.joint_value.push_back(pose_target_right.position.x);
                pose_target_topic.joint_value.push_back(pose_target_right.position.y);
                pose_target_topic.joint_value.push_back(pose_target_right.position.z);


                target_pub.publish(pose_target_topic);
                current_pub.publish(pose_now_topic);

                //Sending ur_script message
                move_msg = combinemsg(eef_vel_left,eef_vel_right,max_acc);

                ROS_INFO_STREAM("Sending msg "<<move_msg);
                tcpip.send_msg(move_msg);

                for(int i = 0 ; i < 3 ; i ++)
                {
                    eef_vel_left_before[i] = eef_vel_left[i];
                    eef_vel_right_before[i] = eef_vel_right[i];
                }
            }
        }
        else
        {
            ROS_ERROR("No IMU Data");
            ros::Duration(0.2).sleep();
        }

        // 如果按键按下，机器人将会发送停止指令并且关闭网口通信
        if(joy_msg.buttons[1] == 1)
        {
            move_msg = "speedl([0,0,0,0,0,0],1,0);$$$speedl([0,0,0,0,0,0],1,0);";
            tcpip.send_msg(move_msg);
            ros::Duration(0.5).sleep();
            tcpip.close_socket();
            // move_msg = "movej([0,0,0,0,0,0,0],30,100);$$$movej([0,0,0,0,0,0,0],30,100);";
            // tcpip.send_msg(move_msg);
            // ros::Duration(5).sleep();
            // tcpip.close_socket();
        }
    }
}

int main(int argc, char *argv[])  
{  
    ros::init(argc, argv, "sinsun_dual_arm_control");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    emg_class EMG;   
    EMG.start(); 
    return 0;  
}  
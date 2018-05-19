
<<<<<<< HEAD
// 将实时的机器人的关节数据发送给郝德宏
// 运行时需要hdh的服务器打开

=======
//将实时的机器人的关节数据发送给郝德宏
>>>>>>> 0bcb0d2209f44d23003687044b46464286c1b7d4
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  

std::string msg;
void subCallback(const std_msgs::String::ConstPtr& msg_receive)
{
    msg = msg_receive->data;
}
int main(int argc, char *argv[])  
{  
    ros::init(argc, argv, "hdh_com");
    ros::NodeHandle nh;

    int client_sockfd;  
    int len;  
    struct sockaddr_in remote_addr; //服务器端网络地址结构体  
    char buf[BUFSIZ];  //数据传送的缓冲区  
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零  
    remote_addr.sin_family=AF_INET; //设置为IP通信  
    remote_addr.sin_addr.s_addr=inet_addr("192.168.8.103");//服务器IP地址    192.168.8.102
    remote_addr.sin_port=htons(9696); //服务器端口号  
 
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
    ros::Subscriber sub = nh.subscribe("hdh_msgs", 1000, subCallback);
    ros::Duration(1.0).sleep();
    ros::Rate loop_rate(20);
    while (ros::ok())
    {
        ROS_INFO_STREAM("SENDING MSG IS "<<msg);
        strcpy(buf,msg.c_str());
        len=send(client_sockfd,buf,strlen(buf),0);
        ros::spinOnce();
        loop_rate.sleep();
    }
    close(client_sockfd);//关闭套接字  
    return 0;  
}  
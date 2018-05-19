
//这个节点的目的是为了验证UR实际的机器人与moveit返回的坐标位置是否相等。

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Transform.h"
#include "tf2/buffer_core.h"


int main(int argc, char* argv[])
{
    ros::init(argc, argv, "tf_data");
    ros::NodeHandle nh;
    ros::Publisher pub;
    pub = nh.advertise<geometry_msgs::TransformStamped>("look_pose",1000);
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfLisener(tfBuffer); 

    ros::Rate rate(1);
    while(nh.ok())
    {
        geometry_msgs::TransformStamped transformStamped;
        try{
        transformStamped = tfBuffer.lookupTransform("base", "tool0",
                                ros::Time(0));
        }
        catch (tf2::TransformException &ex) {
        ROS_WARN("%s",ex.what());
        ros::Duration(1.0).sleep();
        continue;
        }

        pub.publish(transformStamped);
        ros::spinOnce();
        ROS_INFO_STREAM(transformStamped);
        rate.sleep();
    } 
    return 0;
};

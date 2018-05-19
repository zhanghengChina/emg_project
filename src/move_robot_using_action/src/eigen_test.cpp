
//作用是学习Eigen库

#include <iostream>  
#include "ros/ros.h"
#include <Eigen/Core>  
#include "cmath"
#include <Eigen/Geometry>  
#include "algorithm"

#define PI 3.1415926535897932346
using namespace std;
using namespace Eigen;
  
int main(int argc, char **argv)   
{  
    ros::init(argc, argv, "eigen_test");
    ros::NodeHandle nh;


    std::vector<double> qua;
    qua.resize(4);
    qua = {0.462, -0.535, 0.695, 0.128};
    double theat = 2*acos(qua[3]);
    std::vector<double> n;
    std::vector<double> v;
    v.push_back(qua[0]/sin(theat/2));
    v.push_back(qua[1]/sin(theat/2));
    v.push_back(qua[2]/sin(theat/2));
    double norm = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    ROS_INFO_STREAM(norm);



    n.push_back(qua[0]/sin(theat/2)*theat);
    n.push_back(qua[1]/sin(theat/2)*theat);
    n.push_back(qua[2]/sin(theat/2)*theat);
    ROS_INFO_STREAM("Current orientation is "<<n[0]<<" "<<n[1]<<" "<<n[2]);
    ROS_INFO_STREAM("hdshjdks"<<cos(PI));

    double a[5] = {1,2,4,5,3};
    Eigen::Vector3d workspace_error = {3,4,0};
    ROS_INFO_STREAM("The norm of the error is "<<workspace_error.norm());
    Eigen::Matrix<double,5,1> test_matrix;
    test_matrix<<a[0],a[1],a[2],a[3],a[4];
    ROS_INFO_STREAM("test matrix is \n"<<test_matrix);
    double zhangheng = test_matrix[0];
    ROS_INFO_STREAM("the number is "<<zhangheng);
    
    ros::Time time1 = ros::Time::now();
    ROS_INFO_STREAM("TIME NOW IS "<<time1);
/*    ros::Duration time_duration;
    while(ros::ok())
    {
        if((ros::Time::now() - time1)>ros::Duration(10))
        break;

    }*/
    ROS_INFO_STREAM("TIME NOW IS "<<ros::Time::now());

    Eigen::Vector3d xiangliang = {-0.1236863354237139 , -1.095978884891575 , -1.009735870665911};
    double mochang = xiangliang.norm();
    xiangliang = xiangliang/mochang;
    Eigen::AngleAxisd rotationVector(mochang,xiangliang);
    Eigen::Matrix3d  rotationMatrix = Eigen::Matrix3d::Identity();
    rotationMatrix = rotationVector.toRotationMatrix();
    cout<<"rotationMatrix \n"<<rotationMatrix<<endl;

    //旋转轴到四元数
    Eigen::Quaterniond q = Eigen::Quaterniond(rotationVector);
    cout<<"rotation quaternion \n"<<q.coeffs()<<endl;

    //Eigen::Vector3d eulerAngle=rotationVector.matrix().eulerAngles(0,1,2);
    Eigen::Vector3d eulerAngle = rotationMatrix.eulerAngles(2,1,0);
    cout<<"eulerAngle roll pitch yaw\n"<<180*eulerAngle/M_PI<<endl;
    
    return 0;  
}  


# 这个包的基本功能是读取IMU的数据，并且发送给实际的机器人。
#其中定义了两种消息
#另外三个CPP文件的作用都已经在程序中注明：
	#reading.cpp的作用是读取离线的数据，但是通过rosbag之后这个基本上没什么用
	#tfdata.cpp的作用是读取机器人中的两个坐标系之间的位置变化关系，只是为了验证moveit模型的数据与实际的机器人模型数据是否相同；
	#ReceivingEmgDataAndSendingToUR.cpp的作用是读取IMU的数据并且将数据发送给机器人

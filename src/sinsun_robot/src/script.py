# Echo client program
import socket
import time
import numpy as np

HOST = "192.168.1.5"    # The remote host
PORT = 2000              # The same port as used by the server
print "Starting Program"
time_init = time.time()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

# 现在直接发送speeld可以完成运动，但是面板不能退出，如果想让面板退出，必须执行sleep函数
# 但是执行了sleep函数之后，机械臂再接受speedl就会报错，所以只能断开之后重新连接。


#s.send("speedl([0,0,0,0,0,0],0.001,2);$$$speedl([0,0.005,0,0,0,0],0.001,2);")
#time.sleep(5)
#s.send("speedl([0,0,0,0,0,0],1,1);$$$speedl([0,0,0,0,0,0],1,1);")
#time.sleep(2.0)
#s.send("sleep(1000);$$$sleep(1000);")
# speedl([vx,vy,vz,wx,wy,wz],a,t)

'''s.send("Sleep(10);$$$Sleep(10);")
time.sleep(0.1)
s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(0.1)'''
s.send("speedl([-0.01,0,0,0,0,0],2,0);$$$speedl([0,0.01,0,0,0,0],2,0);")
time.sleep(2.0)
s.send("speedl([0.01,0,0,0,0,0],2,0);$$$speedl([0,-0.01,0,0,0,0],2,0);")
time.sleep(2.0)
# Sleep
#s.send("Sleep(10);$$$Sleep(10);")
#s.send("Sleep(10);$$$Sleep(10);")
s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(5)
s.send("speedl([-0.01,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],0,0);")
time.sleep(2.0)
s.send("speedl([0.01,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],0,0);")
time.sleep(2.0)
s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(0.5)
#s.send("Sleep(10);$$$Sleep(10);")
#time.sleep(0.5)
s.close()
print "Program finish"
'''time.sleep(5)
s.send("speedl([0,0,0,0,0,0],0.001,2);$$$speedl([0,0.005,0,0,0,0],0.001,2);")
time.sleep(5)
s.send("speedl([0,0,0,0,0,0],0.001,1);$$$speedl([0,0,0,0,0,0],0.001,1);")'''

#s.send("speedl([0,0,0,0,0,0],0,0);$$$speedl([0,0.005,0,0,0,0],0.001,5);")
#speedl([0,0,0,0,0,0],0,0);$$$speedl([0,0,0,0,0,0],0,0);
#speedl([0,0,0,0,0,0],0.001,10);$$$speedl([0.005,0,0,0,0,0],0.001,5);


'''s.send("set_digital_out(8,False)" + "\n")
time.sleep(0.2)
s.send("set_digital_out(9,True)" + "\n")
time.sleep(0.2)
s.send("set_tool_voltage(12)" + "\n")'''
#s.send ("set_digital_out(2,True)" + "\n")
'''s.send("def zhanghengprog():\n"+
        "speedl([0,0.2,-0.1,0,0,0],0.2236068,2)\n"+
        "stopl(0.2236068)\n"+
        "end\n")'''

''' while True:
    theta_d = np.sin((time.time() - time_init))
    s.send("speedj([0,0,0,0,-%f,0],1.5,0.1)\n" % (theta_d))#0.2236068
    time.sleep(0.01) '''

# s.send("stopl(0.2)\n")

#s.send("speedl([0,-0.2,0,0,0,0],0.2,2)\n")
#time.sleep(2)
#s.send("stopl(0.2)\n")
#s.send("stopj(0.1)\n")
    #time.sleep(2)
    #s.send ("movej([0.0002756, -2.3298, 0.86157, -1.5709, 0.0001678, 0.0001678],t=2,r=0.05)" + "\n")
    #time.sleep(2)
    #s.send ("movej([-0.10818, -1.9277, 0.862525, -1.5708, 0.000155795, 0.000179763], t=2.0,r=0.05)" + "\n")
    #time.sleep(2)
    #print "The count is:", count
    #data = s.recv(1024)

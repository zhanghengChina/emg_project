# we just use this script to test the sinsun robot command.
import socket
import time
import numpy as np

HOST = "192.168.1.5"    # The remote host
PORT = 2000              # The same port as used by the server
print "Starting Program"
time_init = time.time()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
#s.send("speedl([0,0,0,0,0,0],0.001,2);$$$speedl([0,0.005,0,0,0,0],0.001,2);")
#time.sleep(5)
#s.send("speedl([0,0,0,0,0,0],1,1);$$$speedl([0,0,0,0,0,0],1,1);")
#time.sleep(2.0)
#s.send("sleep(1000);$$$sleep(1000);")
# speedl([vx,vy,vz,wx,wy,wz],a,t)

s.send("speedl([-0.01,0,0,0,0,0],2,0);$$$speedl([0,0.01,0,0,0,0],2,0);")
time.sleep(2.0)
s.send("speedl([0.01,0,0,0,0,0],2,0);$$$speedl([0,-0.01,0,0,0,0],2,0);")
time.sleep(2.0)

s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(3)

'''s.send("Sleep(10);$$$Sleep(10);")
time.sleep(0.1)
s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(0.1)'''

s.send("movej([47.2,-4.62,-10.14,-85.35,-10.36,44.13,28.28],20,100);$$$movej([-27.26,-16.71,-8.45,91.44,20.84,-25.57,-81.19],20,100);")
time.sleep(3.0)

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
s.close()
print "Program finish"

s.send("movej([47.2,-4.62,-10.14,-85.35,-10.36,44.13,28.28],20,100);$$$movej([-27.26,-16.71,-8.45,91.44,20.84,-25.57,-81.19],20,100);")
# s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(0.5)
#s.send("Sleep(10);$$$Sleep(10);")
#time.sleep(0.5)
s.close()
print "Program finish"

#!/usr/bin/env python3
import serial
import time

import numpy as np
import multiprocessing as mp

import rospy
from armlet_imu.msg import IMU


class IMU_Read(mp.Process):
    def __init__(self, length, event, queue, val, port_name, baud_rate):
        mp.Process.__init__(self)
        self.ser = serial.Serial(port_name, baud_rate)
        self.val = val
        self.cache_decode = self.decode()
        self.event = event
        self.queue = queue
        self.cache = []
        self.cache_imu = []
        self.count = 0
        self.length = length

    def decode(self):
        res = []
        for i in range(256):
            temp = i
            data = temp << 4 | temp >> 4
            data = ~data & 0x55 | data & 0xAA
            res.append(data)
        return res

    def judge(self, data):
        res = []
        for i in range(len(data)):
            data[i] = self.cache_decode[data[i]]
        if sum(data[:-1]) % 256 == data[-1]:
            for i in range(2, 6):
                if i % 2 == 0:
                    if data[i] >= 128:
                        res.append(-65535 + 256*data[i])
                    else:
                        res.append(256*data[i])
                else:
                    res[-1] += data[i]
            return res
        else:
            return False

    def run(self):
        while not rospy.is_shutdown():
            self.event.wait()
            self.cache += self.ser.read_all()
            if len(self.cache) > 30:
                try:
                    num = self.cache.index(51, -350, -21)
                except ValueError:
                    self.cache = []
                    continue

                if self.cache[num + 1] == 238:
                    temp = self.cache[num + 2 : num + 21]
                    temp = self.judge(temp)
                    self.cache = []
                    if temp:
                        if self.val.value == 0:
                            self.cache_imu.append(temp)
                        elif self.val.value == 1:
                            if self.count == 0:
                                self.count += 1
                                self.cache_imu = np.array(self.cache_imu)
                                self.cache_imu = np.mean(self.cache_imu[-20:,], axis = 0, dtype = np.int)
                            #temp[0] -= self.cache_imu[0]
                            temp[1] -= self.cache_imu[1]
                            temp[0] = temp[0]*np.pi/18000
                            temp[1] = temp[1]*np.pi/18000
                            l = self.length*np.cos(temp[0])
                            z = self.length*np.sin(temp[0])
                            x = l*np.sin(-temp[1])
                            y = l*np.cos(-temp[1])
                            self.queue.put([x, y, z])
                        else:
                            pass
                else:
                    self.cache = []


class Main_Control:
    def __init__(self, event_cache, val, arr):
        self.cache_event = event_cache
        self.val = val
        self.arr = arr
        self.arr_old = self.arr[:]
        self.time_start = time.time()
        self.pub = rospy.Publisher('imu', IMU, queue_size = 10)
        self.run()

    def run(self):
        def _sete(event):
            if not event.is_set():
                event.set()

        def _cleare(event):
            if event.is_set():
                event.clear()
        _sete(self.cache_event[0])

        while not rospy.is_shutdown():
            time_pass = time.time() - self.time_start
            if time_pass < 3:
                self.val.value = 0
            else:
                _sete(self.cache_event[1])
                self.val.value = 1
                if self.arr_old != self.arr[:]:
                    self.arr_old = self.arr[:]
                    self.pub.publish(self.arr_old)
                time.sleep(0.2)

class Show(mp.Process):
    def __init__(self, upper_queue, lower_queue, event, val, arr):
        mp.Process.__init__(self)
        self.event = event
        self.val = val
        self.arr = arr
        self.queue_upper = upper_queue
        self.queue_lower = lower_queue
        self.time_init = time.time()
        #self.file_name = 'mixed.txt'
        #self.file = open(self.file_name, 'w')
        #self.file.close()
        self.cache = []
        self.cache_temp = []

    def write_file(self, data):
        file = open(self.file_name, 'a')
        for i in data:
            file.write(str(i) + '\t')

        file.write('\n')
        file.close()

    def run(self):
        while not rospy.is_shutdown():
            self.event.wait()
            data_upper = self.queue_upper.get()
            data_lower = self.queue_lower.get()
            self.cache_temp.append([data_upper, data_lower])
            if time.time() - self.time_init >= 0.199:
                self.time_init = time.time()
                for i in range(len(self.cache_temp)):
                    data = np.array(data_upper) + np.array(data_lower)
                    if self.cache == []:
                        self.cache = data
                    else:
                        temp = data - self.cache
                        if any(temp > 0.5):
                            print(temp)
                            print('===========')
                            print(self.cache)
                            print('fuck! Wrong!!')
                            continue
                        else:
                            #self.write_file(temp)
                            temp = temp.tolist()
                            print('The timestamp is', time.time())
                            print(temp)
                            for j in range(len(temp)):
                                self.arr[j] = temp[j]
                            self.cache = data
                            self.cache_temp = []
                            break
                self.cache = data


if __name__ == '__main__':
    rospy.init_node('armlet_imu', anonymous = True)
    event_cache = []
    queue_cache = []
    arr = mp.Array('f', range(3))
    val = mp.Value('i', -1)
    for i in range(2):
        event_cache.append(mp.Event())
    for i in range(2):
        queue_cache.append(mp.Queue(1))
    show = Show(queue_cache[0], queue_cache[1], event_cache[1], val, arr)
    upper_imu = IMU_Read(0.5, event_cache[0], queue_cache[0], val,  '/dev/rfcomm0', 460800)
    lower_imu = IMU_Read(0.5, event_cache[0], queue_cache[1], val,  '/dev/rfcomm1', 460800)

    show.start()
    upper_imu.start()
    lower_imu.start()
    main_control = Main_Control(event_cache, val, arr)

    show.join()
    upper_imu.join()
    lower_imu.join()

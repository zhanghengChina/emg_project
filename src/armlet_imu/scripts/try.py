#!/usr/bin/env python3

import numpy as np
import serial
class Data:
    def __init__(self):
        self.cache_decode = self.decode()

    def decode(self):
        def _decode(data):
            temp = data << 4 | data >> 4
            return (~temp & 0x55 | temp & 0xAA)
        cache_res = []
        for i in range(256):
            cache_res.append(_decode(i))
        return cache_res

class Cal:
    def __init__(self):
        pass


class Lower_Armlet:
    def __init__(self):
        pass

if __name__ == '__main__':


# Untitled - By: robocon - 周五 5月 24 2019

#THRESHOLD1 = (0, 7, -12, 7, -7, 14)#直线阈值原程序(14, 76, -31, 4, -38, -4)
THRESHOLD1 =(28, 0, -13, 38, -17, 23)
#THRESHOLD1 =(0, 49, -20, 33, -20, 36)
THRESHOLD2 = (22, 100, -10, 10, -3, 5)#白色阈值

import sensor, image, time
from pyb import LED
from pyb import UART

LED(1).on()
LED(2).on()
LED(3).on()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()
#串口通信
uart = UART(3, 115200)#串口

#摄像头倒转
sensor.set_vflip(True)
sensor.set_hmirror(True)
#关闭自动白平衡
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)


head = bytearray([0xAA])#包头
tail = bytearray([0x55])#包尾
zero = bytearray([0x00])

while(True):
    clock.tick()
    #img = sensor.snapshot()
    img = sensor.snapshot().binary([THRESHOLD1])
    line = img.get_regression([THRESHOLD2], robust = True)#更改白线阈值

    if (line):
        V = abs(line.theta())#黑线弯曲角度
        C = abs(line.rho())#黑线与中线的距离
        print(C)#在串行终端里进行查看
        #print(V)
        #output the head and C
        #uart.write(head)
        uart.write(bytearray([C]))#线的值
        #uart.write(bytearray([V]))#角度值
        #uart.write(tail)


        img.draw_line(line.line(), color = 127)
    else:#没有识别到黑条的时候就发0
        #uart.write(head)
        uart.write(zero)
        #uart.write(zero)
        #uart.write(tail)



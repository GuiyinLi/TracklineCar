# Untitled - By: ThinkPad - 周二 6月 18 2019

import sensor, image, time,pyb

from pyb import LED
LED(1).on()
LED(2).on()
LED(3).on()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_vflip(True)
sensor.set_hmirror(True)
#关闭自动白平衡
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

clock = time.clock()
#THRESHOLD1 = (0, 7, -12, 7, -7, 14)
THRESHOLD1 =(28, 0, -13, 38, -17, 23)
while(True):
    clock.tick()
    img = sensor.snapshot()#原图像  用来测试用
    #img = sensor.snapshot().binary([THRESHOLD1])#二值化图像
    #print(clock.fps())

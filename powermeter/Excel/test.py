import os
import time
import serial

os.remove('data.log')
file = open("data.log","w");
ser = serial.Serial('COM13',4800)
print('start')
time.sleep(2)
print('log')
t = time.time() + 3
ser.readline()
while True :
    x = str(ser.readline()).strip("b").strip("'")
    y=x[:len(x)-4]
    print(y)
    file.write(y+'\n')
    if time.time()>t :
        break
print('end log')
ser.close()
file.close()
print('end')

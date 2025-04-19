#!/bin/usr/python2
import serial
import threading
import time

comPort = None def TransmitThread():
  while comPort: for n in
    range(ord("A"),ord("Z")+1):
      if comPort: comPort.write(chr(n))
        time.sleep(1)
def ReceiveThread(): while comPort: if
    comPort.inWaiting() > 0:
      n = comPort.read(1) print( n ) else:
      time.sleep(0.1)
def LoopbackTest(comPortName, comBaudRate):
  global comPort comPort = serial.Serial \
            ( port=comPortName,
              baudrate=comBaudRate,
              parity=serial.PARITY_NONE,
              stopbits=serial.STOPBITS_ONE,
              bytesize=serial.EIGHTBITS
            )
  threading.Thread(target=TransmitThread).start()
  threading.Thread(target=ReceiveThread).start()
  try:
    while True: time.sleep(1) except
  KeyboardInterrupt:
    print("") finally: comPort = None if
__name__ == "__main__":
  LoopbackTest("/dev/serial0", 31250)

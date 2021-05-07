#!/usr/bin/env python3
# coding=utf-8
# MarcoAAG
# Version 1.0
# 20.April.2021

import serial
import serial.tools.list_ports
import sys
import os

def findPorts():
    ports_name = []
    ports = serial.tools.list_ports.comports()
    for i in ports:
        ports_name.append(i.device)
    return ports_name

class backEnd:
    def __init__(self, _port, _baudrate, _parity, _stopbits, _bytesize, _timeout):
        try:
            self.serial_connection = serial.Serial(
                port = _port,\
                baudrate = _baudrate,\
                parity = _parity,\
                stopbits = _stopbits,\
                bytesize = _bytesize,\
                timeout = _timeout\
            )
            self.adc_data = [0,0]
            self.y = [0.0 ,0.0]
        except Exception as e:
            print("Error open serial port: " + str(e))
            sys.exit(0)

        self.serial_connection.flushInput()
        self.serial_connection.flushOutput()

    def readData(self):
        self.raw_data = self.serial_connection.readline()
        self.data = self.raw_data.decode("utf-8")
        return(self.data)

    def closePort(self):
        self.serial_connection.close()

    def protocolInit(self):
        self.receivedChar = self.readData()
        if(self.receivedChar[0] != "@"):
            self.serial_connection.write('N'.encode())
            return 1
        else:
            self.serial_connection.write('O'.encode())
            return 0

    def getData(self):
        self.data = self.readData()
        self.data2send = int(self.data,16)
        return self.data2send

    def lowPassFilter(self):
        self.A = 0.1

        self.adc_data[1] = self.adc_data[0]
        self.adc_data[0] = self.getData()

        self.datafilter2send = (self.adc_data[0] + self.adc_data[1]) / self.A

        return self.datafilter2send

    def lowPF(self):
        self.alpha = 0.05
        self.y[1] = self.y[0]
        self.y[0] = (self.alpha * self.getData()) + (1.0 - self.alpha)*self.y[1]
        return self.y[0]

# def main():
#     my_port = backEnd("/dev/ttyUSB0", 9600, serial.PARITY_NONE, serial.STOPBITS_ONE, serial.EIGHTBITS, 0.01)
#     while(1):
#         print(my_port.readData())
#     connected = my_port.protocolInit()
#     while(connected == 1):
#         connected = my_port.protocolInit()

#
# if __name__ == "__main__":
#     try:
#         main()
#     except KeyboardInterrupt:
#         print("\nInterrupted by keyboard")
#         try:
#             sys.exit(0)
#         except SystemExit:
#             os._exit(0)

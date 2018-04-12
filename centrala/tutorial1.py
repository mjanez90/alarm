#!/usr/bin/env python

from __future__ import absolute_import, division, print_function, \
                                                    unicode_literals
import time
from IOPi import IOPi
import sys
import serial, string, time
import csv


def main():
    """
    Main program function
    """
    ser = serial.Serial('/dev/ttyUSB0', baudrate=9600)
    
##   
##    bus2 = IOPi(0x21)
##    bus1 = IOPi(0x20)
##
##    bus2.set_port_direction(0, 0x00)
##    bus2.write_port(0, 0x00)
##    
##    bus2.set_port_direction(1, 0x00)
##    bus2.write_port(1, 0x00)
##
##    
##    bus1.set_port_direction(0, 0x00)
##    bus1.write_port(0, 0x00)
##    
##    bus1.set_port_direction(1, 0x00)
##    bus1.write_port(1, 0x00)
##
##    bus1.set_port_direction(0, 0xFF)
##    bus1.set_port_pullups(0, 0xFF)
##    
##    bus1.set_port_direction(1, 0xFF)
##    bus1.set_port_pullups(1, 0xFF) 

    timeMS = 0.02
    ponovitev = 5
    
    test = 0
    
    count = 0

    while True:
            
        
        ArduinoSay = ser.readline()
        ArduinoSayASCII = ArduinoSay.decode("ascii").rstrip()
        
        Number, Command = ArduinoSayASCII.split(".")
        print (ArduinoSayASCII)
        print (Number)
        print (Command)
        
        noro = 123
        
        
        if ArduinoSayASCII == "init.start":
                    
            print ("Zaganjam")
        
                                 
                        
        if ArduinoSayASCII == "init.end":
                                    
            print ("Zagon koncan")
            

                                    
        if Command == "cleartempPassword":
            
            count = count + 1                    
            print ("PW clear")
            
                        
        if Command == "NewStatus":
                    
            print ("test20")
            
            NewSt = list(str(Number))
            
            NewStVhod = (NewSt[0])
            NewStGaraza = (NewSt[1])
            NewStKlet = (NewSt[2])
            NewStKuhinja = (NewSt[3])
                
            print (NewStVhod)
            print (NewStGaraza)
            print (NewStKlet)
            print (NewStKuhinja)
            
            ser.write(bytes(b'ChangeOK'))
            
            time.sleep(4)
            
            bus_BoltLock = IOPi(0x21)
            
            bus_BoltLock.set_port_direction(0, 0x00)
            bus_BoltLock.write_port(0, 0x00)
            
            bus_BoltLock.set_port_direction(1, 0x00)
            bus_BoltLock.write_port(1, 0x00)
##
##            bus_BoltLock.set_port_direction(0, 0xFF)
##            bus_BoltLock.set_port_pullups(0, 0xFF)
##            
##            bus_BoltLock.set_port_direction(1, 0xFF)
##            bus_BoltLock.set_port_pullups(1, 0xFF)
            
            if NewStVhod == "1":
                bus_BoltLock.write_pin(7, 0)
                
                
            if NewStVhod == "0":
                bus_BoltLock.write_pin(7, 1)
                
                
                
            if NewStGaraza == "1":
                bus_BoltLock.write_pin(5, 0)
                
                
            if NewStGaraza == "0":
                bus_BoltLock.write_pin(5, 1)
                
                
            
            if NewStKlet == "1":
                bus_BoltLock.write_pin(3, 0)
                
                
            if NewStKlet == "0":
                bus_BoltLock.write_pin(3, 1)
                
                
            
            if NewStKuhinja == "1":
                bus_BoltLock.write_pin(1, 0)
                
                
            if NewStKuhinja == "0":
                bus_BoltLock.write_pin(1, 1)
                
                
                

            ser.write(bytes(b'OK.1111'))
            
            
                    
                                
            
        if Command == ("CheckPW") and len(Number) == 4:
            print ('neki dela')
                                                    
            if Number in open("/home/pi/Desktop/gesla.txt").read():
                
                print ("pravilono")
                                
                Number = ""
                Command = ""
                                                        
                time.sleep(0.5)
                ser.write("PWok".encode())
                
                

                
                ## preveri stanje zaklepa
                csv_file = open("/home/pi/Desktop/BoltLockCSV.csv")
                csv_reader = csv.reader(csv_file)
                
                
                data=[]
                for row in csv_reader:
                    
                    vrata = str(row[0])
                    TrueOrFalse = str(row[1])
     
                    data.append([TrueOrFalse])
                    
                            
                test1 = ''.join(data[1])
                
                test2 = ''.join(data[2])
                
                test3 = ''.join(data[3])
                
                test4 = ''.join(data[4])
                
                
                if test1 == "FALSE":
                    vhod = False
                    StatusLockUnlock1 = ("0")
                else:
                    vhod = True
                    StatusLockUnlock1 = ("1")
                                        
                    
                if test2 == "FALSE":
                    garaza = False
                    StatusLockUnlock2 = ("0")
                else:
                    garaza = True
                    StatusLockUnlock2 = ("1")
                    
                    
                if test3 == "FALSE":
                    klet = False
                    StatusLockUnlock3 = ("0")
                else:
                    klet = True
                    StatusLockUnlock3 = ("1")
                    
                    
                if test4 == "FALSE":
                    kuhinja = False
                    StatusLockUnlock4 = ("0")
                else:
                    kuhinja = True
                    StatusLockUnlock4 = ("1")


                csv_file.close()


                StatusLockUnlock = StatusLockUnlock1 , StatusLockUnlock2 , StatusLockUnlock3 , StatusLockUnlock4
                s = ""
                SendFirstStatus = (s.join( StatusLockUnlock ))
                print (SendFirstStatus)
                
                Number = ""
                Command = ""
                        
                        
##                time.sleep(2)
##                bus2.write_pin(7, 1)
                time.sleep(2)
                ser.write(SendFirstStatus.encode())
                main()

                                                        
                                                        
            else:
                print ("napacno")
                ser.write(bytes(b'WrongPW'))
                main()
        elif Command == ("CheckPW") and len(Number) < 4:
            print ("napacnosamo za")
            Number = ""
            Command = ""
            ser.write(bytes(b'WrongPW'))
            main()
                
                
            

if __name__ == "__main__":
    main()

    

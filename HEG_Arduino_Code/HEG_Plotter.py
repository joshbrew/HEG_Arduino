# -*- coding: utf-8 -*-

import serial as s
import matplotlib.pyplot as m
import time as t


class HEG(object):
    def __init__(self):
        super(HEG, self).__init__()
        self.ser = s.Serial('COM3', 9600 )
        self.readrate = 0.0005 # refresh rate (s)
        
        #Data Structures
        self.ADC = []
        self.baseline = []
        self.ratio = []
        self.score = []
        
        self.time = 0
        self.timestamps = []
        

    def read(self):
        while(True):
            line = self.ser.readline().decode()
            
            if line == '\n':
                print(' ')
            
                
            if 'ADC' in line:
                t.sleep(self.readrate)
                line = self.ser.readline().decode()
                if '\r\n' in line:
                    line = line.strip('\r\n')
                self.ADC.append(int(line))
                print("ADC:")
                print(int(line))
                
            elif 'Baseline R' in line:
                t.sleep(self.readrate)
                line = self.ser.readline().decode()
                if '\r\n' in line:
                    line = line.strip('\r\n')
                self.baseline.append(float(line))
                print("Baseline:")
                print(float(line))
            
            elif 'Now' in line:
                t.sleep(self.readrate)
                line = self.ser.readline().decode()
                if '\r\n' in line:
                    line = line.strip('\r\n')
                self.ratio.append(float(line))
                print("Now:")
                print(float(line))
            
            elif 'Score' in line:
                t.sleep(self.readrate)
                line = self.ser.readline().decode()
                if '\r\n' in line:
                    line = line.strip('\r\n')
                self.score.append(float(line))
                print("Score:")
                print(float(line))
                
            else:
                print(line)
            
            t.sleep(self.readrate)
            
            if len(self.ADC) > 1:
                m.subplot(1,1,1)
                m.plot(range(len(self.ADC)),self.ADC,'-')
                m.title('HEG Data')
                m.xlabel('Samples')
                m.ylabel('ADC Reading')
            if len(self.ratio) > 1:
                m.subplot(2,1,1)
                m.plot(range(len(self.baseline)),self.baseline)
                m.plot(range(len(self.ratio)),self.ratio)
                m.xlabel('Samples')
                m.ylabel('Ratio')
                
                m.subplot(3,1,1)
                m.plot(range(len(self.score)),self.score)
                m.xlabel('Samples')
                m.ylabel('Score')
            
if __name__ == "__main__":
    Session = HEG()
    Session.read()
## HEG - Hemoencephalography
### DIY Biofeedback

Uses an Arduino Nano V3, an Adafruit ADS1115, an OPT101, a BR1102W and AN1102W Red and IR LEDs respectively.

Run this in with Arduino IDE or with the correct libraries in your preferred.
I will get a CircuitPython version going.

To use, just plug in the Nano, upload this script, and it should read out
the ADS data. 
Uncomment the baseline, ratio, and score printing to see that in the console. 
The baseline resets when you get a bad signal (if the sensor is absorbing too much external light) 
so you can play around with that in case the scoring isn't working right. 

- The official recommended LED FPS is 17 (1/17 sec), and sample at whatever your maximum ADC rate is. Default set to 2 (1 per LED per sec).
- Ratio is calculated by averaging multiple samples collected for the individual LEDs, then dividing the Red avg with the IR avg. 
- Score is summed from ratio minus baseline for each successful reading.
- This is as basic and bare-bones an implementation as possible. 


Note: See in HEG_Graph.png, the higher square peak is the IR, the lower one is the Red light. That's what you should expect to see from the sensor.
![Graph](https://github.com/moothyknight/HEG_Arduino/blob/master/HEG_Graph.png)
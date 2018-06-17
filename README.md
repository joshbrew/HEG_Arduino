## HEG - Hemoencephalography - nIR or pIR
### DIY Brain-Bloodflow Biofeedback

![Blueprint](https://raw.githubusercontent.com/moothyknight/HEG_Arduino/master/HEG_Arduino_BP.png)

Build time: 1-2 hours.

Uses an Arduino Nano V3, an Adafruit ADS1115, an OPT101, a BR1102W and AN1102W Red and IR LEDs respectively. (~$20)
- You'll have to find a headband or something to hold it on your head, and a way to mount & secure the wiring. I used some cheap [perfboard](https://www.adafruit.com/product/2670) I could break into smaller pieces. 
- You'll need a soldering kit. 
- The LEDs are surface mounts and require something like [this](https://nifteecircuits.com/products/sot23-6). They each need a 220 Ohm resistor on the power input.
- Nano V3 uses a generic USB Mini wire to connect to the computer.


HEGs typically cost hundreds or thousands on the market, so this is a much better solution for people wanting to get their feet wet with biofeedback and do a cool DIY project to understand the extremely straightforward science better. [HEG biofeedback](https://en.wikipedia.org/wiki/Hemoencephalography) was originally developed as a safe and non-invasive method to treat ADD in the late 90s, later expanding to treating disorders like PTSD and Depression due to common stress symptoms like [Hypofrontality](https://en.wikipedia.org/wiki/Hypofrontality) being treatable with this tool. It is implicated for much more, but there's not a whole lot of data (which this 20 dollar version could solve). It is informally called "brain pushups" for how it works out your brain to enable better bloodflow and oxygenation, therefore cognitive functioning and self-control. It only takes 5-10 minutes in the first session to gain control of bloodflow in the targeted area. I don't recommend doing more than 10 minutes at a time with this thing. Be wary of fatigue or headaches the first few times when you start gaining control of your bloodflow.


#### Explanation:
![Explanation](https://raw.githubusercontent.com/moothyknight/HEG_Arduino/master/HEGExplained.png)

INSTRUCTIONS AFTER ASSEMBLY:

1. To install the Nano, install the Arduino IDE and the USB drivers. Otherwise install this driver for your OS with your Nano plugged in: https://github.com/HobbyComponents/CH340-Drivers

2. Open the Arduino IDE, make sure the Nano is selected in the Board tab, as well as the correct Port.

3. Upload the HEG_Arduino.ino sketch to the Nano with the Arduino IDE or with the correct libraries in your preferred IDE (e.g. Visual Studio). 

4. See in HEG_Graph.png, the higher square peak is the IR, the lower one is the Red light. That's what you should expect to see from the sensor. Make sure the sensor is properly insulated from all light other than the direction of your forehead. It will absorb light through the back, for instance. 

5. Uncomment the baseline, ratio, and score printing to see that in the console once you have verified the signal is good. 
Play around with the sample rates in the sketch.

- The official recommended LED FPS is 17 (1/17 sec), and sample at whatever your maximum ADC rate is. Default set to 2 (1 per LED per sec).
- Ratio is calculated by averaging multiple samples collected for the individual LEDs, then dividing the Red avg with the IR avg. 
- Score is summed from ratio minus baseline for each successful reading.
- Baseline resets when you get a bad signal (if the sensor is absorbing too much external light) so you can play around with that in case the scoring isn't looking right. 
- This is as basic and bare-bones an implementation as possible. 
- I will get a CircuitPython version going to open up more possibilities. 

This is a near-Infrared setup for bloodflow, where a higher Red/IR light ratio equates to higher oxygenation. To read passive IR for metabolic information, just unplug the Red LED. Brain metabolism is 90% correlated with bloodflow, and bloodflow is easier to understand, so I recommend keeping the Red LED. 

For the scoring system, all that matters is your ability to consciously raise the ratio - correlated with increasing oxygenated blood concentration - especially after it falls some, regardless of the final score. And wiggling your forehead to mess with the sensor doesn't count. I found the score increased the most when I was curious and relaxed at the same time. You can pair the scoring system with a stimulus (like a video playing forward or backward depending on increasing or decreasing score) to assist with this. I'll have some demos for that available soon.

For more info, to assist with open-source development, or if you want me to make you your own HEG, email me at: brewster.joshua1@gmail.com.
To be clear, I'm an enthusiast and student, not a health professional. I'm not making claims with this thing, I just reverse-engineered and updated the hardware (with assistance from [Eric Nichols, KL7AJ](https://alaskit.co/about-us)), so use it at your own risk.

#### Sample Graph (noisy):
![Graph](https://raw.githubusercontent.com/moothyknight/HEG_Arduino/master/HEG_Graph.PNG)

#### Prototype w/ bad shielding:
![Prototype](https://raw.githubusercontent.com/moothyknight/HEG_Arduino/master/HEG_Arduino_Proto.jpg)

#### Sample Output & Score:
![output](https://raw.githubusercontent.com/moothyknight/HEG_Arduino/master/HEGOutput.PNG)

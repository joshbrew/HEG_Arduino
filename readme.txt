Uses an Arduino Nano V3, an Adafruit ADS1115, an OPT101, a BR1102W and AN1102W IR and Red LED respectively.

Run this in with Arduino IDE or with the correct libraries in your preferred.
I will get a CircuitPython version going.

To use, just plug in the Nano, upload this script, and it should read out
the ADS data. Uncomment the baseline, ratio, and score to see that. 
The baseline resets when you get a bad signal so you can play around with that in case the scoring
isn't working right.




#include <Wire.h>
#include <Adafruit_ADS1015.h>

/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int16_t adc0; // Resulting 16 bit integer

Adafruit_ADS1115 ads(0x48);
float Voltage = 0.0;
float range = 2^15 - 1;
float gain = 2.048; // +/- V
float bits2mv = gain / range;
bool first_led = false; // Bool to alternate LEDS
unsigned long startMillis;
unsigned long currentMillis;
unsigned long ledMillis;
const unsigned long ledrate = 500; // LED flash rate (ms)
const unsigned long samplerate = 2; // ADC read rate (ms)


void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_TWO);

  //ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV (default)
  //ads.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  //ads.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV
  //ads.setGain(GAIN_FOUR);    // 4x gain   +/- 1.024V  1 bit = 0.5mV
  //ads.setGain(GAIN_EIGHT);   // 8x gain   +/- 0.512V  1 bit = 0.25mV
  //ads.setGain(GAIN_SIXTEEN); // 16x gain  +/- 0.256V  1 bit = 0.125mV

  startMillis = millis();
  ledMillis = millis();
}

void loop() {

  currentMillis = millis();

  if(currentMillis - startMillis >= samplerate) {

    // read the analog in value:
    adc0 = ads.readADC_SingleEnded(0);
    Voltage = (adc0 * bits2mv); 
    
    // print the results to the Serial Monitor:
    Serial.print("ADC Value: ");
    Serial.println(adc0);
    //Serial.print("\tVoltage: ");
    //Serial.println(Voltage,7);
    
    // wait a minimum of 1.2 milliseconds before the next loop for the analog-to-digital
    // converter to settle after the last reading:
    startMillis = currentMillis;
    
  }
  
  if(currentMillis - ledMillis >= ledrate) {
    if(first_led == false) {
      digitalWrite(3,HIGH);
      digitalWrite(2,LOW);
      first_led = true;
    }
    else {
      digitalWrite(3,LOW);
      digitalWrite(2,HIGH);
      first_led = false;
    }
    ledMillis = currentMillis;
  }
  
}

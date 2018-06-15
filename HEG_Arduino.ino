#include <Wire.h>
#include <Adafruit_ADS1015.h>

/*
 Reads I2C input for the ADS1115, outputs power to LEDs.
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
  // PUT IR IN D2, AND RED IN D3
int16_t adc0; // Resulting 16 bit integer

Adafruit_ADS1115 ads(0x48);
float Voltage = 0.0;
float range = 32767; // 16 bit ADC (15 bits of range minus one)
float gain = 0.256           ; // +/- V
float bits2mv = gain / range;

bool first_led = false; // Bool to alternate LEDS
bool badSignal = false;
bool signalDetermined = false;

int ticks0 = 0;
int ticks1 = 0;
int ticks2 = 0;

long redValue = 0;
long irValue = 0;
float redavg = 0;
float iravg = 0;
float ratio = 0;
float baseline = 0;
float score = 0;

unsigned long startMillis;
unsigned long currentMillis;
unsigned long ledMillis;

//Make sure these divide without decimals remaining for best results
const unsigned long ledRate = 500; // LED flash rate (ms)
const unsigned long sampleRate = 2; // ADC read rate (ms). 

void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  
  Serial.begin(9600);
  ads.begin();
  ads.setGain(GAIN_SIXTEEN);

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

  if(currentMillis - startMillis >= sampleRate) {

    // read the analog in value:
    adc0 = ads.readADC_SingleEnded(0);
    Voltage = (adc0 * bits2mv); // FIX 
    
    // print the results to the Serial Monitor:
    Serial.print("ADC Value: ");
    Serial.println(adc0);
    //Serial.print("\tVoltage: "); 
    //Serial.println(Voltage,7);

    if(adc0 >= 10000) { // The gain is high but anything over 30000 is most likely not a valid signal
      Serial.print("\n Bad Read ");
      badSignal = true;

      //Temp: reset baseline on bad read
      signalDetermined = false;
      baseline = 0;

      ticks0 = 0; // Reset counter
      ticks1 = 0;
      ticks2 = 0;
      redValue = 0;
      irValue = 0;
    }
    else {
      badSignal = false;
      if(signalDetermined == false){
        ticks0++;
        if(ticks0 > 100) { // Wait for 100 samples of good signal before getting baseline
          // IR IN D2, RED IN D3
          if(ticks0 < 300) { // Accumulate samples for baseline
            if(first_led == true) { // RED
              redValue += adc0;
              ticks1++;

            }
            else { // IR
              irValue += adc0;
              ticks2++;
            }
          }
          else {
            signalDetermined = true;
            redavg = redValue / ticks1;
            iravg = irValue / ticks2;

            baseline = redavg / iravg; // Set baseline ratio
            ticks0 = 0; // Reset counters
            ticks1 = 0;
            ticks2 = 0;
            redValue = 0; // Reset values
            irValue = 0;

            //Serial.print("\tBaseline R: ");
            //Serial.println(baseline);
          }
        }
      }
      else {
        ticks0++;
        if(first_led == true) { // RED
          redValue += adc0;
          ticks1++;
        }
        else { // IR
          irValue += adc0;
          ticks2++;
        }
        if(ticks0 > 100) { // Accumulate 50 samples before taking reading
          redavg = redValue / ticks1;
          iravg = irValue / ticks2;
          ratio = redavg / iravg;
          score += ratio - baseline;
          
          //Serial.print("\tBaseline R: ");
          //Serial.print(baseline);
          //Serial.print("\tNow: ");
          //Serial.print(ratio);
          //Serial.print("\tScore: ");
          //Serial.print(score);
          //Serial.print("\n");

          ticks0 = 0;
          ticks1 = 0;
          ticks2 = 0;
        }
      }
    }
    
    // wait a minimum of 1.2 milliseconds before the next loop for the analog-to-digital
    // converter to settle after the last reading:
    startMillis = currentMillis;
    
  }

  // Switch LEDs back and forth.
  // PUT IR IN D2, AND RED IN D3
  if(currentMillis - ledMillis >= ledRate) {
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

  //End loop
}

#include <Wire.h>
#include <Adafruit_ADS1015.h>

/*
 Reads I2C input for the ADS1115, outputs power to LEDs.
*/

bool DEBUG = true;
bool VIEW_ADC_VALUE = true;

  // PUT IR IN D2, AND RED IN D3
int16_t adc0; // Resulting 16 bit integer

//Setup ADS1115
Adafruit_ADS1115 ads(0x48);

float Voltage = 0.0;
float range = 32767; // 16 bit ADC (15 bits of range minus one)
float gain = 0.256           ; // +/- V
float bits2mv = gain / range;

//Signal flags
bool first_led = false; // Bool to alternate LEDS
bool badSignal = false;
bool signalDetermined = false;

//Counters
int ticks0 = 0;
int ticks1 = 0;
int ticks2 = 0;

//Scoring variables
long redValue = 0;
long irValue = 0;
float redavg = 0;
float iravg = 0;
float ratio = 0;
float baseline = 0;
float score = 0;

//Timing variables
unsigned long startMillis;
unsigned long currentMillis;
unsigned long ledMillis;

//Make sure these divide without remainders for best results
const unsigned long ledRate = 50; // LED flash rate (ms)
const unsigned long sampleRate = 2; // ADC read rate (ms). ADS1115 has a max of 860sps or 1/860 * 1000 ms

void setup() {
  
  //Set Digital pin modes
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // Init ADS
  ads.begin();
 
  //This turns the gain up to max to see the most difference
  ads.setGain(GAIN_SIXTEEN);

  //ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV (default)
  //ads.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  //ads.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV
  //ads.setGain(GAIN_FOUR);    // 4x gain   +/- 1.024V  1 bit = 0.5mV
  //ads.setGain(GAIN_EIGHT);   // 8x gain   +/- 0.512V  1 bit = 0.25mV
  //ads.setGain(GAIN_SIXTEEN); // 16x gain  +/- 0.256V  1 bit = 0.125mV

  //Start timers
  startMillis = millis();
  ledMillis = millis();
}

void loop() {

  currentMillis = millis();

  if(currentMillis - startMillis >= sampleRate) {

    // read the analog in value:
    adc0 = ads.readADC_SingleEnded(0);
    //Voltage = (adc0 * bits2mv);
    
    // print the results to the Serial Monitor:
    if (VIEW_ADC_VALUE == true) {
      Serial.println("ADC Value: ");
      Serial.println(adc0);
      //Serial.println("\tVoltage: "); 
      //Serial.println(Voltage,7);
    }
    if (DEBUG == false) {
      if(adc0 >= 10000) { // The gain is high but anything over 10000 is most likely not a valid signal
        Serial.println("\nBad Read ");
        badSignal = true;
  
        //Temp: reset baseline on bad read
        signalDetermined = false;
        baseline = 0;
  
        ticks0 = 0; // Reset counter
        ticks1 = 0;
        ticks2 = 0;
        redValue = 0; // Reset values
        irValue = 0;
      }
      else {
        badSignal = false;
        if(signalDetermined == false){
          ticks0++;
          if(ticks0 > 100) { // Wait for 100 samples of good signal before getting baseline
            // IR IN D2, RED IN D3
            if((ticks1 < 500) && (ticks2 < 500)) { // Accumulate samples for baseline
              if(first_led == true) { // RED
                redValue += adc0;
                ticks1++;
              }
              else { // IR
                irValue += adc0;
                ticks2++;
              }
              Serial.println("\nGetting Baseline. . .");
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
              
              //Uncomment this
              Serial.println("\tBaseline R: ");
              Serial.print(baseline);
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
          if((ticks2 > 50) && (ticks1 > 50)) { // Accumulate 50 samples per LED before taking reading
            redavg = redValue / ticks1; // Divide value by number of samples accumulated
            iravg = irValue / ticks2;
            ratio = redavg / iravg; // Get ratio
            score += ratio - baseline; // Simple scoring method
            
            Serial.print("\tBaseline R: ");
            Serial.print(baseline);
            Serial.print("\tNow: ");
            Serial.print(ratio);
            Serial.print("\tScore: ");
            Serial.print(score);
            Serial.print("\n");
            
            ticks0 = 0; //Reset Counters
            ticks1 = 0;
            ticks2 = 0;
            redValue = 0; //Reset values to get next average
            irValue = 0;
            
          }
        }
      }
      
  
      startMillis = currentMillis;
    }
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

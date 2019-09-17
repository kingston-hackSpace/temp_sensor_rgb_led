/*
============================             
  www.kingstonhack.space    
============================

 edited from original script by Texas Instruments Incorporated - http://www.ti.com/ (C) 2017
 created on: 4/2/19

 This script is designed to read from the TMP102 temperature sensor output data to the serial
 Turns the reading into a LED changed state.

 Digital oputput 9 RGB LED Red pin
 Digital oputput 10 RGB LED Green pin
 Digital oputput 11 RGB LED Blue pin

 Analog input 4 I2C connection SDA
 Analog input 5 I2C connection SCL

 https://learn.sparkfun.com/tutorials/tmp102-digital-temperature-sensor-hookup-guide/all
 
*/

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

#include <Wire.h>

void setup() 
{
  Serial.begin(9600);
  
  //Initialize I2C Communication
  Wire.begin();
  
  //Initialize the LED inputs
  pinMode(BLUE_PIN, OUTPUT); 
  pinMode(RED_PIN, OUTPUT); 
  pinMode(GREEN_PIN, OUTPUT);  

}

void loop() 
{
  //call the sensorRead function to retrieve the temperature
  double temperature = sensorRead();

  //Print the current temperature to the right of the label
  Serial.println(temperature);

  if(temperature<22&&temperature>0){
    digitalWrite(BLUE_PIN,LOW);
    digitalWrite(RED_PIN,HIGH);
    }
   else if(temperature>24&&temperature<78){
    digitalWrite(BLUE_PIN,HIGH);
    digitalWrite(RED_PIN,LOW);
    }
    else{
    digitalWrite(BLUE_PIN,LOW);
    digitalWrite(RED_PIN,LOW);
     }

  //wait 1/2 second
  delay(500);
}

double sensorRead(void)
{
  //temp holds the two bytes of data read back from the TMP102
  uint8_t temp[2];

  //tempc holds the modified combination of bytes
  int16_t tempc;

  //Point to device 0x48
  Wire.beginTransmission(0x48);
  //Point to register 0x00 (Temperature Register)
  Wire.write(0x00);
  //Relinquish master control of I2C line
  Wire.endTransmission();
  //delay to allow for sufficient conversion time
  delay(10);
  
  //Request temperature data
  Wire.requestFrom(0x48, 2);

    //if the two bytes are sucessfully returned
    if (2 <= Wire.available())
    {       
      //read out the data
      temp[0]  = Wire.read();
      temp[1] = Wire.read();

      //ignore the lower 4 bits of byte 2
      temp[1] = temp[1] >> 4;
      //combine to make one 12 bit binary number
      tempc = ((temp[0] << 4) | temp[1]);

      //Convert to celcius (0.0625C resolution) and return
      return tempc*0.0625;
    }
}

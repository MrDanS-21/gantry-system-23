#include <Wire.h>                                                                                           //Library to use the I2C communication protocol used by the colour sensor
#include "DFRobot_TCS34725.h"                                                                               //library to use the colour sensor

uint16_t Clear, red, green, blue;                                                                           // creates unsigned integer variables to store raw sensor values
                                                
DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup()
{
    Serial.begin(9600);                                             
    Serial.println("Test Patches");                                                                         //this checks if the colour sensor is present and working.
    if (tcs.begin()) 
    {
      Serial.println("Found sensor");
    } else 
    {
       Serial.println("No TCS34725 found ... check your connections");
        while (1);          // halt!
     }  
}

void loop() 
{
      tcs.getRGBC(&red, &green, &blue, &Clear);                                                             //this obtains raw sensor values
      uint32_t sum = Clear;
      float r, g, b;                                                                                        // these calculations convert them to
      r = red; r /= sum;                                                                                    // values out of 255
      g = green; g /= sum;
      b = blue; b /= sum;
      r *= 256; g *= 256; b *= 256;                                                                         // calculates r.g.b values

     Serial.print("r: ");                                                                                   //prints out the r,g,b values
     Serial.print(r);
     Serial.print("\t");
   
     Serial.print("g: ");
     Serial.print(g);
     Serial.print("\t");

     Serial.print("b: ");
     Serial.println(b);
}

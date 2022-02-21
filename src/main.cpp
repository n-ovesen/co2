// iAQ_Core_I2C.ino
//
// shows how to use the iAQ-CoreMI library with the sensor connected via I2C. 
//
// Copyright (c) 2019 Gregor Christandl
//
// connect the iAQ-Core to the Arduino like this:
//
// Arduino - iAQ-Core
// 3.3V ---- VCC
// GND ----- GND
// SDA ----- SDA
// SCL ----- SCL
// other pins should be left unconnected.

#include <Arduino.h>
#include <Wire.h>
#include "iAQCoreTwoWire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <SPI.h>


//iAQ-Core I2C address is fixed, so only one sensor is allowed per I2C bus. 
iAQCoreTwoWire iaq(&Wire);

//OLED Display object
Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();
//Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);


String spinner[8] = {"-", "\\", "|", "/", "-", "\\", "|", "/"};

int i = 1;
int invert = false;

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(9600);

  //Initiate featherwing display
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.init();
  

  //clear display buffer
  oled.clearDisplay();
  oled.display();
  oled.setTextColor(SSD1306_WHITE);
  oled.setRotation(2);
  oled.setTextSize(2);
  oled.setCursor(0,0);
  oled.println("AQM");
  oled.setTextSize(1);
  oled.println("\nAir Quality Monitor");
  oled.display();

	//wait for serial connection to open (only necessary on some boards)
  //while (!Serial);

  #if defined(ESP32)
	  Wire.begin(SDA, SCL);
  #elif defined(ESP8266)
	  Wire.begin(D2, D3);
  #else
	  Wire.begin();
  #endif 

	//iAQ-Core can operate at a maximum of 100kHz clock speed 
	Wire.setClock(100000L);

  #if defined(ESP8266)
	//iAQ-Core uses clock stretching, but the default clock stretching limit must be increased for correct operation. 
	//This is only needed on ESP8266.
	  Wire.setClockStretchLimit(1000);
  #endif /* defined(ESP8266) */

	if (!iaq.begin())
	{
		Serial.println("begin() failed. check the connection to your iAQ-Core.");
		while (1);
	}
}

void loop()
{
	delay(5000);

  oled.clearDisplay();
  oled.setCursor(0,0);
    // get the current voltage of the battery


	//read the sensor to determine if it holds a valid value. 
	//hasValue MUST be called to update sensor values. 

  //Serial.println(i);
  



	//Serial.print("sensor has valid value: "); Serial.println(iaq.hasValue() ? "true" : "false");
  int v = iaq.hasValue();
  


	if (iaq.isRunin())
  {
		Serial.println("sensor is runin state");
    oled.setTextSize(2);
    oled.println("WARMING UP  SENSOR");
    oled.display();
  }


	else if (iaq.isBusy())
		Serial.println("sensor is busy");

	else if (iaq.isValid())
	{

    Serial.print(v); Serial.print(" "); Serial.print(iaq.getCO2()); Serial.print(" "); Serial.print(iaq.getTVOC()); Serial.println("");
    oled.setTextSize(1);
    String co2Val = "co2: " + String(iaq.getCO2()) + " ppm";
    String vocVal = "voc: " + String(iaq.getTVOC()) + " ppb";
    oled.println(co2Val);
    oled.println(vocVal);
    String sp = "\n                    " + spinner[i];
    oled.println(sp);
    i++;
    

    if(i > 7){
      i = 0;
    }

		//*Serial.print("CO2 equivalent: "); Serial.print(iaq.getCO2()); Serial.println("ppm");
		//Serial.print("TVOC equivalent: "); Serial.print(iaq.getTVOC()); Serial.println("ppb");
		//Serial.print("Sensor Resistance: "); Serial.print(iaq.getResistance()); Serial.println("Ohm");
	}
  

	else if (iaq.isError())
		Serial.println("sensor is in error state");

  oled.display();
}
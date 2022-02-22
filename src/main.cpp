#include <Arduino.h>
#include <Wire.h>
#include "iAQCoreTwoWire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <SPI.h>
#include <RunningAverage.h>


//iAQ-Core I2C address is fixed, so only one sensor is allowed per I2C bus. 
iAQCoreTwoWire iaq(&Wire);

//OLED Display object
//Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();
Adafruit_SSD1306 oled = Adafruit_SSD1306(128, 32, &Wire);

String spinner[8] = {"-", "\\", "|", "/", "-", "\\", "|", "/"};

RunningAverage Co2Avg(120);
RunningAverage VocAvg(120);

int i = 1;

void setup(){

    if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

	
	Serial.begin(9600);

  //clear display buffer and set up
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.display();  
  oled.setRotation(2);

  //Print splash screen
  oled.setTextSize(2);
  oled.setCursor(0,0);
  oled.println("AQM");
  oled.setTextSize(1);
  oled.println("\nAir Quality Monitor");
  oled.display();
  
  //just a pause, to display the unneccessary splash screen
  delay(2000);

  Wire.begin(); 

	//iAQ-Core can operate at a maximum of 100kHz clock speed 
	Wire.setClock(100000L);

	if (!iaq.begin()){

		Serial.println("begin() failed. check the connection to your iAQ-Core.");
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.println("HE DED :(");
		while (1);
	}
}

//Returns the trend of CO2 based on running average over the last 120 samples(10 minutes with 5sec loop)

uint8_t Co2Trend(int c){

  uint8_t s = 0x00;
  int avg = int(Co2Avg.getAverage());

  if(c > avg){

    s = 0x18;
  }

  else if(c < avg){

    s = 0x19;
  }

  else{

    s = 0x1a;
  }

  return s;
}

//Returns the trend of TVOC based on running average over the last 120 samples(10 minutes with 5sec loop)

uint8_t VocTrend(int v){

  uint8_t s = 0x00;
  int avg = int(VocAvg.getAverage());

  if(v > avg){

    s = 0x18;
  }

  else if(v < avg){

    s = 0x19;
  }

  else{

    s = 0x1a;
  }

  return s;
}

//Returns silly smileys based on whether the CO2 measurment is over treshold values found here: https://www.fhi.no/ml/miljo/inneklima/artikler-inneklima-og-helseplager/karbondioksid-co2-og-inneklima/

String co2Treshold(int c){

  String s = "";

  int co2High = 1000;
  int co2Med  = 700;

  if(c > co2High){

    s = ":(";
  }

  else if (c > co2Med){

    s = ":/";
  }

  else {

    s = ":)";
  }

  return s;  
}

//Returns silly smileys based on whether the TVOC measurment is over treshold values found here: https://www.worldgbc.org/sites/default/files/bp-resource/BPFP-IEQ-Guidance-Note.pdf

String vocTreshold(int v){

  String s = "";

  int vocHigh = 500;
  int vocMed  = 250;
  
  if(v > vocHigh){

    s = ":(";
  }

  else if (v > vocMed){

    s = ":/";
  }

  else {

    s = ":)";
  }

  return s;  
}

void loop(){

	delay(5000);

  oled.clearDisplay();
  oled.setCursor(0,0);
  int v = iaq.hasValue();

	if (iaq.isRunin()){

		Serial.println("sensor is runin state");
    oled.setTextSize(2);
    oled.println("WARMING UP  SENSOR");
    oled.display();
  }

	else if (iaq.isBusy())
		Serial.println("sensor is busy");

	else if (iaq.isValid())
	{

    int co2 = iaq.getCO2();
    int voc = iaq.getTVOC();

    Co2Avg.addValue(co2);
    VocAvg.addValue(voc);

    oled.setTextSize(1);
    
    oled.write(Co2Trend(co2));
    oled.println(" co2: " + String(co2) + " ppm " + co2Treshold(co2));

    oled.write(VocTrend(voc));
    oled.println(" voc: " + String(voc) + " ppb " + vocTreshold(voc));

    Serial.println(String(co2) + " " + String(Co2Avg.getAverage()) + " " + String(voc) + " " + String(VocAvg.getAverage()));

    String sp = "\n                    " + spinner[i];  //displays a spinner in the lower right corner, just so you know if it is running or if it has locked up, because I write crap code.
    oled.println(sp);
    

    // makes the spinner spin
    i++;    

    if(i > 7){

      i = 0;
    }
	}
  

	else if (iaq.isError()){

		Serial.println("sensor is in error state");

  }

  oled.display();
}
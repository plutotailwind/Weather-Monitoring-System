#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <SFE_BMP180.h>

#define DHTPIN 2          
#define DHTTYPE DHT11    

#define ALTITUDE 920.0   

DHT dht(DHTPIN, DHTTYPE);

SFE_BMP180 pressure;

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Serial.begin(9600);
  lcd.init();                      
  lcd.backlight();               
  dht.begin();
  pressure.begin();
}

void loop() {
  float temperature = dht.readTemperature();  
  float humidity = dht.readHumidity();        
  float pressureValue = readPressure();       

  lcd.clear();                 

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print((char)223); 
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(9, 0);
  lcd.print("P: ");
  lcd.print(pressureValue);
  lcd.print("hPa");
  

  if (temperature > 28.0) {
    delay(2000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("It's too hot to");
    lcd.setCursor(0, 1);
    lcd.print("go out!");
  } else {
    delay(2000); 
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Have a nice");
    lcd.setCursor(0, 1);
    lcd.print("day!");
  }
  

  delay(2000); 
}


float readPressure() {
  char status;
  double T, P, p0, a;

  status = pressure.startTemperature();
  if (status != 0) {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0) {
      status = pressure.startPressure(3);
      if (status != 0) {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0) {
          p0 = pressure.sealevel(P, ALTITUDE);
          return p0;
        }
      }
    }
  }
  return -1.0; 
}

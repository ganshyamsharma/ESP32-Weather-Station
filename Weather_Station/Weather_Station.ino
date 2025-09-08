// ---------------------- Hardware setup ----------------------

#define MQ135_data_pin 4                          //Analog pin for MQ135 Gas Sensor
#define DHT_data_pin 16                           //Digital data pin for DHT Temp. & Humidity sensor

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW         //Type of MAX7219 LED dot matrix display
#define MAX_DEVICES 4                             //Number of 8*8 dot matrix displays
#define CS_PIN   5                                //Chip Select pin for led display

// --------------------- Libraries Inclusion ------------------
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <DHT.h>

// --------------------- Object Creation ----------------------
DHT dht(DHT_data_pin, DHT22);
MD_Parola parola = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// ---------------------- Variables ----------------------
char msg[50];                          
char temp[50];
char hum[50];
char ppm[50];  
int cnt = 0;             
unsigned long lastRead = 0;
const unsigned long interval = 2000; // read sensor every 2s

void setup() {
  Serial.begin(9600);
  dht.begin();
  parola.begin();
  parola.setIntensity(2);    // brightness (0-15)
  parola.displayClear();
  //Set your custom text string to be displayed during power on below e.g. -BIBA-
  parola.displayText("-BIBA-", PA_CENTER, 50, 2000, PA_WIPE, PA_NO_EFFECT); 
}

void loop() {
  // Read DHT every 2 seconds
  if (millis() - lastRead > interval) {
    lastRead = millis();

    int gas_ppm = analogRead(MQ135_data_pin)/4;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
       
    snprintf(hum, sizeof(hum), "H:%.0f%%", h);

    // Load message into display
    if(cnt == 0) {
      snprintf(msg, sizeof(msg), "%dppm", gas_ppm);
      cnt++;
      }
    else if(cnt == 1) {
      snprintf(msg, sizeof(msg), "T:%.0fC", t);
      cnt++;
      }
    else if(cnt == 2) {
      snprintf(msg, sizeof(msg), "H:%.0f%%", h);
      cnt = 0;
      }
    else {
      cnt = 0;
    }
    parola.displayText(msg, PA_CENTER, 100, 0, PA_PRINT, PA_NO_EFFECT);
  }
    

  // Animate the display
  if (parola.displayAnimate()) {
    parola.displayReset();
  }
}

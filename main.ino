#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <WiFi.h>

char ssid[] = "Rede trindade";
char pass[] = "";

int status = WL_IDLE_STATUS;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SCREEN_SDA 21
#define SCREEN_SCL 22


#define ULTRASOUND_ECHO_PIN 19
#define ULTRASOUND_TRIGGER_PIN 23


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int battery_adc_voltage = 0;
float voltage_value;

String header;
int mode = -1;

void setup() {

  Serial.begin(115200);

  pinMode(ULTRASOUND_TRIGGER_PIN, OUTPUT);  
  pinMode(ULTRASOUND_ECHO_PIN, INPUT);  

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.display();
  drawInfo();
  // // attempt to connect to Wifi network:
  // WiFi.mode(WIFI_STA); 
  // WiFi.begin(ssid, pass);

  // Serial.println("\nConnecting");

  // while(WiFi.status() != WL_CONNECTED){
  //     Serial.print(".");
  //     delay(100);
  // }

  // Serial.println("\nConnected to the WiFi network");
  // Serial.print("Local ESP32 IP: ");
  // Serial.println(WiFi.localIP());

  // put your setup code here, to run once:
  delay(1000);

}

long duration = 0l;  
int distance = 0;  
long currentTime = millis();

void loop() {

  if(currentTime + 5000 > millis()) { return; }
  currentTime = millis();
  measureDistance();
  drawInfo();
}

float maxDistance = 30.0; 

void drawInfo() {
  display.clearDisplay();

  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner
  display.cp437(true);          // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  display.write("Measured Distance");
  display.setCursor(0, 10);
  display.write(String(distance).c_str());
  display.write("cm");
  display.setCursor(0, 20);
  display.write("Full percentage");
  display.setCursor(0, 30);
  float percentage = distance / maxDistance;
  display.write(String(percentage).c_str());
  display.write("%");
  display.display();
  delay(2000);
}



void measureDistance() {
  digitalWrite(ULTRASOUND_TRIGGER_PIN,HIGH);  
  delayMicroseconds(10);  
  digitalWrite(ULTRASOUND_TRIGGER_PIN,LOW);  
  duration=pulseIn(ULTRASOUND_ECHO_PIN,HIGH);  
  distance = duration*0.017;  
  Serial.print(distance);
  Serial.println(" cm");
}


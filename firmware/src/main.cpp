#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Time.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ESP32Time rtc(7200);
int selected = 0;
long timeOfSetting = 0;
bool alarmHasStarted=false;
int howManyMinutes = 0;
int chosenMenu = 0;
void drawMainMenu() {
  chosenMenu=0;
   display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.println("gClock");
  display.setTextSize(1);
 display.drawRect(7,20, 60,20, WHITE);
  display.setTextColor(WHITE);
  display.setCursor(10, 25);
  display.print("Set alarm");
  display.drawRect(7,43, 60,20, WHITE);
  display.setTextColor(WHITE);
  display.setCursor(10, 47);
  display.print("Rem alarm");
  display.display();
}

void selectSetAlarm(){
  
    selected=0;
    display.setTextSize(1);
    display.fillRect(7,43, 60,20, BLACK);
    display.drawRect(7,43, 60,20, WHITE);
  display.setTextColor(WHITE);
  display.setCursor(10, 47);
  display.print("Rem alarm");
 display.fillRect(7,20, 60,20, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(10, 25);
  display.print("Set alarm");
  display.display();
 
}

void selectRemAlarm(){
  selected=1;
  display.setTextSize(1);
  display.fillRect(7,20, 60,20, BLACK);
 display.drawRect(7,20, 60,20, WHITE);
  display.setTextColor(WHITE);
  display.setCursor(10, 25);
  display.print("Set alarm");
  display.fillRect(7,43, 60,20, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(10, 47);
  display.print("Rem alarm");
  display.display();
 
}
void setAlarm(){
timeOfSetting = rtc.getLocalEpoch();
alarmHasStarted = true;
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(20,20);
display.print("ALARM HAS ");
display.setCursor(20,30);
display.print("BEEN SET");
display.setCursor(20,40);
display.print("FOR ");
display.print(howManyMinutes);
display.print(" MINUTES");
display.display();
delay(1500);
drawMainMenu();
selectSetAlarm();
}

void remAlarm(){
timeOfSetting = 0;
alarmHasStarted = false;
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(30,30);
display.print("ALARM HAS ");
display.setCursor(30,40);
display.print("BEEN REMOVED");
display.display();
delay(1500);
drawMainMenu();
selectSetAlarm();
}

void changeMinutesMenu(){
  chosenMenu=1;
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(50, 10);
display.print("SET MINUTES");
display.setCursor(50, 40);
display.print(howManyMinutes);
display.display();
}

void changeMinutes(){
 display.fillRect(50, 40, 20, 20, BLACK);
  display.setCursor(50, 40);
display.print(howManyMinutes);
display.display();
}

void setup() {
  Serial.begin(115200);
  
  rtc.setTime(00, 00, 00, 1, 1, 2026);
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);

  drawMainMenu();
  selectSetAlarm();
  
}



void loop() {
  if(alarmHasStarted){
   if((rtc.getLocalEpoch())>=timeOfSetting+howManyMinutes*60){
      tone(8, 2500, 2000);
      alarmHasStarted=false;
    }
  }
  if(digitalRead(2)==LOW && chosenMenu==0){
    selectSetAlarm();
  }else if(digitalRead(3)==LOW && chosenMenu==0){
    selectRemAlarm();
  }
  else if(digitalRead(4)==LOW && chosenMenu==0){
    if(selected==0) {setAlarm();}
    else if(selected==1) {remAlarm();}
  }
  else if(digitalRead(5)==LOW && chosenMenu==0){
    changeMinutesMenu();
  }else if(digitalRead(4)==LOW && chosenMenu==1){
    display.clearDisplay();
    display.setCursor(10,30);
    display.print("Minutes are set");
    display.display();
    delay(2000);
    drawMainMenu();
    selectSetAlarm();
  }
  else if(digitalRead(2)==LOW && chosenMenu==1){
    howManyMinutes=howManyMinutes+1;
    changeMinutes();
    delay(200);
  }
  else if(digitalRead(3)==LOW && chosenMenu==1){
    if(howManyMinutes>0){ 
    howManyMinutes=howManyMinutes-1;
    changeMinutes();
    delay(200);
  }
  }


}
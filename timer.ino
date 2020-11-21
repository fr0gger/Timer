#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

// Variables
int speakerPin = 7;
int button1=11;
int button2=12;
int buttonmenu=10;

unsigned long timerStart, timerElapsed, timerInverse;
float tabatatens = 0; //tens place
int tabataones = 0; //ones place of tabata counter to keep track of how many cycles
volatile boolean timeFlag = false;
volatile boolean timerFlag = false;
boolean TimerHasChanged();
unsigned long start_num = 0; // Number to countdown from - 3,2,1
unsigned long timer_num = 1; //timer number to countdown from

long previousMillis = 0;
long interval = 250; //colon blink interval
long previousMillis2 = 0;
volatile boolean blink = false;

int value = 0;
int menu = 0;
int var = 0;


//-------------SETUP-------------------
// Setup Everything
void setup() {
  #ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");

  matrix.begin(0x70);
  matrix.setBrightness(20);
  pinMode(speakerPin, OUTPUT);
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(buttonmenu,INPUT_PULLUP);

  #endif
}

// ---------------------BUZZER-----------------
void playTone(int tone, int duration) {
  for (long k = 0; k < duration * 1000L; k += tone * 2) {  
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

//-----------------START TIMER ----------------
void StartTimer() {
  timerStart = millis();
  timerFlag = true;
}

//check the time flag
boolean TimeHasChanged()
{
  return timeFlag;
}

//check the timer flag
boolean TimerHasChanged()
{
  return timerFlag;
}

// stop timer
void StopAction(void)
{
  timeFlag = false;
}

//------------------------ 321-go countdown ------------------
void ThreeTwoOne() {
  start_num = 6; // start at 4 - extra second needed for some reason.

  StartTimer();
  while (TimerHasChanged() ) {
    DisplayTimerTime();
    //Blink_Colon();
  }
  playTone(1519,1200);
  return;
}


//------------------------- Display TImer -----------------
void DisplayTimerTime() {
  float h, m, s, ms;
  unsigned long over;
  timerElapsed = millis() - timerStart;  //store elapsed time
  timerInverse = ((start_num * 1000) - timerElapsed);
  if (timerInverse > 900) {
    timerFlag = true;
  }
  else timerFlag = false;
  h = int(timerInverse / 3600000);
  over = timerInverse % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;
  /*Serial.print("Elapsed time: ");
  Serial.print(h, 0);
  Serial.print("h ");
  Serial.print(m, 0);
  Serial.print("m ");
  Serial.print(s, 0);
  Serial.print("s ");
  Serial.print(ms, 0);
  Serial.println("ms");
  Serial.println(); */

  matrix.writeDigitNum(0, m / 10);
  matrix.writeDigitNum(1, int(m) % 10);
  //matrix.drawColon(true);
  matrix.writeDigitNum(3, s / 10);
  matrix.writeDigitNum(4, int(s) % 10);
  matrix.writeDisplay();
  //Blink_Colon();
}


// ----------- START The TIMER-------------
void StartTimerAction(unsigned long start_num2)
{
  Serial.println("start timer action");
  matrix.blinkRate(false);

  ThreeTwoOne(); // 3,2,1 countdown
  start_num = start_num2;//(value * 60) + 1; // assign the toggled timer_num to start_num to begin countdown
  //Serial.println(value);
  //Serial.println(start_num);
  Blink_Colon();
  StartTimer();  // start the timer
  while (TimerHasChanged() ) {
    DisplayTimerTime();
    }
  playTone(1519,2000);
  return;
}

//--------------Start Tabata --------------
void StartTabataAction(int tbtime)
{
  Serial.println("start tabata");
  matrix.blinkRate(false);
  ThreeTwoOne();
  for (int tabatat = 0; tabatat < tbtime; tabatat++) {
    tabataones = (tabatat % 10);
    tabatatens = int(tabatat / 10);

    start_num = 21; // 20 second cycle
    StartTimer();  // start the timer

    while (TimerHasChanged() ) {
      DisplayTabataTime();
      Blink_Colon();
    }
    playTone(1519,500);

    start_num = 11; // 10 second cycle
    StartTimer();  // start the timer

    while (TimerHasChanged() ) {
      DisplayTabataTime();
      Blink_Colon();
    }
      playTone(1519,500);
  }
  playTone(1519,1500);

  matrix.blinkRate(true);
  while (var < 200) {
  }
  //loop();
}

//----------------START EMOM--------- 
void EMOM(int tbtime){
  Serial.println("start EMOM");
  matrix.blinkRate(false);
  ThreeTwoOne();
  for (int tabatat = tbtime -1; tabatat > -1; tabatat--) {
    tabataones = (tabatat % 10);
    tabatatens = int(tabatat / 10);
    start_num = 61; // 20 second cycle
    StartTimer();  // start the timer

    while (TimerHasChanged() ) {
      DisplayEMOMTime();
      Blink_Colon();
    }
    playTone(1519,500);
  }
   playTone(1519,1500);
  
}


//------------DISPLAY EMOM----------------
void DisplayEMOMTime() {
  float h, m, s, ms;
  unsigned long over;
  timerElapsed = millis() - timerStart;  //store elapsed time
  //Serial.println(timerElapsed);
  timerInverse = ((start_num * 1000) - timerElapsed);
  if (timerInverse > 900) {
    timerFlag = true;
  }
  else timerFlag = false;
  h = int(timerInverse / 3600000);
  over = timerInverse % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;
  //Serial.println(tabataones);
  
  matrix.writeDigitNum(0, tabatatens);
  matrix.writeDigitNum(1, tabataones);
  //matrix.drawColon(true);
  matrix.writeDigitNum(3, s / 10);
  matrix.writeDigitNum(4, int(s) % 10);
  matrix.writeDisplay();
  //Blink_Colon();
}


//--------------------DISPLAY TABATA ---
void DisplayTabataTime() {
  float h, m, s, ms;
  unsigned long over;
  timerElapsed = millis() - timerStart;  //store elapsed time
  timerInverse = ((start_num * 1000) - timerElapsed);
  if (timerInverse > 900) {
    timerFlag = true;
  }
  else timerFlag = false;
  h = int(timerInverse / 3600000);
  over = timerInverse % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;

  matrix.writeDigitNum(0, tabatatens);
  matrix.writeDigitNum(1, tabataones);
  //matrix.drawColon(true);
  matrix.writeDigitNum(3, s / 10);
  matrix.writeDigitNum(4, int(s) % 10);
  matrix.writeDisplay();
  //Blink_Colon();
}

//------------------ POMODORO TIMER ------------------
void pomodoro(int tbtime)
{
  Serial.println("start pomodoro action");
  matrix.blinkRate(false);
  ThreeTwoOne();
  matrix.print(2500);
  for (int tabatat = 0; tabatat < tbtime; tabatat++) {
    tabataones = (tabatat % 10);
    tabatatens = int(tabatat / 10);

    start_num = 1501; // 20 second cycle
    StartTimer();  // start the timer

    while (TimerHasChanged() ) {
      pomodorotime();
      Blink_Colon();
    }
    playTone(1519,500);

    start_num = 300; // 10 second cycle
    StartTimer();  // start the timer

    while (TimerHasChanged() ) {
      pomodorotime();
      Blink_Colon();
    }
      playTone(1519,500);
  }
  playTone(1519,1500);

  matrix.blinkRate(true);
  while (var < 200) {
      
  }
}

//------------- Pomodoro Timer 25 min 5 min break-----------
void pomodorotime() {
  float h, m, s, ms;
  unsigned long over;
  timerElapsed = millis() - timerStart;  //store elapsed time
  timerInverse = ((start_num * 1000) - timerElapsed);
  if (timerInverse > 900) {
    timerFlag = true;
  }
  else timerFlag = false;
  h = int(timerInverse / 3600000);
  over = timerInverse % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;
  
  matrix.writeDigitNum(0, m/10);
  matrix.writeDigitNum(1, int(m) % 10);
  matrix.drawColon(true);
  matrix.writeDigitNum(3, s / 10);
  matrix.writeDigitNum(4, int(s) % 10);
  matrix.writeDisplay();
  //Blink_Colon();
}



// -----------------------MENU 1---------------
void menu1() {
  while (var < 200) {

   Serial.println("Enter MENU 1");
  // Select time
  if(digitalRead(button1) == LOW) {
     matrix.blinkRate(2);
     Serial.println("PRESSED");
     value++;
     start_num = (value * 30); // assign the toggled timer_num to start_num to begin countdown
     StartTimer();  // start the timer
     //while (TimerHasChanged() ) {
     DisplayTimerTime();
     //blink colon
     matrix.writeDigitRaw (2, 0x02);
     matrix.writeDisplay();  
     delay(200);
   } 
  
  if(digitalRead(button2) == LOW) {
     Serial.println("PRESSED");
     value--;
     start_num = (value * 30);
     StartTimer();  // start the timer

     if (value < 0 or start_num == 4294967266 ) {
        value = 0;
        start_num  = 0;
  
     }
     DisplayTimerTime();
     matrix.writeDigitRaw (2, 0x02);
     matrix.writeDisplay();
     delay(200);
  }
  
  if(digitalRead(buttonmenu) == LOW) {
       StartTimerAction(start_num);
        delay(100);
     }
   }
}


//-------------------MENU2------------------

void menu2() {


     matrix.print(0000, DEC);
     matrix.writeDisplay();
     delay(100);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     matrix.writeDigitRaw(3, B10000000);
     matrix.blinkRate(2);
     matrix.drawColon(false);
     matrix.writeDisplay();

  while (var < 200) {
  Serial.println("Enter MENU 2");
  // Select time
  if(digitalRead(button1) == LOW) {
     matrix.blinkRate(2);
  
     Serial.println("PRESSED");
     value++;
     start_num = value; // assign the toggled timer_num to start_num to begin countdown
     matrix.print(value);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     matrix.writeDisplay();  
     delay(200);
   } 

   if(digitalRead(button2) == LOW) {
     Serial.println("PRESSED");
     value--;
     start_num = value;
     matrix.print(value);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);   
     if (value < 0 or start_num == 4294967266 ) {
        value = 0;
        start_num  = 0;
        matrix.writeDigitNum(4, start_num);
     }

     matrix.writeDisplay();  
     delay(200);
  }
  
  if(digitalRead(buttonmenu) == LOW) {
       Serial.println("Start timer");
       StartTabataAction(start_num);
       delay(100);
     }
  }
}


//------------------ MENU3-------------------

void menu3() {
     matrix.print(0000, DEC);
     matrix.writeDisplay();
     delay(100);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     matrix.writeDigitRaw(3, B10000000);
     matrix.blinkRate(2);
     matrix.drawColon(false);
     matrix.writeDisplay();

  while (var < 200) {

   Serial.println("Enter MENU 3");
  // Select time
  if(digitalRead(button1) == LOW) {
     matrix.blinkRate(2);
    
     Serial.println("PRESSED");
     value++;
     start_num = value; // assign the toggled timer_num to start_num to begin countdown
     matrix.print(value);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     matrix.writeDisplay();  
     delay(200);
   } 

   if(digitalRead(button2) == LOW) {
     Serial.println("PRESSED");
     value--;
     start_num = value;
     matrix.print(value);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     
     if (value < 1 or start_num == 4294967266 ) {
        value = 1;
        start_num  = 1;
        matrix.writeDigitNum(4, start_num);
     }

     matrix.writeDisplay();  
     delay(200);
  }
  
  if(digitalRead(buttonmenu) == LOW) {
       Serial.println("Start timer");
       EMOM(start_num);
       delay(100);
       
     }
  }
}


//--------------- MENU4--------------

void menu4() {
     matrix.print(0000, DEC);
     matrix.writeDisplay();
     delay(100);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     matrix.writeDigitRaw(3, B10000000);
     matrix.blinkRate(2);
     matrix.drawColon(false);
     matrix.writeDisplay();

  while (var < 200) {

   Serial.println("Enter MENU 4");
  // Select time
  if(digitalRead(button1) == LOW) {
     matrix.blinkRate(2);
  
     Serial.println("PRESSED");
     value++;
     start_num = value; // assign the toggled timer_num to start_num to begin countdown
     matrix.print(value);
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     matrix.writeDisplay();  
     delay(200);
   } 

   if(digitalRead(button2) == LOW) {
     Serial.println("PRESSED");
     value--;
     start_num = value;
     matrix.print(value);   
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01111100);
     
     if (value < 1 or start_num == 4294967266 ) {
        value = 1;
        start_num  = 1;
        matrix.writeDigitNum(4, start_num);
     }
     matrix.writeDisplay();  
     delay(200);
  }
   
  if(digitalRead(buttonmenu) == LOW) {
       Serial.println("Start timer");
       pomodoro(start_num);
       delay(100);
     }
  }
}


//-------------MAIN---------------- 
void loop() {
  
  matrix.print(10000, DEC);
  matrix.writeDisplay();
  delay(800);
  
while (var < 200) {
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01010100);
     matrix.writeDigitRaw(3, B10000000);
     //matrix.blinkRate(2);
     matrix.drawColon(false);
     matrix.writeDisplay();
          
  // menu 1: timer, menu 2: Tabata; Menu 3 EMOM, Menu 4 Pomodoro
  //Choose menu
 if(digitalRead(button1) == LOW) {
     Serial.println("PRESSED");
     menu++;

     if (menu > 4) {
         Serial.println("not above ");
         menu = 4;
     }
     
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01010100);
     //matrix.blinkRate(2);
     matrix.writeDigitNum(4, menu);
     matrix.drawColon(false);
     matrix.writeDisplay();
     delay(200);
   }
     
     
   if(digitalRead(button2) == LOW) {
     Serial.println("PRESSED");
     menu--;

     if (menu < 1) {
         Serial.println("not low ");
         menu = 1;
     }
     matrix.writeDigitRaw(0, B01010100);
     matrix.writeDigitRaw(1, B01010100);
     //matrix.blinkRate(2);
     matrix.writeDigitNum(4, menu);
     matrix.drawColon(false);
     matrix.writeDisplay();
     delay(200);
   }
     
   if(digitalRead(buttonmenu) == LOW) {
       Serial.println("Validation Pressed");
       if(menu == 1) {
         Serial.println("Menu 1");
         playTone(1200,300);
         matrix.blinkRate(4);
         matrix.blinkRate(false);

         matrix.print(10000, DEC);
         matrix.writeDisplay();
         delay(300);
         matrix.print(0000, DEC);
         matrix.drawColon(true);
         matrix.writeDisplay();
         
         menu1();
         
       } else if (menu == 2) {
         Serial.println("menu2 ");
         playTone(1200,300);
         menu2();
         
       } else if (menu == 3) {
         Serial.println("menu3 ");
         playTone(1200,300);
         menu3();
         delay(100);
       } else if (menu == 4) {
         Serial.println("menu4 ");
         playTone(1200,300);
         menu4();
         delay(100);
         
       } 
     }
    delay(100);      
   }
}

void Blink_Colon() { // "blink without delay" function to blink the colon while clock is running

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    // save the last time you blinked
    previousMillis = currentMillis;

    // if the colon is off turn it on and vice-versa:
    if (blink == false) {
      matrix.writeDigitRaw (2, 0x02);
      matrix.writeDisplay();
      blink = true;
      
    }
    else {
      matrix.writeDigitRaw (2, 0x00);
      matrix.writeDisplay();
      blink = false;
    }
  }
}

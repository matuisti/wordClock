#include <Adafruit_NeoPixel.h>
#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>

#define RGBLEDPIN    6
#define FWDButtonPIN 2
#define REVButtonPIN 3
#define LED13PIN    13



#define N_LEDS 169 // 13 x 13 grid
#define TIME_HEADER  "T"   // Header tag for serial time sync message
//#define BRIGHTNESSDAY 50 // full on 200
#define BRIGHTNESSNIGHT 20

Adafruit_NeoPixel grid = Adafruit_NeoPixel(N_LEDS, RGBLEDPIN, NEO_GRB + NEO_KHZ800);

int brightnessPin = A0;
int brightness = 0;
int brightnessVal;
int BRIGHTNESSDAY = 70; //Starting brightness
int intBrightness = BRIGHTNESSDAY; // the brightness of the clock (0 = off and 255 = 100%)
int intTestMode; // set when both buttons are held down
String strTime = ""; // used to detect if word time has changed
int intTimeUpdated = 0; // used to tell if we need to turn on the brightness again when words change

// a few colors
uint32_t colorWhite = grid.Color(255, 255, 255);
uint32_t colorBlack = grid.Color(0, 0, 0);
uint32_t colorRed = grid.Color(255, 0, 0);
uint32_t colorOrange = grid.Color(255, 120, 0);
uint32_t colorGreen = grid.Color(150, 200, 0);
uint32_t colorBlue = grid.Color(0, 0, 50);
uint32_t colorJGreen = grid.Color(50, 179, 30);
uint32_t colorVaaleanPunainen = grid.Color(255, 100, 140);
uint32_t colorLightBlue = grid.Color(88, 255, 250);

// the words
int arrK[] = {168, -1};
int arrULT[] = {143, 142, 141, -1};
int arrA[] = {118, -1};
int arrELLO[] = {167, 166, 165, 164, -1};
int arrLOTTA[] = {163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, -1};
int arrMINA[] = {144, 145, 146, 147, -1};
int arrON[] = {140, 139, -1};
int arrHUOMENTA[] = {130, 131, 132, 133, 134, 135, 136, 137, -1};
int arrVARTTIA[] = {117, 119, 120, 121, 122, 123, -1};
int arrPUOLI[] = {125, 126, 127, 128, 129, -1};
int arrMKAKSI[] = {116, 115, 114, 113, 112, -1};
int arrKYMMENTA[] = {111, 110, 109, 108, 107, 106, 105, 104, -1};
int arrMVIISI[] = {91, 92, 93, 94, 95, -1};
int arrRAKASTAN[] = {96, 97, 98, 99, 100, 101, 102, 103, -1};
int arrVAILLE[] = {90, 89, 88, 87, 86, 85, -1};
int arrYLI[] = {84, 83, 82, -1};
int arrYKSI[] = {81, 80, 79, 78, -1};
int arrKAKSI[] = {67, 68, 69, 70, 71, -1};
int arrTOISTA[] = {72, 73, 74, 75, 76, 77, -1};
int arrVIISI[] = {64, 63, 62, 61, 60, -1};
int arrHEI[] = {59, 58, 57, -1};
int arrKUUSI[] = {56, 55, 54, 53, 52, -1};
int arrSEITSEMAN[] = {39, 40, 41, 42, 43, 44, 45, 46, -1};
int arrN[] = {47, -1};
int arrELJA[] = {48, 49, 50, 51, -1};
int arrKAHDEKSAN[] = {38, 37, 36, 35, 34, 33, 32, 31, 30, -1};
int arrMURU[] = {29, 28, 27, 26, -1};
int arrKOLME[] = {13, 14, 15, 16, 17, -1};
int arrKYMMENEN[] = {18, 19, 20, 21, 22, 23, 24, 25, -1};
int arrSINUA[] = {12, 11, 10, 9, 8, -1};
int arrYHDEKSAN[] = {7, 6, 5, 4, 3, 2, 1, 0, -1};
//int arrSYDANISO[] = {45,57,73,81,100,108,109,97,111,112,94,87,69,59,-1};
//int arrSYDANPIENI[] = {58,72,82,98,84,96,86,70,-1};
//int arrM[] = {108,125,134,126,106,128,130,129,104,-1};
//int arrL[] = {10,11,12,13,38,39,-1};

void setup() {
  // AIKA TERMINAALISTA: date +T%s > /dev/cu.wchusbserial1420
  Serial.begin(9600);
  //  while(!Serial); // Needed for Leonardo only
  delay(200);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  setSyncInterval(60); // sync the time every 60 seconds (1 minutes)
  if (timeStatus() != timeSet) {
    Serial.println("Unable to sync with the RTC");
    RTC.set(1406278800);   // set the RTC to Jul 25 2014 9:00 am
    setTime(1406278800);
  } else {
    Serial.println("RTC has set the system time");
  }
  // print the version of code to the console


  // setup the LED strip
  grid.begin();
  grid.show();

  // set the bright ness of the strip
  grid.setBrightness(intBrightness);

  colorWipe(colorBlack, 0);
  spellWord(arrHEI, colorOrange);
  delay(1000);
  spellWord(arrHEI, colorWhite);
  delay(1000);
  fadeOut(10);
  colorWipe(colorBlack, 0);

  paintWord(arrLOTTA, colorVaaleanPunainen );
  fadeIn(20);
  delay(1000);
  fadeOut(20);
  colorWipe(colorBlack, 0);

  // set the bright ness of the strip
  grid.setBrightness(intBrightness);
  delay(1000);
  LOVE();
  delay(200);
  //MURU();

  colorWipe(colorBlack, 0);
  grid.setBrightness(intBrightness);

  // initialize the onboard led on pin 13
  pinMode(LED13PIN, OUTPUT);

  // initialize the buttons

  pinMode(FWDButtonPIN, INPUT);
  pinMode(REVButtonPIN, INPUT);
  // lets kick off the clock
  digitalClockDisplay();

}

void loop() {
  
    brightness = analogRead(brightnessPin);
    brightnessVal = (brightness / 7) + 10;
    BRIGHTNESSDAY = brightnessVal;
    
    if (brightnessVal < 10) {
      brightnessVal = 10;       
     }
  
  // if there is a serial connection lets see if we need to set the time
  if (Serial.available()) {
    time_t t = processSyncMessage();
    if (t != 0) {
      Serial.print("Time set via connection to: ");
      Serial.print(t);
      Serial.println();
      RTC.set(t);   // set the RTC and the system time to the received value
      setTime(t);
    }
  }
  // check to see if the time has been set
  if (timeStatus() == timeSet) {
    // time is set lets show the time

    if ((hour() > 5) && (hour() < 9)) {
      intBrightness =  BRIGHTNESSDAY;
      paintWord(arrHUOMENTA, colorLightBlue);
      paintWord(arrMURU, colorLightBlue);
    }
    else if ((hour() > 8) && (hour() < 20)) {
      intBrightness =  BRIGHTNESSDAY;
      paintWord(arrHUOMENTA, colorBlack);
      paintWord(arrMURU, colorBlack);
    }
    else {
      intBrightness = BRIGHTNESSNIGHT;
      paintWord(arrHUOMENTA, colorBlack);
    }
    grid.setBrightness(intBrightness);

    // test to see if both buttons are being held down
    // if so  - start a self test till both buttons are held down again.
    if ((digitalRead(FWDButtonPIN) == HIGH) && (digitalRead(REVButtonPIN) == HIGH)) {
      intTestMode = !intTestMode;
      if (intTestMode) {
        Serial.println("Selftest Mode TRUE");
        // run through a quick test
        test_grid();
      } else {
        Serial.println("Selftest mode FALSE");
      }
    }
    // test to see if a forward button is being held down for time setting
    if (digitalRead(FWDButtonPIN) == HIGH) {
      digitalWrite(LED13PIN, HIGH);
      Serial.println("Kelloa siirretty 5 min eteenpäin");
      incrementTime(300);
      delay(100);
      digitalWrite(LED13PIN, LOW);
    }

    // test to see if the back button is being held down for time setting
    if (digitalRead(REVButtonPIN) == HIGH) {
      digitalWrite(LED13PIN, HIGH);
      Serial.println("Kelloa siirretty 5 min taaksepäin");
      incrementTime(-300);
      delay(100);
      digitalWrite(LED13PIN, LOW);

    }

    // and finaly we display the time (provided we are not in self tes mode
    if (!intTestMode) {
      displayTime();
    }
  } else {
    colorWipe(colorBlack, 0);
    paintWord(arrYKSI, colorRed);
    delay(1000);
    paintWord(arrYKSI, colorGreen);
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(100);
  }
  delay(1000);
}

void incrementTime(int intSeconds) {
  // increment the time counters keeping care to rollover as required
  if (timeStatus() == timeSet) {
    Serial.print("adding ");
    Serial.print(intSeconds);
    Serial.println(" seconds to RTC");
    //    colorWipe(colorBlack, 0);
    adjustTime(intSeconds);
    RTC.set(now() + intSeconds);
    digitalClockDisplay();
    displayTime();
  }
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print("-");
  Serial.print(month());
  Serial.print("-");
  Serial.print(year());
  Serial.println();
}

void displayTime() {
  String strCurrentTime; // build the current time
  //colorWipe(colorBlack, 0);
  paintWord(arrLOTTA, colorJGreen);
  // Now, turn on the "It is" leds
  paintWord(arrK, colorWhite);
  paintWord(arrELLO, colorWhite);
  delay(200);
  // if the time IS one of the following IS is green
  if ((minute() == 0)
      |  (minute() == 5)
      | (minute() == 10)
      | (minute() == 15)
      | (minute() == 20)
      | (minute() == 25)
      | (minute() == 30)
      | (minute() == 35)
      | (minute() == 40)
      | (minute() == 45)
      | (minute() == 50)
      | (minute() == 55)) {
    paintWord(arrON, colorJGreen);
  } else {
    paintWord(arrON, colorWhite);
  }
  if ((minute() > 0) && (minute() < 5)) {
    // FIVE MINUTES
    strCurrentTime = "tasan ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorBlack);
  }
  // now we display the appropriate minute counter
  if ((minute() > 4) && (minute() < 10)) {
    // FIVE MINUTES
    strCurrentTime = "five ";
    paintWord(arrMVIISI, colorWhite);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorBlack);
  }
  if ((minute() > 9) && (minute() < 15)) {
    //TEN MINUTES;
    strCurrentTime = "ten ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorWhite);
  }
  if ((minute() > 14) && (minute() < 20)) {
    // QUARTER
    strCurrentTime = "a quarter ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorWhite);
    paintWord(arrA, colorWhite);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorBlack);
  }
  if ((minute() > 19) && (minute() < 25)) {
    //TWENTY MINUTES
    strCurrentTime = "twenty ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorWhite);
    paintWord(arrKYMMENTA, colorWhite);
  }
  if ((minute() > 24) && (minute() < 30)) {
    //TWENTY FIVE
    strCurrentTime = "twenty five ";
    paintWord(arrMVIISI, colorWhite);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorWhite);
    paintWord(arrKYMMENTA, colorWhite);
  }
  if ((minute() > 29) && (minute() < 35)) {
    strCurrentTime = "half ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorOrange);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorBlack);

  }
  if ((minute() > 34) && (minute() < 40)) {
    //TWENTY FIVE
    strCurrentTime = "twenty five ";
    paintWord(arrMVIISI, colorWhite);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorWhite);
    paintWord(arrKYMMENTA, colorWhite);

  }
  if ((minute() > 39) && (minute() < 45)) {
    strCurrentTime = "twenty ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorWhite);
    paintWord(arrKYMMENTA, colorWhite);
  }
  if ((minute() > 44) && (minute() < 50)) {
    strCurrentTime = "a quarter ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorWhite);
    paintWord(arrA, colorWhite);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorBlack);
  }
  if ((minute() > 49) && (minute() < 55)) {
    strCurrentTime = "ten ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorWhite);
  }
  if ((minute() > 54) && (minute() < 59)) {
    strCurrentTime = "five ";
    paintWord(arrMVIISI, colorWhite);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorBlack);
  }
  if (minute() == 0) {
    strCurrentTime = "00 ";
    paintWord(arrMVIISI, colorBlack);
    paintWord(arrVARTTIA, colorBlack);
    paintWord(arrA, colorBlack);
    paintWord(arrPUOLI, colorBlack);
    paintWord(arrMKAKSI, colorBlack);
    paintWord(arrKYMMENTA, colorBlack);
  }

  if ((minute() < 5)) {
    switch (hour()) {
      case 1:
      case 13:
        strCurrentTime = strCurrentTime + "one ";
        paintWord(arrYKSI, colorWhite);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 2:
      case 14:
        strCurrentTime = strCurrentTime + "two ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorWhite);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 3:
      case 15:
        strCurrentTime = strCurrentTime + "three ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorWhite);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 4:
      case 16:
        strCurrentTime = strCurrentTime + "four ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorWhite);
        paintWord(arrELJA, colorWhite);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 5:
      case 17:
        strCurrentTime = strCurrentTime + "five ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorWhite);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 6:
      case 18:
        strCurrentTime = strCurrentTime + "six ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorWhite);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 7:
      case 19:
        strCurrentTime = strCurrentTime + "seven ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorWhite);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorWhite);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 8:
      case 20:
        strCurrentTime = strCurrentTime + "eight ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorWhite);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 9:
      case 21:
        strCurrentTime = strCurrentTime + "nine ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorWhite);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 10:
      case 22:
        strCurrentTime = strCurrentTime + "ten ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorWhite);
        paintWord(arrTOISTA, colorBlack);

        break;
      case 11:
      case 23:
        strCurrentTime = strCurrentTime + "eleven ";
        paintWord(arrYKSI, colorWhite);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorWhite);

        break;
      case 0:
      case 12:
      case 24:
        strCurrentTime = strCurrentTime + "twelve ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorWhite);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorWhite);

        break;
    }
    strCurrentTime = strCurrentTime + "oclock ";
    paintWord(arrYLI, colorBlack);
    paintWord(arrVAILLE, colorBlack);
  } else if ((minute() < 30) && (minute() > 4)) {
    strCurrentTime = strCurrentTime + "past ";
    paintWord(arrYLI, colorWhite);
    paintWord(arrVAILLE, colorBlack);

    switch (hour()) {
      case 1:
      case 13:
        strCurrentTime = strCurrentTime + "one ";
        paintWord(arrYKSI, colorWhite);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 2:
      case 14:
        strCurrentTime = strCurrentTime + "two ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorWhite);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 3:
      case 15:
        strCurrentTime = strCurrentTime + "three ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorWhite);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 4:
      case 16:
        strCurrentTime = strCurrentTime + "four ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorWhite);
        paintWord(arrELJA, colorWhite);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 5:
      case 17:
        strCurrentTime = strCurrentTime + "five ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorWhite);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 6:
      case 18:
        strCurrentTime = strCurrentTime + "six ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorWhite);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 7:
      case 19:
        strCurrentTime = strCurrentTime + "seven ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorWhite);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorWhite);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 8:
      case 20:
        strCurrentTime = strCurrentTime + "eight ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorWhite);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 9:
      case 21:
        strCurrentTime = strCurrentTime + "nine ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorWhite);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 10:
      case 22:
        strCurrentTime = strCurrentTime + "ten ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorWhite);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 11:
      case 23:
        strCurrentTime = strCurrentTime + "eleven ";
        paintWord(arrYKSI, colorWhite);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorWhite);
        break;
      case 0:
      case 12:
      case 24:
        strCurrentTime = strCurrentTime + "twelve ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorWhite);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorWhite);
        break;
    }
  } else {

    strCurrentTime = strCurrentTime + "o clock ";
    paintWord(arrYLI, colorBlack);

    switch (hour()) {
      case 1:
      case 13:
        strCurrentTime = strCurrentTime + "two ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorWhite);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 14:
      case 2:
        strCurrentTime = strCurrentTime + "three ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorWhite);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 15:
      case 3:
        strCurrentTime = strCurrentTime + "four ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorWhite);
        paintWord(arrELJA, colorWhite);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 4:
      case 16:
        strCurrentTime = strCurrentTime + "five ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorWhite);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 5:
      case 17:
        strCurrentTime = strCurrentTime + "six ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorWhite);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 6:
      case 18:
        strCurrentTime = strCurrentTime + "seven ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorWhite);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorWhite);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 7:
      case 19:
        strCurrentTime = strCurrentTime + "eight ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorWhite);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 8:
      case 20:
        strCurrentTime = strCurrentTime + "nine ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorWhite);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 9:
      case 21:
        strCurrentTime = strCurrentTime + "ten ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorWhite);
        paintWord(arrTOISTA, colorBlack);
        break;
      case 10:
      case 22:
        strCurrentTime = strCurrentTime + "eleven ";
        paintWord(arrYKSI, colorWhite);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorWhite);
        break;
      case 11:
      case 23:
        strCurrentTime = strCurrentTime + "twelve ";
        paintWord(arrYKSI, colorBlack);
        paintWord(arrKAKSI, colorWhite);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorWhite);
        break;
      case 0:
      case 12:
      case 24:
        strCurrentTime = strCurrentTime + "one ";
        paintWord(arrYKSI, colorWhite);
        paintWord(arrKAKSI, colorBlack);
        paintWord(arrKOLME, colorBlack);
        paintWord(arrN, colorBlack);
        paintWord(arrELJA, colorBlack);
        paintWord(arrVIISI, colorBlack);
        paintWord(arrKUUSI, colorBlack);
        paintWord(arrSEITSEMAN, colorBlack);
        paintWord(arrKAHDEKSAN, colorBlack);
        paintWord(arrYHDEKSAN, colorBlack);
        paintWord(arrKYMMENEN, colorBlack);
        paintWord(arrTOISTA, colorBlack);
        break;
    }
  }

  if (minute() > 34) {
    strCurrentTime = strCurrentTime + "past ";
    paintWord(arrVAILLE, colorWhite);

    if (strCurrentTime != strTime) {

      strTime = strCurrentTime;
      if (strTime == "") {
      }
    }

    else {
      grid.show();
    }
  }
}

void LOVE() {
  delay(100);
  spellWord(arrMINA, colorVaaleanPunainen);
  spellWord(arrRAKASTAN, colorJGreen);
  spellWord(arrSINUA, colorVaaleanPunainen);
  //spellWord(arrRAKASTAN, colorRed);
  delay(400);
  paintWord(arrLOTTA, colorJGreen);
  delay(400);
  paintWord(arrLOTTA, colorBlack);
  delay(400);
  paintWord(arrLOTTA, colorJGreen);
  delay(2000);

  chase(colorWhite, 3);
  colorWipe(colorBlack, 0);

}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void rainbow(uint8_t wait) {
  //secret rainbow mode
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < grid.numPixels(); i++) {
      grid.setPixelColor(i, Wheel((i + j) & 255));
    }
    grid.show();
    delay(wait);
  }
}

static void chase(uint32_t color, uint8_t wait) {
  for (uint16_t i = 0; i < grid.numPixels() + 4; i++) {
    grid.setPixelColor(i  , color); // Draw new pixel
    grid.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    grid.show();
    delay(wait);
  }
}

void fadeOut(int time) {
  for (int i = intBrightness; i > 0; --i) {
    grid.setBrightness(i);
    grid.show();
    delay(time);
  }
}

void fadeIn(int time) {
  for (int i = 1; i < intBrightness; ++i) {
    grid.setBrightness(i);
    grid.show();
    delay(time);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, uint8_t wait) {
  for (uint16_t i = 0; i < grid.numPixels(); i++) {
    grid.setPixelColor(i, color);
  }
  grid.show();
  delay(wait);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return grid.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return grid.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return grid.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void paintWord(int arrWord[], uint32_t intColor) {
  for (int i = 0; i < grid.numPixels() + 1; i++) {
    if (arrWord[i] == -1) {
      grid.show();
      break;
    } else {
      grid.setPixelColor(arrWord[i], intColor);
    }
  }
}

void spellWord(int arrWord[], uint32_t intColor) {
  for (int i = 0; i < grid.numPixels() + 1; i++) {
    if (arrWord[i] == -1) {
      break;
    } else {
      grid.setPixelColor(arrWord[i], intColor);
      grid.show();
      delay(400);
    }
  }
}


unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    pctime = pctime - 18000;
    return pctime;
    if ( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
      pctime = 0L; // return 0 to indicate that the time is not valid
    }
    Serial.println();
    Serial.println("Time Set via Serial");
    Serial.println();
  }
  return pctime;
}

// runs throught the various displays, testing

void test_grid() {

  colorWipe(colorBlack, 0);
  spellWord(arrHEI, colorJGreen);
  delay(1000);
  fadeOut(50);
  grid.setBrightness(intBrightness);
  paintWord(arrHEI, colorWhite);
  delay(1000);
  fadeOut(50);
  grid.setBrightness(intBrightness);
  colorWipe(colorJGreen, 50);
  chase(colorRed, 2); // Red
  chase(colorGreen, 2); // Green
  chase(colorBlue, 2); // Blue
  chase(colorWhite, 2); // White
  colorWipe(colorBlack, 0);
  paintWord(arrELLO, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrMINA, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrON, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrPUOLI, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrVARTTIA, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrYLI, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrVAILLE, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrYKSI, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrKAKSI, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrKOLME, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrELJA, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrMVIISI, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrVIISI, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrKUUSI, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrSEITSEMAN, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrKAHDEKSAN, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrYHDEKSAN, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrRAKASTAN, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrKYMMENEN, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrTOISTA, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrTOISTA, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrKYMMENTA, colorWhite);
  grid.show();
  delay(1000);
  colorWipe(colorBlack, 0);
  paintWord(arrLOTTA, colorJGreen);
  grid.show();
  delay(2000);
  paintWord(arrON, colorWhite);
  grid.show();
  delay(1000);
  fadeOut(100);
  colorWipe(colorBlack, 0);
  grid.setBrightness(intBrightness);
  intTestMode = !intTestMode;
}


/*
  Advanced Example: Controlling the activation of a 8x1 MUX using serial commands lib

  The library: https://github.com/dzalf/CD4051-MAX4617_Analog_Multiplexer_Library is required


*/


#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <MUX.h>
#include <serialCommand.h>

#define INSTR_LEN 4

bool stringFinished = false;    // Flag to indicate reception of a string after terminator is reached
bool serialTail = false;        // Flag to detect "governator"
char serialString[] = " - ";        // Empty serial string variable for the input command in the form: INST_XXXX. In this case, the isntruction is 4 chars long
// and the variable is 4 chars long. The divisory chars is an underscore: '_'
bool waitFlag = false;

int j = 1;    // Loop variable
byte channels;        // Variable to point at the channels
int pwm = 0;

const int freqOut = 10;  // Pin that sends the PWM signal

MUX mux(7, 6, 5, 4); // Instantiate a MUX object. Use pins [14, 15, 16] as [C, B, A] from the uController
U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 52, /* data=*/ 51, /* cs=*/ 53, /* dc=*/ 49, /* reset=*/ 48); // Using a 128x64 OLED screen
serialCommand serialCOM(false);    // Instantiation of new object. No debugging selected

void setup() {

  Serial.begin(115200);

  serialCOM.begin();
  serialCOM.instrLength = INSTR_LEN;

  mux.setup();
  mux.disable();

  u8g2.begin();
  channels = 0;
  pinMode(freqOut, OUTPUT);

}

void loop() {

  // if(Serial.available() > 0); serialEvent(); // only needed for Arduino Leonardo

  if (stringFinished) {

    waitFlag = false;
    stringFinished = false;

    Serial.print("Serial says: ");
    Serial.println(serialString);

    if (serialCOM.equal(serialString, "GAIN")) {

      Serial.println("Gain will be changed!");

      int gain = serialCOM.getValue(serialString);

      Serial.print("gain = ");
      Serial.println(gain);

    } else if (serialCOM.equal(serialString, "MUXE")) {

      Serial.println("Mux enabled");

      mux.enable();

    } else if (serialCOM.equal(serialString, "MUXD")) {

      Serial.println("Mux disabled");

      mux.disable();

    } else if (serialCOM.equal(serialString, "PWMV")) {

      Serial.println("PWM to be adjusted!");

      pwm = serialCOM.getValue(serialString);

      analogWrite(freqOut, pwm);

      u8g2.drawStr(20, 51, "PWM Changed");

      Serial.print("pwm = ");
      Serial.println(pwm);

    }  else if (serialCOM.equal(serialString, "CHAN")) {

      Serial.println("Chan selected");

      int ch = serialCOM.getValue(serialString);

      Serial.print("chan = ");
      Serial.println(ch);
      mux.selectChannel(ch);

    } else {

      Serial.println("No action will be taken...");


    }

  }


  u8g2.firstPage();

  do {
    u8g2.drawRFrame(0, 0, 128, 64, 7);
    u8g2.setFont(u8g2_font_6x12_t_cyrillic);
    u8g2.drawStr(15, 10, "Serial Commander");
    u8g2.drawHLine(1, 14, 127);

    u8g2.drawStr(5, 25, "Serial CMD: ");
    u8g2.setCursor(75, 25);
    u8g2.print(serialString);

    u8g2.drawStr(5, 38, "Serial Val: ");
    u8g2.setCursor(75, 38);
    u8g2.print(serialCOM.getValue(serialString));


  } while ( u8g2.nextPage() );

  if (waitFlag == false) {
    waitFlag = true;
    Serial.println(" Waiting...");
  }

  delay(5); // it is important to wait for a bit
}

void serialEvent() {

  int idx = 0;

  while (Serial.available()) {

    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      serialTail = true;
      serialString[idx] = inChar;
    }

    if (!serialTail) {

      serialString[idx] = inChar; // here I build the instruction
      idx++;
    }

    if (serialTail) {

      Serial.flush();
      stringFinished = true;
      serialTail = false;

    }
  }
}

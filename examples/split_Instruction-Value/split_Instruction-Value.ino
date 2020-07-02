
/*
  Test code for working with string without using the class String which apparently
  poorly handles memory in the Arduino memory space

*/

#include <serialCommand.h>

#define INSTRUCTION_SIZE 4

// For incoming data from serialCOM GUI
bool stringFinished = false;    // Flag to indicate reception of a string after terminator is reached
bool serialTail = false;        // Flag to detect the "governator"
char serialString[] = "    ";        // Empty serial string variable
char *instruction;
char *INSTR_SET[] = {"HOLA", "GAIN", "VOLT", "SPEE", "INIT"}; // Order is VERY important!

int j = 1; // to keep track of iterations
int instrID;

serialCommand serialCOM(INSTRUCTION_SIZE);    // Instantiation of new object

void setup() {

  Serial.begin(115200);

}

void loop() {

  // if(Serial.available() > 0); serialEvent(); // only needed for Arduino Leonardo

  if (stringFinished) {

    stringFinished = false;

    instruction = serialCOM.getCommand(serialString);

    instrID = getID(instruction);

    Serial.print("Instruction: ");
    Serial.println(instruction);

    Serial.print("ID: ");
    Serial.println(instrID);

    switch (instrID) {

      case 0:

        Serial.println("Hi there! (again)");
        break;
      case 1:

        Serial.println("GAIN CHANGE");
        break;
      case 2:

        Serial.println("VOLTS CHANGE");
        break;
      case 3:

        Serial.println("SPEED CHANGE");
        break;

      default:

        break;

    }

    //free(cmd);    // VERY IMPORTANT!!!

    if (serialCOM.equal(serialString, "GAIN")) {

      Serial.println("Gain will be changed!");

      int gain = serialCOM.getValue(serialString);

      Serial.print("gain = ");
      Serial.println(gain);

    } else if (serialCOM.equal(serialString, "VOLT")) {

      Serial.println("Volts to be adjusted!");

      int volts = serialCOM.getValue(serialString);

      Serial.print("volts = ");
      Serial.println(volts);

    }  else if (serialCOM.equal(serialString, "SPEE")) {

      Serial.println("Speed is adjusted!");

      int speed = serialCOM.getValue(serialString);

      Serial.print("speed = ");
      Serial.println(speed);

    } else {

      Serial.println("No action will be taken...");

    }

  }

  Serial.print(j);
  Serial.println(" Waiting...");

  delay(1200);

  j++;

  if (j > 10) j = 1;

}

int getID(char* inStr) {

  bool done  = false;
  int pointer = -1;

  while (!done) {
    pointer++;
    if (strcmp(INSTR_SET[pointer], inStr) == 0) {
      done = true;
      break;
    }
  }

  return pointer;

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

      // here I build the instruction
      //inputString += inChar;
      serialString[idx] = inChar;
      idx++;
      //Serial.println(serialString[idx]);
    }

    if (serialTail) {
      //Serial.println(serialString);
      stringFinished = true;
      Serial.flush();
      serialTail = false;

      Serial.println("Chain complete!");
      //Serial.println(serialString);

    }
  }
}



/*
    Library for sending serial commands via the serial port.

    The library receives a command in the form: COMM_XXXX, where:

    COMM -> is the instruction coded in 4 characters,
      _  -> command separator,
    XXXX -> value sent to the serial. It can be omitted if the instruction does not
            involve handling a value them.

    This library helps when implementing a Finite States Machine and the MCU is controlled through
    the serial port via a PC. 

    Why this library is relevant? I coded this as I needed to implement a FSM on an Arduino board for 
    controlling a PID temperature controller.

    Author: dzalf - Daniel Melendrez
    Date: June 2020 (COVID-19 Vibes)

    Version: 0.1.1  - Initial deployment
             0.1.9  - General cleanup. Corrected some typos and reviewed methods implementation
             ...    - After a long hiatus (mainly due to COVID and life stuff), touching this code, again
    (Sept-21) 1.0.1 - Changes in data types
                      Custom char separator
                      Custom instruction size

    #TODO: Make library more memory efficient
           Make the library safer to use (memory-wise, instruction size, etc) 
           Add a whole lot of comments!                  

*/


#include "serialCommand.h"

// Constructors

serialCommand::serialCommand(bool debug) {

  _debug = debug;

}

serialCommand::serialCommand(int sizeInstr, bool debug) {

   _instrLength = sizeInstr;
  _debug = debug;

}

serialCommand::serialCommand(void) {


}

serialCommand::serialCommand(int sizeInstr) {

  _instrLength = sizeInstr;

}


// Methods

void serialCommand::begin(char *x) {

  _divChar = x;

  if (_debug) Serial.println(_divChar);

}

int serialCommand::getValue(char _inStr[]) {

  int _val = -1;

  _pch = strtok (_inStr, "_");

  if (_pch != NULL) {

    if (_debug) {
      Serial.print("pch = ");
      Serial.println(_pch);
    }

    _pch = strtok (NULL, "_");
    _val = atoi(_pch);

    if (_debug) {
      Serial.print("value + 1: ");
      Serial.println(_val + 1);
    }

    return _val;

  } else {

    if (_debug) Serial.println("Pointer is NULL");
    return -1;

  }
}

bool serialCommand::equal(char _inStr[], char instruction[]) {

  bool _cmp = false;
  char _buffer[_instrLength];

  _pch = strstr(_inStr, instruction);

  if (_pch != NULL) {

    if (_debug) {
      Serial.print("pch: ");
      Serial.print(_pch);
    }

    strncpy(_buffer, _pch, _instrLength);

    if (_debug) {

      Serial.print("buffer: ");

      for (int k = 0; k < _instrLength; k++) {
        Serial.print(_buffer[k]);
      }

      Serial.print("\n");
    }

    _cmp = strncmp(_buffer, instruction, _instrLength);

    if (_debug) {
      Serial.print("Comparison: ");

      Serial.println(_cmp);

    }

    if (_cmp == 0) {

      return 1;
    }

  } else {

    return 0;

    if (_debug) Serial.println("Pointer is NULL");
  }
}

char * serialCommand::getCommand(char _inStr[]) {
    
    strncpy(_instrBuffer, _inStr, _instrLength);
    
    _instrBuffer[_instrLength] = '\0';              // Since strncpy does NOT include null character
    
    return _instrBuffer;

}

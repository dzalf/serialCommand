
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
*/

#ifndef serialCommand_h
#define serialCommand_h

#include <Arduino.h>
#include <string.h>

class serialCommand{
  
  public:

  byte instrLength;
  
  serialCommand(bool debug = false);
  serialCommand(void);
  serialCommand(int);

  void begin(void); //char []
  int getValue(char []);
  bool equal(char[], char[]);
  char * getCommand(char[]);

  private:

  byte _instrLength = instrLength;
  char *_pch;
  bool _debug = false;
  char * _instrBuffer = (char *) malloc(_instrLength); // char pointer --> allocate memory for the size of the instruction
  
  };

  #endif

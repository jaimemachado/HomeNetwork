/*
 Name:		HomeNetwork.ino
 Created:	12/10/2016 7:33:15 PM
 Author:	jaime
*/


#include <arduino.h>
#include "UARTProtocol.h"

UARTProtocol protocol(UARTProtocolBaudRate::BAUD9600);




// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(9, OUTPUT);


	digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
	digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
	digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
	protocol.init();

}

// the loop function runs over and over again until power down or reset
void loop() {
	digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(50);              // wait for a second
	digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
	delay(50);              // wait for a second
	protocol.run();

}


/*
 Name:		HomeNetwork.ino
 Created:	12/10/2016 7:33:15 PM
 Author:	jaime
*/

#include <avr/eeprom.h>
#include <arduino.h>
#include "UARTProtocol.h"


#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))
#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }
#define MIN(x,y) ( x > y ? y : x )


UARTProtocol protocol("", UARTProtocolBaudRate::BAUD9600);


#define initialAddressDefault 1
// Change this any time the EEPROM content changes
const long magic_number = 0x5432;

struct __eeprom_data {
	long magicValue;
	uint8_t address;
};

void initialise_eeprom() {
	eeprom_write(initialAddressDefault, address);
	eeprom_write(magic_number, magicValue);
}


// the setup function runs once when you press reset or power the board
void setup() {
	long magic;
	eeprom_read(magic, magicValue);
	if (magic != magic_number)
	{
		initialise_eeprom();
	}
		

	uint8_t localAdress;
	eeprom_read(localAdress, address);

	pinMode(9, OUTPUT);


	digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
	digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
	digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
	protocol.init(localAdress);

}



// the loop function runs over and over again until power down or reset
void loop() {
	digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(50);              // wait for a second
	digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
	delay(50);              // wait for a second
	protocol.run();

}


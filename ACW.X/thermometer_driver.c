#include <xc.h>
#include "Utils.h"

void write_byte(unsigned char val) {
	unsigned char i;
	unsigned char temp;

	// Iterate through bits from l to r.
	for (i = 8; i > 0; i--) {
		temp = val & 0x01; // Shift lowest bit.
		set_dq_low();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		//        NOP();
		if (temp == 1) {
			set_dq_high();
		}
		delay2(t63us);
		set_dq_high();
		NOP();
		NOP();
		val = val >> 1; // right shift bit.
	}
}

unsigned char read_byte() {
	unsigned char i;
	unsigned char value = 0; //read temperature         
	static char j;

	for (i = 8; i > 0; i--) {
		value >>= 1;
		set_dq_low();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP(); //6us              
		set_dq_high(); // pull high       
		NOP();
		NOP();
		NOP();
		NOP();
		NOP(); //4us               
		j = dq;
		if (j) value |= 0x80; // ????
		delay2(t63us); //63us              
	}
	return (value);
}

int get_temp() {
	//The sequence has to be
	// followed by each transaction: 1 Initialisation -> 2 ROM Function Command -> 3Memory
	// Function Command -> 4 Transaction/Data

	unsigned char TLV = 0; //temperature high byte                     
	unsigned char THV = 0; //temperature low byte   
	set_dq_high();
	reset(); //reset,wait for  18b20 response.                                                                                                              
	write_byte(0XCC); //ignore ROM matching                                                                                                                            
	write_byte(0X44); //send  temperature convert command                                                                                                              
	//    for (int i = 0; i < 2; ++i) {
	//        delay2(t503us);
	//    }

	delay(20000); //Might want to lower

	reset(); //reset again,wait for 18b20 response                                                                                                        
	write_byte(0XCC); //ignore ROM matching                                                                                                                            
	write_byte(0XBE); //send read temperature command                                                                                                                  
	TLV = read_byte(); //read temperature low byte                                                                                                                      
	THV = read_byte(); //read temperature high byte                                                                                                                   
	set_dq_high(); //release general line                                                                                                                           
	return (TLV >> 4) | ((THV << 4) & 0X3f); //temperature value
}

void resetThermometer(void) {
	char presence = 1;
	while (presence) {
		set_dq_low(); // MCU pull low
		delay2(t503us);
		set_dq_high(); // release general line and wait for pull high
		delay2(t70us);
		if (dq == 1)
			presence = 1; // Didn't receive response.
		else
			presence = 0; // Received response.

		delay2(t430us);
	}
}

void display_temp(int TZ) {
	unsigned int wd = 0; //temperature BCD code  after convert
	unsigned char TX = TLV << 4; //temperature decimal
	intEntries = TZ % 10; //integer Entries bit                                                                                                                            
	tens = TZ / 10; //integer ten bit	
	
	if (TX & 0x80) {
		wd = wd + 5000;
	}
	if (TX & 0x40) {
		wd = wd + 2500;
	}
	if (TX & 0x20) {
		wd = wd + 1250;
	}
	if (TX & 0x10) {
		wd = wd + 625; //hereinbefore four instructions are turn  decimal into BCD code        
	}

	pt10 = wd / 1000; //ten cent bit                                                                           
	pt100 = (wd % 1000) / 100; //hundred cent bit                                                                       
	pt1000 = (wd % 100) / 10; //thousand cent bit                                                                      
	pt10000 = wd % 10; //myriad cent bit






	// SHOW ON BIG LCD
	//    unsigned char tens, intEntries, pt10, pt100, pt1000, pt10000;
	writecmd(0x01); // Clear lcd
	writeInt(tens); //display integer ten bit                        
	writeInt(intEntries); //display integer ten bit                        
	writechar('.'); //display integer ten bit                        
	writeInt(pt10); //display integer ten bit                        
	writeInt(pt100); //display integer ten bit                        
	writeInt(pt1000); //display integer ten bit                        
	writeInt(pt10000); //display integer ten bit      
	delay(10000);

	//    // SHOW ON 7SEG LCD
	//    PORTD = table[tens]; //display integer ten bit                        
	//    PORTA = 0x3e;
	//    delay2(tUKus);
	//
	//    PORTD = table[intEntries]&0X7F; //display integer Entries bit and decimal dot    
	//    PORTA = 0x3d;
	//    delay2(tUKus);
	//
	//    PORTD = table[pt10]; //display decimal ten cent bit                   
	//    PORTA = 0x3b;
	//    delay2(tUKus);
	//
	//    PORTD = table[pt100]; //display decimal hundred cent bit               
	//    PORTA = 0x37;
	//    delay2(tUKus);
	//
	//    PORTD = table[pt1000]; //display decimal thousand  cent bit             
	//    PORTA = 0x2f;
	//    delay2(tUKus);
	//
	//    PORTD = table[pt10000]; //display decimal myriad cent bit                
	//    PORTA = 0x1f;
	//    delay2(tUKus);
}
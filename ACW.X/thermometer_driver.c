#include <xc.h>
#include "Utils.h"
#include "thermometer_driver.h"

#define dq RE0
#define dq_dir TRISE0
#define set_dq_high() dq_dir = 1
#define set_dq_low() dq = 0; dq_dir = 0

void resetThermometer(void); // Privatised.
void write_byte(unsigned char val); // Privatised.
unsigned char read_byte(); // Privatised.

unsigned char TLV = 0; //temperature high byte                     
unsigned char THV = 0; //temperature low byte   
char temperature[8]; //Stores the temperature
// Timings for delay2 args. Cannot be const.
int t503us[2] = { 2, 70 };
int t430us[2] = { 2, 60 };
int t70us[2] = { 2, 8 };
int t63us[2] = { 2, 7 };
int tUKus[2] = { 1, 70 };

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
		DelayT(t63us);
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
		DelayT(t63us); //63us              
	}
	return (value);
}

int get_temp() {
	//The sequence has to be
	// followed by each transaction: 1 Initialisation -> 2 ROM Function Command -> 3Memory
	// Function Command -> 4 Transaction/Data

	set_dq_high();
	resetThermometer(); //reset,wait for  18b20 response.                                                                                                              
	write_byte(0XCC); //ignore ROM matching                                                                                                                            
	write_byte(0X44); //send  temperature convert command                                                                                                              
	//    for (int i = 0; i < 2; ++i) {
	//        delay2(t503us);
	//    }

	Delay(20000); //Might want to lower

	resetThermometer(); //reset again,wait for 18b20 response                                                                                                        
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
		DelayT(t503us);
		set_dq_high(); // release general line and wait for pull high
		DelayT(t70us);
		if (dq == 1)
			presence = 1; // Didn't receive response.
		else
			presence = 0; // Received response.

		DelayT(t430us);
	}
}

char* calculate_temp(int TZ) {
	unsigned int wd = 0; //temperature BCD code  after convert
	unsigned char TX = TLV << 4; //temperature decimal
	

	temperature[0] = TZ / 10; //integer ten bit	
	temperature[1] = TZ % 10; //integer Entries bit                                                                                                                            
	temperature[2] = '.';
	
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

	temperature[3] = wd / 1000; //ten cent bit                                                                           
	temperature[4] = (wd % 1000) / 100; //hundred cent bit                                                                       
	temperature[5] = (wd % 100) / 10; //thousand cent bit                                                                      
	temperature[6] = wd % 10; //myriad cent bit
	temperature[7] = '\0';
	return temperature;





	//// SHOW ON BIG LCD
	////    unsigned char tens, intEntries, pt10, pt100, pt1000, pt10000;
	//writecmd(0x01); // Clear lcd
	//writeInt(tens); //display integer ten bit                        
	//writeInt(intEntries); //display integer ten bit                        
	//writechar('.'); //display integer ten bit                        
	//writeInt(pt10); //display integer ten bit                        
	//writeInt(pt100); //display integer ten bit                        
	//writeInt(pt1000); //display integer ten bit                        
	//writeInt(pt10000); //display integer ten bit      
	//delay(10000);

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
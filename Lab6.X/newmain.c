#include <xc.h>
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config
#define rs RA5
#define rw RA4
#define e RA3
#define psb RA2
#define dq RC0
#define dq_dir TRISC0
#define set_dq_high() dq_dir = 1
#define set_dq_low() dq = 0; dq_dir = 0

// Timings for delay2 args. Cannot be const.
int t503us[2] = {2, 70};
int t430us[2] = {2, 60};
int t70us[2] = {2, 8};
int t63us[2] = {2, 7};
int tUKus[2] = {1, 70};

//
unsigned char TLV = 0; //temperature high byte                     
unsigned char THV = 0; //temperature low byte                      
unsigned char TZ = 0; //temperature integer after convert         
unsigned char TX = 0; //temperature decimal  after convert        
unsigned int wd; //temperature BCD code  after convert       

// Temperature component vars?
unsigned char tens, intEntries, pt10, pt100, pt1000, pt10000;
unsigned char table[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; // 7seg lcd nums 0-9
//the display code of 0-9     

//
void init(); //I/O init
void lcd_init(); //LCD init
void writecmd(char x); //display one byte
void writechar(char x); //LCD display setting.
void delay(int t); //delay
void delay2(int arr[]);
int strlen(char a[]);
void writeString(char str[]);
void setCursorPos(int lineN, int pos);
unsigned char read_byte();
void write_byte(unsigned char val);
void display_temp();
void reset(void);
int get_temp();


//I/O init

void init() {
    ADCON1 = 0X07; //a port as ordinary i/o.
    TRISA = 0X00; //a port as output.
    TRISD = 0X00; //d port as output.
    TRISC = 0x00;
    psb = 1;
}

//---------------------------------------
//LCD init

void lcd_init() {
    writecmd(0x0F); //display on,cursor on,blink on.
    writecmd(0x38); // 8 bits 2 lines 5*7 mode. / Set function
    writecmd(0x01); //clr screen
//    writecmd(0x02); //

}
//--------------------------------------
//write a byte to lcd.

void writechar(char x) {
    rs = 1; //is data not command.
    rw = 0; //is write not read.
    PORTD = x; //data send to PORTD
    e = 0; //pull low enable signal.
    delay(60); //for a while.
    e = 1; //pull high to build the rising edge.
}

void writeInt(int x) {
    writechar(x + 48);
}
//--------------------------------------
//lcd display setting

void writecmd(char x) {
    rs = 0; //is command not data
    rw = 0; //is write not read.
    PORTD = x;
    e = 0; //pull low enable signal.
    delay(60); //for a while.
    e = 1; //pull high to build the rising edge
}
//--------------------------------------
//delay

void delay(int t) {
    for (int i = 0; i < t; i++);
}
//---------------------------------------

int strlen(char a[]) {
    int len = 0;
    while (a[len])
        len++;
    return len;
}

void writeString(char str[]) {
    for (int i = 0; i < strlen(str); ++i) {
        writechar(str[i]);
    }
}

void setCursorPos(int lineN, int pos) {
    const int ln1 = 0xC0;
    const int ln2 = 0xD0;
    const int ln3 = 0xC8;
    const int ln4 = 0xD8;

    int addr;

    if (lineN == 1) {
        // Write to line 1.
        addr = ln1 + pos;
    } else if (lineN == 2) {
        // Write to line 2.
        addr = ln2 + pos;
    } else if (lineN == 3) {
        // Write to line 2.
        addr = ln3 + pos;
    } else if (lineN == 4) {
        // Write to line 2.
        addr = ln4 + pos;
    }

    writecmd(addr);
}
//---------------------------------------

void delay2(int t[]) {
    char x = t[0];
    char y = t[1];

    char z;
    do {
        z = y;
        do {
            ;
        } while (--z);
    } while (--x);
}

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

void display_temp() {
    TX = TLV << 4; //temperature decimal
    intEntries = TZ % 10; //integer Entries bit                                                                                                                            
    tens = TZ / 10; //integer ten bit                                                                                                                                
    wd = 0;
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

void reset(void) {
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

int get_temp() {
    //The sequence has to be
    // followed by each transaction: 1 Initialisation -> 2 ROM Function Command -> 3Memory
    // Function Command -> 4 Transaction/Data

    int i;
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
    TZ = (TLV >> 4) | ((THV << 4)&0X3f); //temperature integer       
    
    return TZ;
}

//main

void main() {
    init(); //call system initialize function                                                                                                                                 
    lcd_init();
    while (1) {       
        get_temp(); //call temperature convert function
        display_temp(); //call display function
        
        delay(10000);
    }
}
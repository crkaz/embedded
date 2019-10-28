#INCLUDE<P16F877A.INC>
    __CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC
; Predefined values
outer_delay equ 0ffh
inner_delay equ 0afh
    org 00h
 
main
    clrf PORTA
    clrf PORTD

    banksel TRISA
    clrf TRISA

    banksel TRISD
    clrf TRISD

    clrf STATUS

    MOVLW 0xAA ;The 7 seg to use 101000 00
    MOVWF PORTA
loop
    ;INCF PORTA
    call delay
    MOVLW 0xF9 ;Value to show
    MOVWF PORTD ;MOVWF PORTD ; Does 06h stay in W? YES

    call delay
    MOVLW 0xA4 ;Value to show
    MOVWF PORTD ;MOVWF PORTD ; Does 06h stay in W? YES 

    call delay
    MOVLW 0xB0 ;Value to show
    MOVWF PORTD ;MOVWF PORTD ; Does 06h stay in W? YES 

    call delay
    MOVLW 0x99 ;Value to show
    MOVWF PORTD ;MOVWF PORTD ; Does 06h stay in W? YES 

    call delay
    MOVLW 0x92 ;Value to show
    MOVWF PORTD ;MOVWF PORTD ; Does 06h stay in W? YES 

    call delay
    MOVLW 0x84 ;Value to show
    MOVWF PORTD ;MOVWF PORTD ; Does 06h stay in W? YES 

    goto loop

; Delay loops
delay
    movlw outer_delay
    movwf 30h
    outer_loop
    movlw inner_delay
    movwf 31h
    inner_loop
    decfsz 31h,1
    goto inner_loop
    decfsz 30h,1
    goto outer_loop
    return    

    end
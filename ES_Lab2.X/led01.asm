#INCLUDE<P16F877A.INC>
__CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC
org 00h
 
main
clrf PORTA
clrf PORTD
 
banksel TRISA
clrf TRISA
 
banksel TRISD
clrf TRISD
 
clrf STATUS

MOVLW 06H
MOVWF PORTA ; Does 06h stay in W?
 
loop
goto loop
end 
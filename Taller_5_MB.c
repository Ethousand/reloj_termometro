#include <16f887.h>
#use DELAY(clock = 4000000)
#byte portb = 0x06
#byte porta = 0x05
#fuses xt, NOPROTECT, NOWDT, NOBROWNOUT, PUT, NOLVP

#include <lcd.c>

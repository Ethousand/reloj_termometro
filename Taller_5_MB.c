#include <16f887.h>
#use DELAY(clock = 4000000)
#byte portb = 0x06
#byte porta = 0x05
#fuses xt, NOPROTECT, NOWDT, NOBROWNOUT, PUT, NOLVP

#include <lcd.c>

// variables de reloj
int hora, min, seg;

//variable de panel lcd

int fila_1[2] = {1, 1};
int fila_2[2] = {1, 2};

void main()
{
    lcd_init();

    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(fila_1);
        printf(lcd_putc, "hola khe hace");
    }
}

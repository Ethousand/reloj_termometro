#include <16f887.h>
#use DELAY(clock = 4000000)
#byte portb = 0x06
#byte porta = 0x05
#fuses xt, NOPROTECT, NOWDT, NOBROWNOUT, PUT, NOLVP

#include <lcd.c>

// variables de reloj
int hora = 1, min, seg, xm = 0;
int hora_i, min_i, seg_i;

//variables de calendario
int day, month, year;
int day_i, month_i, year_i;

// variables del termometro

//ejecucin del programa

void config_hour()
{
    lcd_putc('\f');
    printf(lcd_putc, "config");
    delay_ms(500);
    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "hora:");
        lcd_gotoxy(1, 2);

        if (xm == 0)
        {
            printf(lcd_putc, "%2d:%02d:%02d am", hora_i, min_i, seg_i);
        }
        else
        {
            printf(lcd_putc, "%2d:%02d:%02d pm", hora_i, min_i, seg_i);
        }

        Delay_ms(200);
        switch (porta)
        {
        case 0x01:
            hora_i++;
            if (hora_i > 12)
            {
                hora_i = 1;
            }

            break;
        case 0x02:
            min_i++;
            if (min_i >= 60)
            {
                min_i = 0;
            }

            break;
        case 0x04:
            seg_i++;
            if (seg_i >= 60)
            {
                seg_i = 0;
            }

            break;
        case 0x08:
            xm = 1 - xm;
            break;

        default:
            Delay_ms(200);
            break;
        }
    }

    lcd_putc("\f");
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "config");
}

void main()
{
    lcd_init();

    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "inicia reloj");
        delay_ms(500);
        config_hour();
        delay_ms(500);
    }
}

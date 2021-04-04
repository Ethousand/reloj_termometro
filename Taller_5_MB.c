#include <16f887.h>
#use DELAY(clock = 4000000)
#byte portb = 0x06
#byte porta = 0x05
#fuses xt, NOPROTECT, NOWDT, NOBROWNOUT, PUT, NOLVP

#include <lcd.c>

//variables generales
int delay = 200, stop = 1000;
int column[2] = {1, 2};

// variables de reloj
int hora = 1, min, seg, xm = 0;
int hora_i, min_i, seg_i;

//variables de calendario
int day, month, year;
int day_i, month_i;
int calentar[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// variables del termometro

//ejecucin del programa

void config_hour()
{
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "config");
    delay_ms(stop);
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
        Delay_ms(delay);

        switch (porta)
        {
        case 0x01:
            lcd_putc("\f");
            return;
            break;

        case 0x02:
            hora_i++;
            if (hora_i > 12)
            {
                hora_i = 1;
            }

            break;
        case 0x04:
            min_i++;
            if (min_i >= 60)
            {
                min_i = 0;
            }

            break;
        case 0x08:
            seg_i++;
            if (seg_i >= 60)
            {
                seg_i = 0;
            }
            break;

        case 0x10:
            xm = 1 - xm;
            break;

        default:
            Delay_ms(delay);
            break;
        }
    }
}

void config_date()
{
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "config 2");
    delay_ms(stop);
    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "fecha:");

        lcd_gotoxy(1, 2);
        printf(lcd_putc, "%2d/%02d/2,%03d", day_i, month_i, year);
        Delay_ms(delay);
        switch (porta)
        {

        case 0x01:
            lcd_putc("\f");
            return;
            break;

        case 0x02:
            //asignar un array para determinar la fecha final del mes
            day_i++;
            if (day_i > 12)
            {
                day_i = 1;
            }
            break;

        case 0x04:
            month_i++;
            if (month_i > 12)
            {
                month_i = 1;
            }
            break;

        case 0x08:
            year++;
            break;

        case 0x10:
            year = 0;
            break;

        default:
            Delay_ms(delay);
            break;
        }
    }
}

void main()
{
    lcd_init();
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "inicia reloj");
    delay_ms(delay);
    config_hour();
    delay_ms(delay);
    config_date();

    lcd_gotoxy(1, 1);
    printf(lcd_putc, "%02d:%02d:%02d xm", hora_i, min_i, seg_i);

    lcd_gotoxy(1, 2);
    printf(lcd_putc, "%02d/%02d/2,%03d", day_i, month_i, year);
    delay_ms(stop * 2);

    while (true)
    {
        lcd_putc("\f");
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "hola khe hace");
        delay_ms(stop * 2);

        lcd_gotoxy(1, 2);
        printf(lcd_putc, "linea 2");
        delay_ms(stop);
    }
}

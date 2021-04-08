#include <16f887.h>
#use DELAY(clock = 4000000)
#byte portb = 0x06
#byte porta = 0x05
#fuses xt, NOPROTECT, NOWDT, NOBROWNOUT, PUT, NOLVP

#include <lcd.c>

//variables generales
int delay = 200, stop = 10;
int column[2] = {1, 2};
int layout = 0;

// variables de reloj
int hora, min, seg, xm;
int hora_i, hora_q, min_i, seg_i, xm_i;

//variables de alarma
int a_hora, a_xm;

//variables de calendario
int day, month, year;
int day_i = 1, month_i = 0;
int calendar[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// variables del termometro

//ejecucin del programa

void config_date()
{
    while (true)
    {
        if (year % 4 == 0)
        {
            calendar[1] = 29;
        }
        else
        {
            calendar[1] = 28;
        }

        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "config date:");

        lcd_gotoxy(1, 2);
        printf(lcd_putc, "%2d/%02d/2,%03d", day_i, month_i + 1, year);
        delay_ms(delay);
        switch (portb)
        {

        case 0x01:
            lcd_putc("\f");
            return;
            break;

        case 0x02:
            //asignar un array para determinar la fecha final del mes
            day_i++;
            if (day_i > calendar[month_i])
            {
                day_i = 1;
            }
            break;

        case 0x04:
            month_i++;
            if (month_i > 11)
            {
                month_i = 0;
            }
            break;

        case 0x08:
            year++;
            break;

        case 0x10:
            year = 0;
            break;

        default:
            delay_ms(delay);
            break;
        }
    }
}

void config_hour()
{
    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "config hora:");
        lcd_gotoxy(1, 2);

        if (hora_i == 0)
        {
            hora_q = 12;
        }
        else
        {
            hora_q = hora_i;
        }

        if (xm_i == 0)
        {
            printf(lcd_putc, "%2d:%02d:%02dAm", hora_q, min_i, seg_i);
        }
        else
        {
            printf(lcd_putc, "%2d:%02d:%02dPm", hora_q, min_i, seg_i);
        }
        delay_ms(delay);

        switch (portb)
        {
        case 0x01:
            lcd_putc("\f");
            return;
            break;

        case 0x02:
            hora_i++;
            if (hora_i > 11)
            {
                hora_i = 0;
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
            xm_i = 1 - xm_i;
            break;

        default:
            delay_ms(delay);
            break;
        }
    }
}

void set_alarm()
{
    while (true)
    {
        if (a_hora == 0)
        {
            hora_q = 12;
        }
        else
        {
            hora_q = a_hora;
        }

        lcd_putc("\f");
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "config alarm:");

        lcd_gotoxy(1, 2);
        if (a_xm == 0)
        {
            printf(lcd_putc, "%02d Am", hora_q);
        }
        else
        {
            printf(lcd_putc, "%02d Pm", hora_q);
        }

        switch (portb)
        {
        case 0x01:
            return;
            break;

        case 0x02:
            a_hora++;
            if (a_hora > 11)
            {
                a_hora = 0;
            }
            break;

        case 0x04:
            a_xm = 1 - a_xm;
            break;

        default:
            break;
        }
        delay_ms(delay);
    }
}

void show_display()
{

    while (true)
    {
        lcd_putc("\f");
        if (year % 4 == 0)
        {
            calendar[1] = 29;
        }
        else
        {
            calendar[1] = 28;
        }

        for (month = month_i; month < 12; month++)
        {
            for (day = day_i; day <= calendar[month]; day++)
            {
                for (xm = xm_i; xm <= 1; xm++)
                {
                    for (hora = hora_i; hora <= 11; hora++)
                    {
                        if (hora == 0)
                        {
                            hora_q = 12;
                        }
                        else
                        {
                            hora_q = hora;
                        }

                        for (min = min_i; min < 60; min++)
                        {
                            for (seg = seg_i; seg < 60; seg++)
                            {
                                lcd_gotoxy(1, column[0]);
                                printf(lcd_putc, "%02d/%02d/2,%03d", day, month + 1, year);

                                lcd_gotoxy(1, column[1]);
                                if (xm == 0)
                                {
                                    printf(lcd_putc, "%02d:%02d:%02dAm", hora_q, min, seg);
                                }
                                else
                                {
                                    printf(lcd_putc, "%02d:%02d:%02dPm", hora_q, min, seg);
                                }
                                //delay_ms(stop);
                                if (portb == 0x01)
                                {
                                    return;
                                }
                                if (portb != 0x00)
                                {
                                    //funcion de cambio para LCD
                                }
                            }
                        }
                    }
                }
                xm_i = 0, hora_i = 0, min_i = 0, seg_i = 0;
            }
        }
        month_i = 0, day = 1, year++;
    }
}

void main()
{
    //configuración de puertos
    set_tris_a(0xff);
    set_tris_b(0xff);

    lcd_init();
    while (true)
    {
        config_date();
        delay_ms(delay);
        set_alarm();
        delay_ms(delay);
        config_hour();
        delay_ms(delay);

        show_display();
        delay_ms(stop);
    }
}

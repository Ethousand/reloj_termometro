#include <16f887.h>
#use DELAY(clock = 4000000)
#byte portb = 0x06
#byte porta = 0x05
#fuses xt, NOPROTECT, NOWDT, NOBROWNOUT, PUT, NOLVP

#include <lcd.c>

//variables generales
int delay = 200, stop = 100;
int column[2] = {1, 2};
int layout;

// variables de reloj
int hora, min, seg, xm;
int hora_i = 1, hora_q, min_i, seg_i, xm_i;

//variables de alarma
int a_hora = 1, a_xm;

//variables de calendario
int day, month, year;
int day_i = 1, month_i = 0;
int calendar[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// variables del termometro

//ejecucin del programa

void config_date()
{
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "config 1");
    delay_ms(stop);
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
        printf(lcd_putc, "fecha:");

        lcd_gotoxy(1, 2);
        printf(lcd_putc, "%2d/%02d/2,%03d", day_i, month_i + 1, year);
        delay_ms(delay);
        switch (portb)
        {

        case 0x01:
            lcd_putc("\f");
            delay_ms(delay);
            return;
            break;

        case 0x02:
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
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "config 2");
    delay_ms(stop);
    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "hora:");
        lcd_gotoxy(1, 2);

        if (xm_i == 0)
        {
            printf(lcd_putc, "%2d:%02d:%02d am", hora_i, min_i, seg_i);
        }
        else
        {
            printf(lcd_putc, "%2d:%02d:%02d pm", hora_i, min_i, seg_i);
        }
        delay_ms(delay);

        switch (portb)
        {
        case 0x01:
            lcd_putc("\f");
            delay_ms(delay);
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
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "config 3");
    delay_ms(stop);

    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "alarma:");
        lcd_gotoxy(1, 2);

        if (a_xm == 0)
        {
            printf(lcd_putc, "%2d am", a_hora);
        }
        else
        {
            printf(lcd_putc, "%2d pm", a_hora);
        }
        delay_ms(delay);

        switch (portb)
        {
        case 0x01:
            lcd_putc("\f");
            delay_ms(delay);
            return;
            break;

        case 0x02:
            a_hora++;
            if (a_hora > 12)
            {
                a_hora = 1;
            }
            delay_ms(delay);
            break;

        case 0x04:
            a_xm = 1 - a_xm;
            delay_ms(delay);
            break;

        default:
            delay_ms(delay);
            break;
        }
    }
}

void show_date_hour()
{
    delay_ms(delay);
    lcd_gotoxy(1, column[0]);
    printf(lcd_putc, "%02d:%02d:%02dxm", hora_i, min_i, seg_i);

    lcd_gotoxy(1, column[1]);
    printf(lcd_putc, "%02d/%02d/2,%03d", day_i, month_i, year);
    delay_ms(stop);

    while (true)
    {
        if (hora_i == 12)
        {
            hora_i = 0;
        }

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
                    for (hora = hora_i; hora < 12; hora++)
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
                                switch (a_xm)
                                {
                                case 0:
                                    if (xm == 0)
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dAm  %02dAm", hora_q, min, seg, a_hora);
                                    }
                                    else
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dPm  %02dAm", hora, min, seg, a_hora);
                                    }
                                    break;

                                case 1:
                                    if (xm == 0)
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dAm  %02dPm", hora_q, min, seg, a_hora);
                                    }
                                    else
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dPm  %02dPm", hora, min, seg, a_hora);
                                    }
                                    break;
                                }

                                //delay_ms(stop);
                            }
                        }
                    }
                }
                xm_i = 0, hora_i = 0, min_i = 0, seg_i = 0;
            }
        }
        month_i = 0, day_i = 1, year++;
    }
}

void main()
{
    //configurar puertos
    set_tris_a(0xff);
    set_tris_b(0x1f);
    portb = 0x00;

    //inicio del panel LCD
    lcd_init();
    lcd_putc('\f');

    while (true)
    {

        config_date();
        config_hour();
        set_alarm();
        delay_ms(delay);

        show_date_hour();
        delay_ms(delay);
    }
}

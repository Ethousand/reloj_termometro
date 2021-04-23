#include <16f887.h>
#device adc = 10
#use DELAY(clock = 4000000)
#byte porta = 0x05
#byte portb = 0x06
#byte portc = 0x07
#fuses xt, NOPROTECT, NOWDT, NOBROWNOUT, PUT, NOLVP

#include <lcd.c> // la libreria define al puerto D como salida por defecto así que no es necesario configurarlo

//variables generales
int delay = 200, stop = 1000;

int column[2] = {1, 2};
int layout;

// variables de reloj
int hora, min, seg, xm;                   // xm = am/pm
int hora_i, hora_LCD, min_i, seg_i, xm_i; // variables para el inicio de reloj y hora_LCD que se mostrará en pantalla

//variables de alarma
int a_hora = 1, a_xm;
int alarm_on;

//variables de alarma
int a_hora = 1, a_xm;

//variables de calendario
int day, month, year;
int day_i = 1, month_i = 0;
int calendar[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // array que determina cuantos dias tiene el mes en cuestion

// variables del termometro
int med, temp; // med: medición directa del puerto; temp: temperatura resultante de la medición

//ejecucin del programa

void config_date()
{
    while (true)
    {
        if (year % 4 == 0) // en caso de ser año viciesto
        {
            calendar[1] = 29;
        }
        else // de no serlo
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
    while (true)
    {
        lcd_putc('\f');
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "config hora:");
        lcd_gotoxy(1, 2);


        if (hora_i == 0) // corrección para mostrar la hora desde las 12 como reloj civil
        {
            hora_LCD = 12;
        }
        else
        {
            hora_LCD = hora_i;
        }
        // display al segun si estas es antes o despues del mediodia
      
        if (xm_i == 0)
        {
            printf(lcd_putc, "%2d:%02d:%02dAm", hora_LCD, min_i, seg_i);
        }
        else
        {
            printf(lcd_putc, "%2d:%02d:%02dPm", hora_LCD, min_i, seg_i);
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
        lcd_putc("\f");
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "config alarm:");

        lcd_gotoxy(1, 2);
        if (a_xm == 0)
        {
            printf(lcd_putc, "%02d Am", a_hora);
        }
        else
        {
            printf(lcd_putc, "%02d Pm", a_hora);
        }

        switch (portb)
        {
        case 0x01:
            return;
            break;

        // en este caso no hay corrección de horario por no tener que mostrarse de manera horaria, solo mostrar los digitos correspondientes
        case 0x02:
            a_hora++;
            if (a_hora > 12)
            {
                a_hora = 1;
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

//esta función no utiliza ciclo while para no alterar el ciclo de reloj en la función show_Display

void change(int status)
{
    delay_ms(delay);
    switch (status)
    {
    case 0x02:
        alarm_on = 0;
        bit_clear(portc, 1);
        break;

    case 0x04:
        layout = 1 - layout;
        if (layout == 1)
        {
            column[0] = 2;
            column[1] = 1;
        }
        else

        {
            column[0] = 1;
            column[1] = 2;
        }
        break;

    default:
        delay_ms(delay);
        break;
    }
    lcd_putc("\f");
}

void show_display()
{
    set_adc_channel(0);
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
                            hora_LCD = 12;
                        }
                        else
                        {
                            hora_LCD = hora;
                        }

                        for (min = min_i; min < 60; min++)
                        {
                            for (seg = seg_i; seg < 60; seg++)
                            {

                                med = read_adc();          //lectura del integrado
                                temp = (med * 500) / 1024; // conversion a la temperatura equivalente

                                //funcion de reloj
                                lcd_gotoxy(1, column[0]);
                                printf(lcd_putc, "%02d/%02d/2,%03d  %02dC", day, month + 1, year, temp);
                                //el LCD solo puee representar digitos de hasta 3 digitos, el cuarto bit es la bandera de signo

                                lcd_gotoxy(1, column[1]);
                                switch (a_xm)

                                {
                                case 1:
                                    if (xm == 0)
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dAm  %02dPm", hora_LCD, min, seg, a_hora);
                                    }
                                    else
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dPm  %02dPm", hora_LCD, min, seg, a_hora);
                                    }
                                    break;

                                default:
                                    if (xm == 0)
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dAm  %02dAm", hora_LCD, min, seg, a_hora);
                                    }
                                    else
                                    {
                                        printf(lcd_putc, "%02d:%02d:%02dPm  %02dAm", hora_LCD, min, seg, a_hora);
                                    }
                                    break;

                                }
                                delay_ms(stop);
                                if (portb == 0x01)
                                {
                                    return;
                                }
                                if (a_hora == hora_LCD && a_xm == xm && min == 0)
                                {
                                    bit_set(portc, 1);
                                }

                                if (portb != 0x00)
                                {
                                    change(portb);
                                    //funcion de cambio para LCD
                                }

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
                                delay_ms(stop);
                            }
                            // cada nuevo ciclo reinicia el numero inicial para que este pueda empezar desde su fecha inicial
                            seg_i = 0;
                        }
                        min_i = 0;
                    }
                    hora_i = 0;
                }
                xm_i = 0;
            }
            day = 1;
        }
        month_i = 0, year++;
    }
}

// en la función main se iniciaran los procesos para ser ejecutados por las demas funciones
void main()
{
    //configuración de termometro
    setup_adc(adc_clock_internal);
    setup_adc(all_analog); // define le puerto a y todos sus pines como entradas analogas
    //configuración de puertos

    set_tris_b(0xff); // entrada
    set_tris_c(0x00); // salida

    portc = 0x00;

    lcd_init();
    while (true)
    {
        config_date();
        delay_ms(delay * 2);
        set_alarm();
        delay_ms(delay * 2);
        config_hour();
        delay_ms(delay * 2);

        show_display();
        delay_ms(stop);
    }
}

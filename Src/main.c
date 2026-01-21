#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "systick.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "exti.h"
#include "rtc.h"

/**
 * @brief Convert integer to string (no sprintf needed)
 * 
 * @param num Number to convert
 * @param str Pointer to buffer
 * @return void
 */
static void intToString(int num, char *str)
{
    int i = 0;
    int isNegative = 0;
    
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }
    
    if (num == 0)
    {
        str[i++] = '0';
    }
    else
    {
        while (num > 0)
        {
            str[i++] = (num % 10) + '0';
            num = num / 10;
        }
    }
    
    if (isNegative)
        str[i++] = '-';
    
    str[i] = '\0';
    
    /* Reverse the string */
    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/**
 * @brief Display current RTC time and date via UART
 * 
 * Displays time in format: HH:MM:SS
 * Displays date in format: MM-DD-YY
 * 
 * @return void
 */
static void displayRtcCalendar(void)
{
    char temp[4];
    uint32_t value;
    
    /*Send time label*/
    uartSendString("Time: ");
    
    /*Send hours*/
    value = rtcTimeGetHour();
    if (value < 10)
        uartSendString("0");
    intToString(value, temp);
    uartSendString(temp);
    uartSendString(":");
    
    /*Send minutes*/
    value = rtcTimeGetMinute();
    if (value < 10)
        uartSendString("0");
    intToString(value, temp);
    uartSendString(temp);
    uartSendString(":");
    
    /*Send seconds*/
    value = rtcTimeGetSecond();
    if (value < 10)
        uartSendString("0");
    intToString(value, temp);
    uartSendString(temp);
    uartSendString("\r\n");

    /*Send date label*/
    uartSendString("Date: ");
    
    /*Send month*/
    value = rtcDateGetMonth();
    if (value < 10)
        uartSendString("0");
    intToString(value, temp);
    uartSendString(temp);
    uartSendString("-");
    
    /*Send day*/
    value = rtcDateGetDay();
    if (value < 10)
        uartSendString("0");
    intToString(value, temp);
    uartSendString(temp);
    uartSendString("-");
    
    /*Send year*/
    value = rtcDateGetYear();
    if (value < 10)
        uartSendString("0");
    intToString(value, temp);
    uartSendString(temp);
    uartSendString("\r\n");
}

int main(void)
{
    /*Initialize UART for debugging*/
    uartInit();

    /*Initialize RTC*/
    rtcInit();
    
    /*Send startup message*/
    uartSendString("=== STM32F411 RTC Demo ===\r\n");
    
    /*Main loop*/
    while (1)
    {
        /*Display RTC time and date*/
        displayRtcCalendar();
        
        /*Add delay to avoid flooding UART output*/
        systickMsecDelay(1000);
    }
}


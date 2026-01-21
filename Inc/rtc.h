#ifndef __RTC__H__
#define __RTC__H__

#define STM32F411xE
#include "stm32f4xx.h"

/**
 * @file rtc.h
 * @brief RTC (Real Time Clock) driver for STM32F411xE
 * 
 * Provides functions to initialize and manage RTC using LSI clock source
 * at 32 kHz frequency.
 * 
 * @author Bare Metal STM32
 * @date 2026
 */

/**
 * @brief Initialize RTC peripheral
 * 
 * Configures RTC with LSI as clock source (32 kHz) and enters initialization mode.
 * Must be called before any other RTC operations.
 * 
 * @return void
 * @note Disables write protection after initialization
 */
void rtcInit(void);

uint8_t rtcConvertDec2BCD(uint8_t value);

/**
 * @brief Convert BCD value to decimal
 * 
 * @param value BCD encoded value
 * @return Decimal value
 * 
 * @note BCD (Binary Coded Decimal) is used by RTC hardware
 */
uint8_t rtcConvertBCD2Dec(uint8_t value);

/**
 * @brief Get current day from RTC
 * 
 * @return Day value (1-31)
 */
uint32_t rtcDateGetDay(void);

/**
 * @brief Get current year from RTC
 * 
 * @return Year value (0-99)
 */
uint32_t rtcDateGetYear(void);

/**
 * @brief Get current month from RTC
 * 
 * @return Month value (1-12)
 */
uint32_t rtcDateGetMonth(void);

/**
 * @brief Get current second from RTC
 * 
 * @return Second value (0-59)
 */
uint32_t rtcTimeGetSecond(void);

/**
 * @brief Get current minute from RTC
 * 
 * @return Minute value (0-59)
 */
uint32_t rtcTimeGetMinute(void);

/**
 * @brief Get current hour from RTC
 * 
 * @return Hour value (0-23 for 24-hour format)
 */
uint32_t rtcTimeGetHour(void);

/**
 * @brief Enable RTC initialization mode
 * 
 * Allows writing to RTC registers. Must be enabled before configuring
 * time and date.
 * 
 * @return void
 */
void rtcEnableInitMode(void);

/**
 * @brief Disable RTC initialization mode
 * 
 * Exits initialization mode and resumes normal RTC operation.
 * 
 * @return void
 */
void rtcDisableInitMode(void);

/**
 * @brief Check if RTC initialization flag is active
 * 
 * @return 1 if init mode is active, 0 otherwise
 */
uint8_t rtcIsActiveFlagInit(void);

/**
 * @brief Check if RTC registers are synchronized
 * 
 * @return 1 if synchronized, 0 otherwise
 */
uint8_t rtcIsActiveFlagRS(void);

/**
 * @brief Internal initialization sequence
 * 
 * @return Status (1 = success, 0 = fail)
 */
static uint8_t rtcInitSeq(void);

/**
 * @brief Wait for RTC synchronization
 * 
 * @return Status (1 = success, 0 = timeout)
 */
static uint8_t waitForSynchro(void);

/**
 * @brief Exit initialization sequence
 * 
 * @return Status (1 = success, 0 = fail)
 */
static uint8_t exitInitSeq(void);

/**
 * @brief Configure RTC date registers
 * 
 * @param weekDay Day of week (1-7)
 * @param day Day of month (1-31)
 * @param month Month (1-12)
 * @param year Year (0-99)
 * 
 * @return void
 */
static void rtcDateConfig(uint32_t weekDay, uint32_t day, uint32_t month, uint32_t year);

/**
 * @brief Configure RTC time registers
 * 
 * @param format1224 Time format (0=24-hour, 1=12-hour with AM/PM)
 * @param hours Hours (0-23)
 * @param minutes Minutes (0-59)
 * @param seconds Seconds (0-59)
 * 
 * @return void
 */
static void rtcTimeConfig(uint32_t format1224, uint32_t hours, uint32_t minutes, uint32_t seconds);


static void rtcSetAsynchPrescaler(uint32_t asynchPrescaler);
static void rtcSetSynchPrescaler(uint32_t synchPrescaler);
#endif // __RTC__H__
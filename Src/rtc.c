#include "rtc.h"

#define PWREN		(1U<<28)
#define CR_DBP		(1U<<8)
#define CSR_LSION	(1U<<0)
#define CSR_LSIRDY	(1U<<1)
#define BDCR_BDRST	(1U<<16)
#define BDCR_RTCEN	(1U<<15)

#define RTC_WRITE_PROTECTION_KEY_1 ((uint8_t)0xCAU)
#define RTC_WRITE_PROTECTION_KEY_2 ((uint8_t)0x53U)
#define RTC_INIT_MASK                 0xFFFFFFFFU
#define ISR_INITF						(1U<<6)


#define WEEKDAY_FRIDAY             		((uint8_t)0x05U)
#define MONTH_DECEMBER             		((uint8_t)0x12U)
#define TIME_FORMAT_PM             		(1U<<22)
#define CR_FMT							(1U<<6)
#define ISR_RSF							(1U<<5)

#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00F9)

/**
 * @brief Initialize RTC peripheral with LSI clock source
 * 
 * Configuration steps:
 * 1. Enable PWR clock
 * 2. Enable backup domain access (DBP bit in PWR->CR)
 * 3. Enable and wait for LSI to be ready
 * 4. Reset and enable RTC
 * 5. Disable write protection
 * 6. Enter initialization mode
 * 
 * @note LSI frequency is approximately 32 kHz
 * @return void
 */
void rtcInit(void)
{
	/*Enable clock access to PWR */
	RCC->APB1ENR |= PWREN;

	/*Enable Backup access to config RTC*/
	PWR->CR |=CR_DBP;

	/*Enable Low Speed Internal (LSI)*/
	RCC->CSR |=CSR_LSION;

	/*Wait for LSI to be ready*/
	while((RCC->CSR & CSR_LSIRDY) != CSR_LSIRDY){}

	/*Force backup domain reset*/
	RCC->BDCR |=BDCR_BDRST;

	/*Release backup domain reset*/
	RCC->BDCR &= ~BDCR_BDRST;

	/*Set RTC clock source to LSI*/
	RCC->BDCR &=~(1U<<8);
	RCC->BDCR |= (1U<<9);

	/*Enable the RTC*/
	RCC->BDCR |=BDCR_RTCEN;

	/*Disable RTC registers write protection*/
	RTC->WPR = RTC_WRITE_PROTECTION_KEY_1;
	RTC->WPR = RTC_WRITE_PROTECTION_KEY_2;

	/*Enter the initialization mode*/
	if(rtcInitSeq() != 1)
	{
		//Do something...
	}

	/*Set desired date :  Friday December 29th 2016*/
	rtcDateConfig(WEEKDAY_FRIDAY,0x29,MONTH_DECEMBER,0x16);

	/*Set desired time :  11:59:55 PM*/
	rtcTimeConfig(TIME_FORMAT_PM,0x11,0x59,0X55);

	/*Set hour format*/
	RTC->CR |=CR_FMT;

	/*Set Asynch prescaler*/
	rtcSetAsynchPrescaler(RTC_ASYNCH_PREDIV);

	/*Set Sync prescaler*/
	rtcSetSynchPrescaler(RTC_SYNCH_PREDIV);

	/*Exit the initialization mode*/
	exitInitSeq();

	/*Enable RTC registers write protection*/
	RTC->WPR = 0xFF;
}

uint8_t rtcConvertDec2BCD(uint8_t value)
{
    return (uint8_t)((((value) / 10U) / 10U) << 4U) | ((value) % 10U);
}

/**
 * @brief Convert BCD (Binary Coded Decimal) to decimal
 * 
 * RTC hardware uses BCD format where each nibble represents a decimal digit.
 * Example: 0x45 BCD = 45 decimal
 * 
 * @param value BCD encoded value
 * @return Decimal value
 */
uint8_t rtcConvertBCD2Dec(uint8_t value)
{
    return (uint8_t)(((uint8_t)((value) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((value) & (uint8_t)0x0FU));
}

/**
 * @brief Get current day of month from RTC
 * 
 * @return Day value (1-31) in decimal format
 */
uint32_t rtcDateGetDay(void)
{
    uint8_t bcd = (RTC->DR >> RTC_DR_DU_Pos) & 0x3F;
    return rtcConvertBCD2Dec(bcd);
}

/**
 * @brief Get current year from RTC
 * 
 * @return Year value (0-99) in decimal format
 */
uint32_t rtcDateGetYear(void)
{
    uint8_t bcd = (RTC->DR >> RTC_DR_YU_Pos) & 0xFF;
    return rtcConvertBCD2Dec(bcd);
}
/**
 * @brief Get current month from RTC
 * 
 * @return Month value (1-12) in decimal format
 */
uint32_t rtcDateGetMonth(void)
{
    uint8_t bcd = (RTC->DR >> RTC_DR_MU_Pos) & 0x1F;
    return rtcConvertBCD2Dec(bcd);
}


/**
 * @brief Get current seconds from RTC
 * 
 * @return Seconds value (0-59) in decimal format
 */
uint32_t rtcTimeGetSecond(void)
{
    uint8_t bcd = (RTC->TR >> RTC_TR_SU_Pos) & 0x7F; // ST[6:4], SU[3:0]
    return rtcConvertBCD2Dec(bcd);
}

/**
 * @brief Get current minutes from RTC
 * 
 * @return Minutes value (0-59) in decimal format
 */
uint32_t rtcTimeGetMinute(void)
{
    return (uint32_t)(READ_BIT(RTC->TR, (RTC_TR_MNT | RTC_TR_MNU))>> RTC_TR_MNU_Pos);
}

/**
 * @brief Get current hours from RTC
 * 
 * @return Hours value (0-23) in decimal format for 24-hour mode
 */
uint32_t rtcTimeGetHour(void)
{
    return (uint32_t)((READ_BIT(RTC->TR, (RTC_TR_HT | RTC_TR_HU))) >> RTC_TR_HU_Pos);
}

/**
 * @brief Enable RTC initialization mode
 * 
 * Allows modification of RTC registers (INIT bit in RTC->ISR)
 * 
 * @return void
 */
void rtcEnableInitMode(void)
{
    RTC->ISR |= RTC_ISR_INIT;
}

/**
 * @brief Disable RTC initialization mode
 * 
 * Exits initialization mode and resumes normal RTC counting
 * 
 * @return void
 */
void rtcDisableInitMode(void)
{
    RTC->ISR &= ~RTC_ISR_INIT;
}

/**
 * @brief Check if RTC is in initialization mode
 * 
 * @return 1 if INIT flag is set, 0 otherwise
 */
uint8_t rtcIsActiveFlagInit(void)
{
    return ((RTC->ISR & RTC_ISR_INITF) == RTC_ISR_INITF);
}

/**
 * @brief Check if RTC registers are synchronized
 * 
 * After entering initialization mode, must wait for RSF (Registers Synchro Flag)
 * to be cleared and set again before registers are synchronized.
 * 
 * @return 1 if RSF is set, 0 otherwise
 */
uint8_t rtcIsActiveFlagRS(void)
{
    return ((RTC->ISR & RTC_ISR_RSF) == RTC_ISR_RSF);
}

/**
 * @brief Internal RTC initialization sequence
 * 
 * Enters initialization mode and waits for synchronization
 * 
 * @return 1 if successful, 0 if timeout
 */
static uint8_t rtcInitSeq(void)
{
    /*Start init mode*/
    rtcEnableInitMode();

    /*Wait till we are in init mode*/
    while(rtcIsActiveFlagInit() != 1) {}
}

/**
 * @brief Wait for RTC synchronization
 * 
 * Clears RSF flag and waits for it to be set again, ensuring
 * that shadow registers are synchronized with RTC registers.
 * 
 * @return 1 if synchronized, 0 if timeout
 * @note Timeout set to ~32000 iterations (approximates 1 second at 32kHz)
 */
static uint8_t waitForSynchro(void)
{
    /*Clear RSF*/
    RTC->ISR &= ~RTC_ISR_RSF;

    /*Wait for registers to synchronize*/
    while(rtcIsActiveFlagRS() != 1) {}
}

//todo
/**
 * @brief Exit RTC initialization sequence
 * 
 * Disables initialization mode and returns to normal operation
 * 
 * @return 1 if successful, 0 if failed
 */
static uint8_t exitInitSeq(void)
{
    /*Stop init mode*/
    rtcDisableInitMode();

    /*Wait for registers to synchronize*/
    return (waitForSynchro());
}

/**
 * @brief Configure RTC date (day of week, day, month, year)
 * 
 * Must be called while RTC is in initialization mode.
 * Parameters should be in decimal format (not BCD).
 * 
 * @param weekDay Day of week (1-7, where 1=Monday)
 * @param day Day of month (1-31)
 * @param month Month (1-12)
 * @param year Year (0-99)
 * 
 * @return void
 * @note Call rtcEnableInitMode() before this function
 * @note Values will be converted to BCD format by hardware
 */
static void rtcDateConfig(uint32_t weekDay, uint32_t day, uint32_t month, uint32_t year)
{
    uint32_t temp = 0;

    temp =
        ((weekDay & 0x7) << RTC_DR_WDU_Pos) |

        ((year  & 0xF0) << (RTC_DR_YT_Pos - 4)) |
        ((year  & 0x0F) << RTC_DR_YU_Pos) |

        ((month & 0x10) << (RTC_DR_MT_Pos - 4)) |   // MT: bit 12
        ((month & 0x0F) << RTC_DR_MU_Pos) |        // MU

        ((day   & 0x30) << (RTC_DR_DT_Pos - 4)) |
        ((day   & 0x0F) << RTC_DR_DU_Pos);

    RTC->DR = temp;
}


/**
 * @brief Configure RTC time (hours, minutes, seconds)
 * 
 * Must be called while RTC is in initialization mode.
 * 
 * @param format1224 Time format (0=24-hour, 1=12-hour with AM/PM)
 * @param hours Hours (0-23 for 24-hour format, 1-12 for 12-hour)
 * @param minutes Minutes (0-59)
 * @param seconds Seconds (0-59)
 * 
 * @return void
 * @note Call rtcEnableInitMode() before this function
 * @note In 12-hour mode, bit 22 indicates AM (0) or PM (1)
 */
static void rtcTimeConfig(uint32_t format1224, uint32_t hours, uint32_t minutes, uint32_t seconds)
{
    register uint32_t temp = 0U;

    temp = format1224 |
            (((hours & 0xF0U) << (RTC_TR_HT_Pos - 4U)) | ((hours & 0x0FU) << RTC_TR_HU_Pos)) |
            (((minutes & 0xF0U) << (RTC_TR_MNT_Pos - 4U)) | ((minutes & 0x0FU) << RTC_TR_MNU_Pos)) |
            (((seconds & 0xF0U) << (RTC_TR_ST_Pos - 4U)) | ((seconds & 0x0FU) << RTC_TR_SU_Pos));
    MODIFY_REG(RTC->TR, (RTC_TR_PM | RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU | RTC_TR_ST | RTC_TR_SU), temp);
}

static void rtcSetAsynchPrescaler(uint32_t asynchPrescaler)
{
  MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_A, asynchPrescaler << RTC_PRER_PREDIV_A_Pos);
}

static void rtcSetSynchPrescaler(uint32_t synchPrescaler)
{
  MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_S, synchPrescaler);
}

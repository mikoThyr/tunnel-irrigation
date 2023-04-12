#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc_ulp.h"

#include "macro.S"


// ------ SECTION BSS  ------
.section    .bss
    .global day_flag    /**< The variable indicates that the time of day will be checked */
day_flag:   .long

// ------ SECTION DATA ------
.section    .data


// ------ SECTION TEXT ------
.section    .text


    .global entry
entry:
    move    r3, day_flag
    ld      r0, r3, 0
    move    r0, 0x00
    st      r0, r3, 0
    wait    65500
    jump    wake_up
    /*
    jumpr   repeat, 0, eq
    WRITE_RTC_REG(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS_S + 0x08, 1, 1)

repeat:
    adc     r0, 1, 9
    st      r0, r3, 0
    jumpr   low, 3500, gt
    WRITE_RTC_REG(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS_S + 0x08, 1, 1)
    jump    repeat
low:
    WRITE_RTC_REG(RTC_GPIO_OUT_W1TC_REG, RTC_GPIO_OUT_DATA_W1TC_S + 0x08, 1, 1)
    jump    entry
    */
wake_up:
    wake
    WRITE_RTC_FIELD(RTC_CNTL_STATE0_REG, RTC_CNTL_ULP_CP_SLP_TIMER_EN, 0)
    halt

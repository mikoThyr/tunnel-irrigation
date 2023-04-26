#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc_ulp.h"

#include "macro.S"


// ------ SECTION BSS  ------
.section    .bss
    .global day_flag    /**< The variable indicates that the time of day will be checked */
day_flag:   .long

    .global run_settings    /**< Exit from ULP and run the wifi to start webserver configuration */
run_settings:.long


// ------ SECTION DATA ------
.section    .data


// ------ SECTION TEXT ------
.section    .text


    .global entry
entry:
    move    r0, run_settings
    move    r1, 0
    st      r1, r0, 0
main:
    wait    65535
    READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S + 17, 1)
    jumpr   settings, 0, gt
    jump    main

settings:
    move    r0, run_settings
    move    r1, 1
    st      r1, r0, 0
wake_up:
    wake
    WRITE_RTC_FIELD(RTC_CNTL_STATE0_REG, RTC_CNTL_ULP_CP_SLP_TIMER_EN, 0)
    halt

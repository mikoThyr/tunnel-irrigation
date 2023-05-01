#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc_ulp.h"

#include "macro.S"


// ------ SECTION BSS  ------
.section    .bss
    .global day_flag        /**< The variable indicates that the time of day will be checked */
day_flag:   .long
    .global day_time        /**< If day_flag is set a value of day_time inicate time of the day (either day or night)*/
day_time:   .long
    .global run_settings    /**< Exit from ULP and run the wifi to start webserver configuration */
run_settings:.long


// ------ SECTION DATA ------
.section    .data


// ------ SECTION TEXT ------
.section    .text


    .global entry
entry:
    move    r2, run_settings
    move    r1, 0
    st      r1, r2, 0

    // If the "DAY" option is set and actually is night.
    move    r2, day_time
    ld      r0, r2, 0
    jumpr   check_night, 1, eq

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


check_night:
    move    r0, 300                 // 60 sec * 5 = 300 sec (5 min)
    stage_rst
one_sec:
    wait    65535
    stage_inc 1
    jumps   one_sec, 122, le        // 1 sec
    sub     r0, r0, 1
    jumpr   check_night, 0, gt

    jump    wake_up

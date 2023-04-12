#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc_ulp.h"

#include "macro.S"

// ------ SECTION BSS ------
.section    .bss

  .global stack_start
stack_start:
  .skip   32
  .global   stack_end
stack_end:  .long 0


// ------ SECTION DATA ------
.section    .data


// ------ SECTION TEXT ------
.section    .text


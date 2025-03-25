;ChipId: PT3220

;Stack Configuration------------------------------------------------------------
Stack_Size      EQU     0x600
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp
;-------------------------------------------------------------------------------

;Heap Configuration-------------------------------------------------------------
Heap_Size       EQU     0x200
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit
;-------------------------------------------------------------------------------
                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset-------------------------------------
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp           ; 0,  load top of stack
                DCD     Reset_Handler          ; 1,  Reset Handler
                DCD     NMI_Handler            ; 2,  NMI Handler
                DCD     HardFault_Handler      ; 3,  Hard Fault Handler
                DCD     0                      ; 4,  Reserved
                DCD     0                      ; 5,  Reserved
                DCD     0                      ; 6,  Reserved
                DCD     0                      ; 7,  Reserved
                DCD     0                      ; 8,  Reserved
                DCD     0                      ; 9,  Reserved
                DCD     0                      ; 10, Reserved
                DCD     SVCall_Handler         ; 11, SVCall Handler
                DCD     0                      ; 12, Reserved
                DCD     0                      ; 13, Reserved
                DCD     PendSV_Handler         ; 14, PendSV Handler
                DCD     SysTick_Handler        ; 15, SysTick Handler

                ; External interrupts    
                DCD    EXTI_IRQHandler         ; 0,  EXTI
                DCD    IWDT_IRQHandler         ; 1,  IWDT
                DCD    BLE_IRQHandler          ; 2,  BB
                DCD    DMAC_IRQHandler         ; 3,  DMAChannel
                DCD    BB_LP_IRQHandler        ; 4,  BB_LowPower
                DCD    BTMR_IRQHandler         ; 5,  BTMR 
                DCD    CTMR_IRQHandler         ; 6,  CTMR
                DCD    ATMR_IRQHandler         ; 7,  ATMR
                DCD    RTC_IRQHandler          ; 8,  RTC
                DCD    I2C_IRQHandler          ; 9,  I2C
                DCD    SPIM_IRQHandler         ; 10, SPI Master
                DCD    SPIS_IRQHandler         ; 11, SPI Slave
                DCD    UART1_IRQHandler        ; 12, UART1
                DCD    UART2_IRQHandler        ; 13, UART2
                DCD    AON_PMU_IRQHandler      ; 14, PMU
                DCD    LVD33_IRQHandler        ; 15, LVD
                DCD    BOD12_IRQHandler        ; 16, BOD
                DCD    USB_IRQHandler          ; 17, USB
                DCD    USB_SOF_IRQHandler      ; 18, USB_SOF
                DCD    FSHC_IRQHandler         ; 19, FSHC
                DCD    MDM_IRQHandler          ; 20, MODEM
                DCD    RF_IRQHandler           ; 21, RF
                    
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

;-------------------------------------------------------------------------------
                 AREA    |.INT|, CODE, READONLY 
                
;Reset Handler------------------------------------------------------------------
Reset_Handler   PROC
                EXPORT  Reset_Handler            [WEAK]
                IMPORT  __main
                IMPORT  core_vector
                IMPORT  trim_load
                IMPORT  sys_pre_init
                    
                ;#ifndef CACHE_USE_SRAM
                IF      :LNOT::DEF:CACHE_USE_SRAM
                ;cache cfg
                ;SYSCFG->CACHSRAM_CFG = 0;
                MOVS    R0, #0x00
                LDR     R1, =0x4000102C
                STR     R0, [R1]
                
                ;CACHE->CRCR0.Word = 0x18004025;
                LDR     R0, =0x18004025
                LDR     R1, =0x1900000C
                STR     R0, [R1]
                
                ;CACHE->CCFR.CACHE_INST_DATA = 1;
                ;MOVS    R0, #0x08
                ;LDR     R1, =0x19000004
                ;STR     R0, [R1]
                
                ;CACHE->CCR.CACHE_EN         = 1;
                MOVS    R0, #0x01
                LDR     R1, =0x19000000
                STR     R0, [R1]
                ENDIF

                ; copy vector to sram
                LDR     R0, =__Vectors
                LDR     R1, =core_vector
                BLX     R1
                
                ;APBMISC->XOSC16M_CTRL.Word = 0x00014894;
                ;.XOSC16M_LP=0 .XOSC16M_CAP_TR=0x14
                LDR     R0, =0x00014894
                LDR     R1, =0x40031054
                STR     R0, [R1]
                
                ; load ft trim
                LDR     R1, =trim_load
                BLX     R1

                LDR     R1, =sys_pre_init
                BLX     R1

                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops here, can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler              [WEAK]
                B       .
                ENDP

HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler        [WEAK]
                B       .
                ENDP

SVCall_Handler  PROC
                EXPORT  SVCall_Handler           [WEAK]
                B       .
                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler           [WEAK]
                B       .
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler          [WEAK]
                B       .
                ENDP

;peripheral module int ---------------------------------------------------------
Default_Handler PROC
                EXPORT      EXTI_IRQHandler      [WEAK]
                EXPORT      IWDT_IRQHandler      [WEAK]
                EXPORT      BLE_IRQHandler       [WEAK]
                EXPORT      DMAC_IRQHandler      [WEAK]
                EXPORT      BB_LP_IRQHandler     [WEAK]
                EXPORT      BTMR_IRQHandler      [WEAK]
                EXPORT      CTMR_IRQHandler      [WEAK]
                EXPORT      ATMR_IRQHandler      [WEAK]
                EXPORT      RTC_IRQHandler       [WEAK]
                EXPORT      I2C_IRQHandler       [WEAK]
                EXPORT      SPIM_IRQHandler      [WEAK]
                EXPORT      SPIS_IRQHandler      [WEAK]
                EXPORT      UART1_IRQHandler     [WEAK]
                EXPORT      UART2_IRQHandler     [WEAK]
                EXPORT      AON_PMU_IRQHandler   [WEAK]
                EXPORT      LVD33_IRQHandler     [WEAK]
                EXPORT      BOD12_IRQHandler     [WEAK]
                EXPORT      USB_IRQHandler       [WEAK]
                EXPORT      USB_SOF_IRQHandler   [WEAK]
                EXPORT      FSHC_IRQHandler      [WEAK]
                EXPORT      MDM_IRQHandler       [WEAK]
                EXPORT      RF_IRQHandler        [WEAK]

EXTI_IRQHandler
IWDT_IRQHandler
BLE_IRQHandler
DMAC_IRQHandler
BB_LP_IRQHandler
BTMR_IRQHandler
CTMR_IRQHandler
ATMR_IRQHandler
RTC_IRQHandler
I2C_IRQHandler
SPIM_IRQHandler
SPIS_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
AON_PMU_IRQHandler
LVD33_IRQHandler
BOD12_IRQHandler
USB_IRQHandler
USB_SOF_IRQHandler
FSHC_IRQHandler
MDM_IRQHandler
RF_IRQHandler
                B       .
                ENDP

                ALIGN
;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap
                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END

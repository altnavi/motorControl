/*
 * lpc845.h
 *
 *  Created on: 2 abr. 2023
 *      Author: chor
 */

#ifndef LPC845_H_
#define LPC845_H_

#define PUERTO0 	0
#define PUERTO1 	1

//#define ENTRADA 	0
//#define SALIDA 		1

#define OFF 0
#define ON 1


typedef signed char 	int8_t;
typedef unsigned char 	uint8_t;
typedef short 			int16_t;
typedef unsigned short 	uint16_t;
typedef int 			int32_t;
typedef unsigned int 	uint32_t;


#define   __I     	volatile const       /*!< Defines 'read only' permissions */
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */


/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */


/**
  \ingroup  CMSIS_core_register
  \defgroup CMSIS_SysTick     System Tick Timer (SysTick)
  \brief    Type definitions for the System Timer Registers.
  @{
 */

/**
  \brief  Structure type to access the System Timer (SysTick).
 */
typedef struct
{
  __IOM uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IOM uint32_t LOAD;                   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
  __IOM uint32_t VAL;                    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
  __IM  uint32_t CALIB;                  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
} SysTick_Type;

#define NVIC_BASE		(0xE000E000u)

#define ISER0			(*(uint32_t *) (NVIC_BASE + 0x100))
#define ICER0			(*(uint32_t *) (NVIC_BASE + 0x180))

/*!pu
 * @addtogroup SYSCON_Peripheral_Access_Layer SYSCON Peripheral Access Layer
 * @{
 */

/** SYSCON - Register Layout Typedef */
typedef struct {
  __IO uint32_t SYSMEMREMAP;                       /**< System Remap register, offset: 0x0 */
       uint8_t RESERVED_0[4];
  __IO uint32_t SYSPLLCTRL;                        /**< PLL control, offset: 0x8 */
  __I  uint32_t SYSPLLSTAT;                        /**< PLL status, offset: 0xC */
       uint8_t RESERVED_1[16];
  __IO uint32_t SYSOSCCTRL;                        /**< system oscillator control, offset: 0x20 */
  __IO uint32_t WDTOSCCTRL;                        /**< Watchdog oscillator control, offset: 0x24 */
  __IO uint32_t FROOSCCTRL;                        /**< FRO oscillator control, offset: 0x28 */
       uint8_t RESERVED_2[4];
  __IO uint32_t FRODIRECTCLKUEN;                   /**< FRO direct clock source update enable register, offset: 0x30 */
       uint8_t RESERVED_3[4];
  __IO uint32_t SYSRSTSTAT;                        /**< System reset status register, offset: 0x38 */
       uint8_t RESERVED_4[4];
  __IO uint32_t SYSPLLCLKSEL;                      /**< System PLL clock source select register, offset: 0x40 */
  __IO uint32_t SYSPLLCLKUEN;                      /**< System PLL clock source update enable register, offset: 0x44 */
  __IO uint32_t MAINCLKPLLSEL;                     /**< Main clock source select register, offset: 0x48 */
  __IO uint32_t MAINCLKPLLUEN;                     /**< Main clock source update enable register, offset: 0x4C */
  __IO uint32_t MAINCLKSEL;                        /**< Main clock source select register, offset: 0x50 */
  __IO uint32_t MAINCLKUEN;                        /**< Main clock source update enable register, offset: 0x54 */
  __IO uint32_t SYSAHBCLKDIV;                      /**< System clock divider register, offset: 0x58 */
       uint8_t RESERVED_5[4];
  __IO uint32_t CAPTCLKSEL;                        /**< CAPT clock source select register, offset: 0x60 */
  __IO uint32_t ADCCLKSEL;                         /**< ADC clock source select register, offset: 0x64 */
  __IO uint32_t ADCCLKDIV;                         /**< ADC clock divider register, offset: 0x68 */
  __IO uint32_t SCTCLKSEL;                         /**< SCT clock source select register, offset: 0x6C */
  __IO uint32_t SCTCLKDIV;                         /**< SCT clock divider register, offset: 0x70 */
  __IO uint32_t EXTCLKSEL;                         /**< external clock source select register, offset: 0x74 */
       uint8_t RESERVED_6[8];
  __IO uint32_t SYSAHBCLKCTRL0;                    /**< System clock group 0 control register, offset: 0x80 */
  __IO uint32_t SYSAHBCLKCTRL1;                    /**< System clock group 1 control register, offset: 0x84 */
  __IO uint32_t PRESETCTRL0;                       /**< Peripheral reset group 0 control register, offset: 0x88 */
  __IO uint32_t PRESETCTRL1;                       /**< Peripheral reset group 1 control register, offset: 0x8C */
  __IO uint32_t FCLKSEL[11];                       /**< peripheral clock source select register. FCLK0SEL~FCLK4SEL are for UART0~UART4 clock source select register. FCLK5SEL~FCLK8SEL are for I2C0~I2C3 clock source select register. FCLK9SEL~FCLK10SEL are for SPI0~SPI1 clock source select register., array offset: 0x90, array step: 0x4 */
       uint8_t RESERVED_7[20];
  struct {                                         /* offset: 0xD0, array step: 0x10 */
    __IO uint32_t FRGDIV;                            /**< fractional generator N divider value register, array offset: 0xD0, array step: 0x10 */
    __IO uint32_t FRGMULT;                           /**< fractional generator N multiplier value register, array offset: 0xD4, array step: 0x10 */
    __IO uint32_t FRGCLKSEL;                         /**< FRG N clock source select register, array offset: 0xD8, array step: 0x10 */
         uint8_t RESERVED_0[4];
  } FRG[2];
  __IO uint32_t CLKOUTSEL;                         /**< CLKOUT clock source select register, offset: 0xF0 */
  __IO uint32_t CLKOUTDIV;                         /**< CLKOUT clock divider registers, offset: 0xF4 */
       uint8_t RESERVED_8[4];
  __IO uint32_t EXTTRACECMD;                       /**< External trace buffer command register, offset: 0xFC */
  __I  uint32_t PIOPORCAP[2];                      /**< POR captured PIO N status register(PIO0 has 32 PIOs, PIO1 has 22 PIOs), array offset: 0x100, array step: 0x4 */
       uint8_t RESERVED_9[44];
  __IO uint32_t IOCONCLKDIV6;                      /**< Peripheral clock 6 to the IOCON block for programmable glitch filter, offset: 0x134 */
  __IO uint32_t IOCONCLKDIV5;                      /**< Peripheral clock 6 to the IOCON block for programmable glitch filter, offset: 0x138 */
  __IO uint32_t IOCONCLKDIV4;                      /**< Peripheral clock 4 to the IOCON block for programmable glitch filter, offset: 0x13C */
  __IO uint32_t IOCONCLKDIV3;                      /**< Peripheral clock 3 to the IOCON block for programmable glitch filter, offset: 0x140 */
  __IO uint32_t IOCONCLKDIV2;                      /**< Peripheral clock 2 to the IOCON block for programmable glitch filter, offset: 0x144 */
  __IO uint32_t IOCONCLKDIV1;                      /**< Peripheral clock 1 to the IOCON block for programmable glitch filter, offset: 0x148 */
  __IO uint32_t IOCONCLKDIV0;                      /**< Peripheral clock 0 to the IOCON block for programmable glitch filter, offset: 0x14C */
  __IO uint32_t BODCTRL;                           /**< BOD control register, offset: 0x150 */
  __IO uint32_t SYSTCKCAL;                         /**< System tick timer calibration register, offset: 0x154 */
       uint8_t RESERVED_10[24];
  __IO uint32_t IRQLATENCY;                        /**< IRQ latency register, offset: 0x170 */
  __IO uint32_t NMISRC;                            /**< NMI source selection register, offset: 0x174 */
  __IO uint32_t PINTSEL[8];                        /**< Pin interrupt select registers N, array offset: 0x178, array step: 0x4 */
       uint8_t RESERVED_11[108];
  __IO uint32_t STARTERP0;                         /**< Start logic 0 pin wake-up enable register 0, offset: 0x204 */
       uint8_t RESERVED_12[12];
  __IO uint32_t STARTERP1;                         /**< Start logic 0 pin wake-up enable register 1, offset: 0x214 */
       uint8_t RESERVED_13[24];
  __IO uint32_t PDSLEEPCFG;                        /**< Deep-sleep configuration register, offset: 0x230 */
  __IO uint32_t PDAWAKECFG;                        /**< Wake-up configuration register, offset: 0x234 */
  __IO uint32_t PDRUNCFG;                          /**< Power configuration register, offset: 0x238 */
       uint8_t RESERVED_14[444];
  __I  uint32_t DEVICE_ID;                         /**< Part ID register, offset: 0x3F8 */
} SYSCON_Type;


/*! @name SYSAHBCLKCTRL0 - System clock group 0 control register */
/*! @{ */
#define SYSCON_SYSAHBCLKCTRL0_GPIO0_MASK         (0x40U)
#define SYSCON_SYSAHBCLKCTRL0_GPIO0_SHIFT        (6U)
#define SYSCON_SYSAHBCLKCTRL0_GPIO1_MASK         (0x100000U)
#define SYSCON_SYSAHBCLKCTRL0_GPIO1_SHIFT        (20U)



/* SYSCON - Peripheral instance base addresses */
/** Peripheral SYSCON base address */
#define SYSCON_BASE                              (0x40048000u)
/** Peripheral SYSCON base pointer */
#define SYSCON                                   ((SYSCON_Type *)SYSCON_BASE)
/** Array initializer of SYSCON peripheral base addresses */


/*!
 * @addtogroup GPIO_Peripheral_Access_Layer GPIO Peripheral Access Layer
 * @{
 */

/** GPIO - Register Layout Typedef */
typedef struct {
  __IO uint8_t B[2][32];                           /**< Byte pin registers for all port 0 and 1 GPIO pins, array offset: 0x0, array step: index*0x20, index2*0x1 */
       uint8_t RESERVED_0[4032];
  __IO uint32_t W[2][32];                          /**< Word pin registers for all port 0 and 1 GPIO pins, array offset: 0x1000, array step: index*0x80, index2*0x4 */
       uint8_t RESERVED_1[3840];
  __IO uint32_t DIR[2];                            /**< Direction registers, array offset: 0x2000, array step: 0x4 */
       uint8_t RESERVED_2[120];
  __IO uint32_t MASK[2];                           /**< Mask register, array offset: 0x2080, array step: 0x4 */
       uint8_t RESERVED_3[120];
  __IO uint32_t PIN[2];                            /**< Port pin register, array offset: 0x2100, array step: 0x4 */
       uint8_t RESERVED_4[120];
  __IO uint32_t MPIN[2];                           /**< Masked port register, array offset: 0x2180, array step: 0x4 */
       uint8_t RESERVED_5[120];
  __IO uint32_t SET[2];                            /**< Write: Set register for port Read: output bits for port, array offset: 0x2200, array step: 0x4 */
       uint8_t RESERVED_6[120];
  __O  uint32_t CLR[2];                            /**< Clear port, array offset: 0x2280, array step: 0x4 */
       uint8_t RESERVED_7[120];
  __O  uint32_t NOT[2];                            /**< Toggle port, array offset: 0x2300, array step: 0x4 */
       uint8_t RESERVED_8[120];
  __O  uint32_t DIRSET[2];                         /**< Set pin direction bits for port, array offset: 0x2380, array step: 0x4 */
       uint8_t RESERVED_9[120];
  __O  uint32_t DIRCLR[2];                         /**< Clear pin direction bits for port, array offset: 0x2400, array step: 0x4 */
       uint8_t RESERVED_10[120];
  __O  uint32_t DIRNOT[2];                         /**< Toggle pin direction bits for port, array offset: 0x2480, array step: 0x4 */
} GPIO_Type;





/* ----------------------------------------------------------------------------
   -- GPIO Register Masks
   ---------------------------------------------------------------------------- */



/* GPIO - Peripheral instance base addresses */
/** Peripheral GPIO base address */
#define GPIO_BASE                                (0x0A0000000u)
/** Peripheral GPIO base pointer */
#define GPIO                                     ((GPIO_Type *)GPIO_BASE)


/* ----------------------------------------------------------------------------
   -- IOCON Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup IOCON_Peripheral_Access_Layer IOCON Peripheral Access Layer
 * @{
 */

/** IOCON - Register Layout Typedef */
typedef struct {
  __IO uint32_t PIO[56];                           /**< Digital I/O control for pins PIO0_17..Digital I/O control for pins PIO1_10, array offset: 0x0, array step: 0x4 */
} IOCON_Type;

/* ----------------------------------------------------------------------------
   -- IOCON Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup IOCON_Register_Masks IOCON Register Masks
 * @{
 */

/*! @name PIO - Digital I/O control for pins PIO0_17..Digital I/O control for pins PIO1_10 */
/*! @{ */
#define IOCON_PIO_MODE_MASK                      (0x18U)
#define IOCON_PIO_MODE_SHIFT                     (3U)
/*! MODE - Selects function mode (on-chip pull-up/pull-down resistor control).
 *  0b00..Inactive. Inactive (no pull-down/pull-up resistor enabled).
 *  0b01..Pull-down. Pull-down resistor enabled.
 *  0b10..Pull-up. Pull-up resistor enabled.
 *  0b11..Repeater. Repeater mode.
 */
#define IOCON_PIO_OD_MASK                        (0x400U)
#define IOCON_PIO_OD_SHIFT                       (10U)
/*! OD - Open-drain mode.
 *  0b0..Disable.
 *  0b1..Open-drain mode enabled. Remark: This is not a true open-drain mode.
 */
/* The count of IOCON_PIO */
#define IOCON_PIO_COUNT                          (56U)


/*!
 * @}
 */ /* end of group IOCON_Register_Masks */


/* IOCON - Peripheral instance base addresses */
/** Peripheral IOCON base address */
#define IOCON_BASE                               (0x40044000u)
/** Peripheral IOCON base pointer */
#define IOCON                                    ((IOCON_Type *)IOCON_BASE)
/** Array initializer of IOCON peripheral base addresses */
#define IOCON_BASE_ADDRS                         { IOCON_BASE }
/** Array initializer of IOCON peripheral base pointers */
#define IOCON_BASE_PTRS                          { IOCON }


/*!
 * @}
 */ /* end of group IOCON_Peripheral_Access_Layer */



/*
 * Definiciones para interrupciones externas
 */

typedef struct {
  __IO uint32_t ISEL;
  __IO uint32_t IENR;
  __O  uint32_t SIENR;
  __O  uint32_t CIENR;
  __IO uint32_t IENF;
  __O  uint32_t SIENF;
  __O  uint32_t CIENF;
  __IO uint32_t RISE;
  __IO uint32_t FALL;
  __IO uint32_t IST;
  __IO uint32_t PMCTRL;
  __IO uint32_t PMSRC;
  __IO uint32_t PMCFG;
} INT_EXT_Types;



/* GPIO - Peripheral instance base addresses */
/** Peripheral GPIO base address */
#define INT_EXT_BASE                                (0x0A0004000u)
/** Peripheral GPIO base pointer */
#define INT_EXT                                     ((INT_EXT_Types *)INT_EXT_BASE)


/*
 * Definiciones para Systick
 */

typedef struct {
  __IO uint32_t RESERVED[4];
  __IO uint32_t CSR;
  __IO uint32_t RVR;
  __IO uint32_t CVR;
  __IO uint32_t CALIB;

} SYSTICK_Types;



/* Systick - base addresses */
/** Peripheral Systick base address */
#define SYSTICK_BASE                                (0x0E000E000u)
/** Peripheral Systick base pointer */
#define SYSTICK                                     ((SYSTICK_Types *)SYSTICK_BASE)

typedef struct {
  __IO uint32_t CFG;
  __IO uint32_t CTL;
  __IO uint32_t STAT;
  __IO uint32_t INTENSET;
  __IO uint32_t INTENCLR;
  __IO uint32_t RXDAT;
  __IO uint32_t RXDATSTAT;
  __IO uint32_t TXDAT;
  __IO uint32_t BRG;
  __IO uint32_t INTSTAT;
  __IO uint32_t OSR;
  __IO uint32_t ADDR;

} USARTs;


#define USART0_BASE		(0x040064000u)
#define USART0      	((USARTs *)USART0_BASE)

#define USART1_BASE		(0x040068000u)
#define USART1      	((USARTs *)USART1_BASE)

#define USART2_BASE		(0x04006C000u)
#define USART2      	((USARTs *)USART2_BASE)

#define USART3_BASE		(0x040070000u)
#define USART3      	((USARTs *)USART3_BASE)

#define USART4_BASE		(0x040074000u)
#define USART4      	((USARTs *)USART4_BASE)



#define PINASSIGN0		(*((uint32_t *)0x04000C000u))
#define PINASSIGN1		(*((uint32_t *)0x04000C004u))
#define PINASSIGN2		(*((uint32_t *)0x04000C008u))
#define PINASSIGN3		(*((uint32_t *)0x04000C00Cu))
#define PINASSIGN4		(*((uint32_t *)0x04000C010u))
#define PINASSIGN5		(*((uint32_t *)0x04000C014u))
#define PINASSIGN6		(*((uint32_t *)0x04000C018u))
#define PINASSIGN7		(*((uint32_t *)0x04000C01Cu))
#define PINASSIGN8		(*((uint32_t *)0x04000C020u))
#define PINASSIGN9		(*((uint32_t *)0x04000C024u))
#define PINASSIGN10		(*((uint32_t *)0x04000C028u))
#define PINASSIGN11		(*((uint32_t *)0x04000C02Cu))
#define PINASSIGN12		(*((uint32_t *)0x04000C030u))
#define PINASSIGN13		(*((uint32_t *)0x04000C034u))
#define PINASSIGN14		(*((uint32_t *)0x04000C038u))

#define PINENABLE0		(*(uint32_t *)0x04000C1C0u)
#define PINENABLE1		(*(uint32_t *)0x04000C1C4u)

#define	UART0CLKSEL		(*((uint32_t *)0x040048090u))
#define	UART1CLKSEL		(*((uint32_t *)0x040048094u))
#define	UART2CLKSEL		(*((uint32_t *)0x040048098u))
#define	UART3CLKSEL		(*((uint32_t *)0x04004809Cu))
#define	UART4CLKSEL		(*((uint32_t *)0x0400480A0u))

typedef struct {
  __IO uint32_t CTRL;
  __IO uint32_t RESERVED;
  __IO uint32_t SEQA_CTRL;
  __IO uint32_t SEQB_CTRL;
  __IO uint32_t SEQA_GDAT;
  __IO uint32_t SEQB_GDAT;
  __IO uint32_t RESERVED2;
  __IO uint32_t RESERVED3;
  __IO uint32_t DAT0;
  __IO uint32_t DAT1;
  __IO uint32_t DAT2;
  __IO uint32_t DAT3;
  __IO uint32_t DAT4;
  __IO uint32_t DAT5;
  __IO uint32_t DAT6;
  __IO uint32_t DAT7;
  __IO uint32_t DAT8;
  __IO uint32_t DAT9;
  __IO uint32_t DAT10;
  __IO uint32_t DAT11;
  __IO uint32_t THR0_LOW;
  __IO uint32_t THR1_LOW;
  __IO uint32_t THR0_HIGH;
  __IO uint32_t THR1_HIGH;
  __IO uint32_t CHAN_THRSEL;
  __IO uint32_t INTEN;
  __IO uint32_t FLAGS;
  __IO uint32_t TRM;

} ADC_Types;


#define ADC_BASE		(0x04001C000u)
#define ADC      		((ADC_Types *)ADC_BASE)

#endif /* LPC845_H_ */

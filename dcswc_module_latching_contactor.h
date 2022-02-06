#include <18F14K22.h>
#device ADC=10
#device *=16
#use delay(clock=16MHz)

/* hardware I2C port is slave and is connected to DCSWC bus */
#use i2c(stream=STREAM_SLAVE,SLAVE, I2C1, FORCE_HW)
/* slave address set based on dip switch in init() */


#fuses INTRC_IO
#fuses NOPLLEN
#fuses NOFCMEN
#fuses NOIESO
#fuses PUT
#fuses BROWNOUT
#fuses WDT512
#fuses NOHFOFST
#fuses NOMCLR
#fuses STVREN
#fuses NOLVP
#fuses NOXINST
#fuses NODEBUG
#fuses NOPROTECT
#fuses NOWRT
#fuses NOWRTC 
#fuses NOWRTB
#fuses NOWRTD
#fuses NOEBTR
#fuses NOEBTRB

#use standard_io(ALL)

#use rs232(UART1,stream=STREAM_FTDI,baud=57600,errors)	

/* program config CRC of 0 and a serial_prefix of 'A' ... that will trigger a write default on first boot */
#ROM 0xF00000 = { 0x00, 0x00, 0x40, 0x00 }




#define SW_OVERRIDE_A        PIN_A5
#define SW_OVERRIDE_B        PIN_A4
#define BRIDGE_B_A           PIN_C5
#define BRIDGE_A_B           PIN_C4
#define BRIDGE_A_A           PIN_C3
#define BRIDGE_B_B           PIN_C6
#define AN_DIP               PIN_C7
#define SER_TO_PC            PIN_B7
#define AN_VTEMP             PIN_A2
#define AN_IN_VOLTS          PIN_C0
#define LED_A                PIN_C1
#define LED_B                PIN_C2
#define I2C_SDA              PIN_B4
#define SER_FROM_PC          PIN_B5
#define I2C_SCL              PIN_B6


/* 
Parameters are stored in EEPROM
*/
#define PARAM_CRC_ADDRESS  0x00
#define PARAM_ADDRESS      PARAM_CRC_ADDRESS+2



/* bit positions for a channel state byte */
#define CH_STATE_BIT_FUTURE   7  /* reserved for future use */
#define CH_STATE_BIT_HTD      6  /* high temperature disconnect */
#define CH_STATE_BIT_LTD      5  /* low temperature disconnect */
#define CH_STATE_BIT_HVD      4  /* high voltage disconnect */
#define CH_STATE_BIT_LVD      3  /* low voltage disconnect */
#define CH_STATE_BIT_CMD_OFF  2  /* commanded off */
#define CH_STATE_BIT_CMD_ON   1  /* commanded on (takes presedence over commanded off) */
#define CH_STATE_BIT_OVERRIDE 0  /* override switch */

/* _OVERRIDE or _CMD_ON set means output on */
#define CH_STATE_MASK_ON     0b00000011 
/* _CMD_OFF, _LVD, _HVD, _LTD, _HTD means output off, unless override by on mask */
#define CH_STATE_MASK_OFF    0b01111100 

#define ADC_SAMPLE_TICKS              20
#define CONTACTOR_POWER_SAVE_MS       200 /* milliseconds for contactor be on. Must be >0 and <= 255 */
#define CONTACTOR_REFRESH_CYCLES      10  /* re-appply contactor coil voltage every N cycles. 65525 disables */

#define BLINK_ON_TIME                 250
#define BLINK_OFF_TIME                100

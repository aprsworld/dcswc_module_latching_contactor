#include "registers_dcswc_module_latching_contactor.h"

void write_i2c(int8 address, int16 value) {

#if 0
		case I2C_REG_COMMAND_OFF:
			timers.command_off_seconds=value;
			break;

		case I2C_REG_CONFIG_PARAM_WRITE:
			if ( 1 == value ) {
				timers.now_write_config=1;
			} else if ( 2 == value ) {
				timers.now_reset_config=1;
			} else if ( 1802 == value ) {
				current.factory_unlocked =1;
			} else if ( 65535 == value ) {
				reset_cpu();
			}
			break;
		case I2C_REG_CONFIG_TICKS_ADC:
			config.adc_sample_ticks=value;
			break;
		case I2C_REG_CONFIG_STARTUP_POWER_ON_DELAY:
			config.startup_power_on_delay=value;
			break;
		case I2C_REG_CONFIG_COMMAND_OFF_HOLD_TIME:
			config.command_off_hold_time=value;
			break;
		case I2C_REG_CONFIG_LVD_DISCONNECT_VOLTAGE:
			config.lvd_disconnect_adc=value;
			break;
		case I2C_REG_CONFIG_LVD_DISCONNECT_DELAY:
			config.lvd_disconnect_delay=value;
			break;
		case I2C_REG_CONFIG_LVD_RECONNECT_VOLTAGE:
			config.lvd_reconnect_adc=value;
			break;
		case I2C_REG_CONFIG_HVD_DISCONNECT_VOLTAGE:
			config.hvd_disconnect_adc=value;
			break;
		case I2C_REG_CONFIG_HVD_DISCONNECT_DELAY:
			config.hvd_disconnect_delay=value;
			break;
		case I2C_REG_CONFIG_HVD_RECONNECT_VOLTAGE:
			config.hvd_reconnect_adc=value;
			break;
		default:
			/* do nothing */
	}
#endif

}


int16 map_i2c(int8 addr) {
	int8 c;

	/* c is the channel we are accessing based on register range */
	if ( addr >= I2C_REG_CH1_COMMAND_ON && addr <= I2C_REG_CH1_FUT_RECONNECT ) {
		/* channel 1 status region */
		c=1;
		/* remap down to channel 0 region */
		addr -= (I2C_REG_CH1_COMMAND_ON - I2C_REG_CH0_COMMAND_ON);
	} else if ( addr >= I2C_REG_CH1_COMMAND_ON_HOLD && addr <= I2C_REG_CH1_FUT_RECONNECT_DELAY ) {
		/* channel 1 configuration region */
		c=1;
		/* remap down to channel 0 region */
		addr -= (I2C_REG_CH1_COMMAND_ON_HOLD - I2C_REG_CH0_COMMAND_ON_HOLD );
	} else {
		c=0;
	}

	timers.led_on_a=100;

	switch ( addr ) {
		/* not channel based */
		case I2C_REG_VOLTAGE_INPUT_AVG: 
			return (int16) adc_get(0);
		case I2C_REG_STATE_CONTACTORS:
			return (int16) make16(channel[0].state,channel[1].state);
		case I2C_REG_TEMPERATURE_BOARD_AVG: 
			return (int16) adc_get(1);

		/* status based on channel value c */
		case I2C_REG_CH0_COMMAND_ON :
			return (int16) channel[c].command_on_seconds;
		case I2C_REG_CH0_COMMAND_ON_HOLD:
			return (int16) channel[c].command_on_hold_seconds;

		case I2C_REG_CH0_COMMAND_OFF:
			return (int16) channel[c].command_off_seconds;
		case I2C_REG_CH0_COMMAND_OFF_HOLD:
			return (int16) channel[c].command_off_hold_seconds;

		case I2C_REG_CH0_LVD_DISCONNECT:
			return (int16) channel[c].lvd_disconnect_delay_seconds;
		case I2C_REG_CH0_LVD_RECONNECT:
			return (int16) channel[c].lvd_reconnect_delay_seconds;

		case I2C_REG_CH0_HVD_DISCONNECT:
			return (int16) channel[c].hvd_disconnect_delay_seconds;
		case I2C_REG_CH0_HVD_RECONNECT:
			return (int16) channel[c].hvd_reconnect_delay_seconds;

		case I2C_REG_CH0_LTD_DISCONNECT:
			return (int16) channel[c].ltd_disconnect_delay_seconds;
		case I2C_REG_CH0_LTD_RECONNECT:
			return (int16) channel[c].ltd_reconnect_delay_seconds;

		case I2C_REG_CH0_HTD_DISCONNECT:
			return (int16) channel[c].htd_disconnect_delay_seconds;
		case I2C_REG_CH0_HTD_RECONNECT:
			return (int16) channel[c].htd_reconnect_delay_seconds;

		case I2C_REG_CH0_FUT_DISCONNECT:
		case I2C_REG_CH0_FUT_RECONNECT:
			return (int16) 0xffff; /* not yet implemented */


		/* non-channel based meta */
		case I2C_REG_SEQUENCE_NUMBER:
			return (int16) current.sequence_number;
		case I2C_REG_TIME_INTERVAL_MILLISECONDS:
			return (int16) current.interval_milliseconds;
		case I2C_REG_TIME_UPTIME_MINUTES:
			return (int16) current.uptime_minutes;
		case I2C_REG_DEFAULT_PARAMS_WRITTEN:
			return (int16) current.default_params_written;

		/* channel based configuration */
		case I2C_REG_CH0_COMMAND_ON_HOLD_INITIAL:
			return (int16) config.ch[c].command_on_hold_time;

		case I2C_REG_CH0_COMMAND_OFF_HOLD_INITIAL:
			return (int16) config.ch[c].command_off_hold_time;

		case I2C_REG_CH0_LVD_DISCONNECT_ADC:
			return (int16) config.ch[c].lvd_disconnect_adc;
		case I2C_REG_CH0_LVD_DISCONNECT_DELAY:
			return (int16) config.ch[c].lvd_disconnect_delay;
		case I2C_REG_CH0_LVD_RECONNECT_ADC:
			return (int16) config.ch[c].lvd_reconnect_adc;
		case I2C_REG_CH0_LVD_RECONNECT_DELAY:
			return (int16) config.ch[c].lvd_reconnect_delay;

		case I2C_REG_CH0_HVD_DISCONNECT_ADC:
			return (int16) config.ch[c].hvd_disconnect_adc;
		case I2C_REG_CH0_HVD_DISCONNECT_DELAY:
			return (int16) config.ch[c].hvd_disconnect_delay;
		case I2C_REG_CH0_HVD_RECONNECT_ADC:
			return (int16) config.ch[c].hvd_reconnect_adc;
		case I2C_REG_CH0_HVD_RECONNECT_DELAY:
			return (int16) config.ch[c].hvd_reconnect_delay;

		case I2C_REG_CH0_LTD_DISCONNECT_ADC:
			return (int16) config.ch[c].ltd_disconnect_adc;
		case I2C_REG_CH0_LTD_DISCONNECT_DELAY:
			return (int16) config.ch[c].ltd_disconnect_delay;
		case I2C_REG_CH0_LTD_RECONNECT_ADC:
			return (int16) config.ch[c].ltd_reconnect_adc;
		case I2C_REG_CH0_LTD_RECONNECT_DELAY:
			return (int16) config.ch[c].ltd_reconnect_delay;

		case I2C_REG_CH0_HTD_DISCONNECT_ADC:
			return (int16) config.ch[c].ltd_disconnect_adc;
		case I2C_REG_CH0_HTD_DISCONNECT_DELAY:
			return (int16) config.ch[c].ltd_disconnect_delay;
		case I2C_REG_CH0_HTD_RECONNECT_ADC:
			return (int16) config.ch[c].ltd_reconnect_adc;
		case I2C_REG_CH0_HTD_RECONNECT_DELAY:
			return (int16) config.ch[c].ltd_reconnect_delay;	

		case I2C_REG_CH0_FUT_DISCONNECT_VALUE:
		case I2C_REG_CH0_FUT_DISCONNECT_DELAY:
		case I2C_REG_CH0_FUT_RECONNECT_VALUE:
		case I2C_REG_CH0_FUT_RECONNECT_DELAY:
			return (int16) 0xffff; /* not yet implemented */
		
		/* configuration */
		case I2C_REG_CONFIG_HARDWARE_MODEL: 
			return (int16) 'D';
		case I2C_REG_CONFIG_HARDWARE_VERSION: 
			return (int16) '2';
		case I2C_REG_CONFIG_SOFTWARE_MODEL: 
			return (int16) 'L';
		case I2C_REG_CONFIG_SOFTWARE_VERSION: 
			return (int16) '0';
		case I2C_REG_CONFIG_SOFTWARE_YEAR:
			return (int16) current.compile_year;
		case I2C_REG_CONFIG_SOFTWARE_MONTH:
			return (int16) current.compile_month;
		case I2C_REG_CONFIG_SOFTWARE_DAY:
			return (int16) current.compile_day;

		case I2C_REG_CONFIG_PARAM_WRITE:
			/* 1 if factory unlocked */ 
			return (int16) current.factory_unlocked; 	

		/* we should have range checked, and never gotten here ... or read unimplemented (future) register */
		default: return (int16) 0xffff;
	}


}



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

	timers.led_on_a=100;

	switch ( addr ) {
		case I2C_REG_VOLTAGE_INPUT_AVG: 
			return (int16) adc_get(0);
		case I2C_REG_STATE_CONTACTORS:
			return (int16) make16(channel[0].state,channel[1].state);
		case I2C_REG_TEMPERATURE_BOARD_AVG: 
			return (int16) adc_get(1);


		case I2C_REG_VOLTAGE_INPUT_NOW: 
			return (int16) current.adc_buffer[0][current.adc_buffer_index];
		case I2C_REG_TEMPERATURE_BOARD_NOW: 
			return (int16) current.adc_buffer[1][current.adc_buffer_index];

		case I2C_REG_COMMAND_ON_A:
			return (int16) channel[0].command_on_seconds;
		case I2C_REG_COMMAND_ON_B:
			return (int16) channel[1].command_on_seconds;

		case I2C_REG_COMMAND_OFF_A:
			return (int16) channel[0].command_off_seconds;
		case I2C_REG_COMMAND_OFF_B:
			return (int16) channel[1].command_off_seconds;

		case I2C_REG_LVD_DISCONNECT_A:
			return (int16) channel[0].lvd_disconnect_delay_seconds;
		case I2C_REG_LVD_DISCONNECT_B:
			return (int16) channel[1].lvd_disconnect_delay_seconds;

		case I2C_REG_HVD_DISCONNECT_A:
			return (int16) channel[0].hvd_disconnect_delay_seconds;
		case I2C_REG_HVD_DISCONNECT_B:
			return (int16) channel[1].hvd_disconnect_delay_seconds;

		case I2C_REG_LTD_DISCONNECT_A:
			return (int16) channel[0].ltd_disconnect_delay_seconds;
		case I2C_REG_LTD_DISCONNECT_B:
			return (int16) channel[1].ltd_disconnect_delay_seconds;


		case I2C_REG_HTD_DISCONNECT_A:
			return (int16) channel[0].htd_disconnect_delay_seconds;
		case I2C_REG_HTD_DISCONNECT_B:
			return (int16) channel[1].htd_disconnect_delay_seconds;

		case I2C_REG_FUT_DISCONNECT_A:
			return (int16) 0xffff; /* not yet implemented */
		case I2C_REG_FUT_DISCONNECT_B:
			return (int16) 0xffff; /* not yet implemented */

		case I2C_REG_SEQUENCE_NUMBER:
			return (int16) current.sequence_number;
		case I2C_REG_TIME_INTERVAL_MILLISECONDS:
			return (int16) current.interval_milliseconds;
		case I2C_REG_TIME_UPTIME_MINUTES:
			return (int16) current.uptime_minutes;
		case I2C_REG_DEFAULT_PARAMS_WRITTEN:
			return (int16) current.default_params_written;

#if 0		
		/* configuration */
		case I2C_REG_CONFIG_HARDWARE_MODEL: 
			return (int16) 'P';
		case I2C_REG_CONFIG_HARDWARE_VERSION: 
			return (int16) 'Z';
		case I2C_REG_CONFIG_SOFTWARE_MODEL: 
			return (int16) 'P';
		case I2C_REG_CONFIG_SOFTWARE_VERSION: 
			return (int16) 3;
		case I2C_REG_CONFIG_SOFTWARE_YEAR:
			return (int16) current.compile_year;
		case I2C_REG_CONFIG_SOFTWARE_MONTH:
			return (int16) current.compile_month;
		case I2C_REG_CONFIG_SOFTWARE_DAY:
			return (int16) current.compile_day;

		case I2C_REG_CONFIG_PARAM_WRITE:
			/* 1 if factory unlocked */ 
			return (int16) current.factory_unlocked; 	
		case I2C_REG_CONFIG_TICKS_ADC: 
			return (int16) config.adc_sample_ticks;
		case I2C_REG_CONFIG_STARTUP_POWER_ON_DELAY: 
			return (int16) config.startup_power_on_delay;
		case I2C_REG_CONFIG_COMMAND_OFF_HOLD_TIME:
			return (int16) config.command_off_hold_time;
		case I2C_REG_CONFIG_LVD_DISCONNECT_VOLTAGE:
			return (int16) config.lvd_disconnect_adc;
		case I2C_REG_CONFIG_LVD_DISCONNECT_DELAY:
			return (int16) config.lvd_disconnect_delay;
		case I2C_REG_CONFIG_LVD_RECONNECT_VOLTAGE:
			return (int16) config.lvd_reconnect_adc;
		case I2C_REG_CONFIG_HVD_DISCONNECT_VOLTAGE:
			return (int16) config.hvd_disconnect_adc;
		case I2C_REG_CONFIG_HVD_DISCONNECT_DELAY:
			return (int16) config.hvd_disconnect_delay;
		case I2C_REG_CONFIG_HVD_RECONNECT_VOLTAGE:
			return (int16) config.hvd_reconnect_adc;

#endif

		/* we should have range checked, and never gotten here ... or read unimplemented (future) register */
		default: return (int16) addr;
	}


}



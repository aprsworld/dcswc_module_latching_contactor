#include "dcswc_module_latching_contactor.h"


typedef struct {
	int16 adc_sample_ticks;

	int16 startup_power_on_delay;

	/* command_off in current */
	int16 command_off_hold_time;

	int16 lvd_disconnect_adc;
	int16 lvd_disconnect_delay;
	int16 lvd_reconnect_adc;

	int16 hvd_disconnect_adc;
	int16 hvd_disconnect_delay;
	int16 hvd_reconnect_adc;
} struct_config;



typedef struct {
	/* circular buffer for ADC readings */
	int16 adc_buffer[3][16];
	int8  adc_buffer_index;

	int16 sequence_number;
	int16 uptime_minutes;
	int16 interval_milliseconds;

	int8 factory_unlocked;


	int8 compile_year;
	int8 compile_month;
	int8 compile_day;

	/* bit position
		7
		6 htd
		5 ltd
		4 hvd
		3 lvd
		2 write watchdog
		1 read watchdog
		0 command
	*/
	int8 power_off_flags; 

	/* bit positions
		7
		6
		5
		4
		3
		2
		1 usb (wifi)
		0 pi (host)
	*/


	int8 default_params_written;
} struct_current;

typedef struct {
	/* action flags */
	int1 now_adc_sample;
	int1 now_adc_reset_count;

	int1 now_millisecond;

	int1 now_write_config;
	int1 now_reset_config;

	/* timers */
	int8 led_on_a;
	int8 led_on_b;

	int16 command_off_seconds;			/* counts down. Off at zero. */
	int16 command_off_hold_seconds;     /* counts down. Off at zero. */

	int16 lvd_disconnect_delay_seconds;	/* counts down */
	int8  lvd_reconnect_delay_seconds;	/* counts down */

	int16 hvd_disconnect_delay_seconds;	/* counts down */
	int8  hvd_reconnect_delay_seconds;	/* counts down */

	int8  contactor_a_powersave;        /* counts down. Off at zero. */
	int8  contactor_b_powersave;        /* counts down. Off at zero. */

} struct_time_keep;

/* global structures */
struct_config config={0};
struct_current current={0};
struct_time_keep timers={0};

#include "adc_dcswc_module_latching_contactor.c"
#include "param_dcswc_module_latching_contactor.c"
#include "i2c_handler_dcswc_module_latching_contactor.c"
#include "interrupt_dcswc_module_latching_contactor.c"

void init(void) {
	int8 buff[32];
	setup_oscillator(OSC_16MHZ);

	setup_adc(ADC_CLOCK_DIV_16);
	/* NTC thermistor on sAN2, input voltage divider on sAN4, DIP switch analog on sAN9
	voltage spans between 0 and Vdd */
	setup_adc_ports(sAN2 | sAN4 | sAN9,VSS_VDD);



	set_tris_a(0b00111111);
	set_tris_b(0b01110000);
	set_tris_c(0b10000001);
//               76543210

	port_a_pullups(0b00110000);
	port_b_pullups(0b00000000);
//                   76543210

	/* data structure initialization */
	/* all initialized to 0 on declaration. Just do this if need non-zero */
	timers.command_off_seconds=65535;

	/* get our compiled date from constant */
	strcpy(buff,__DATE__);
	current.compile_day =(buff[0]-'0')*10;
	current.compile_day+=(buff[1]-'0');
	/* determine month ... how annoying */
	if ( 'J'==buff[3] ) {
		if ( 'A'==buff[4] )
			current.compile_month=1;
		else if ( 'N'==buff[5] )
			current.compile_month=6;
		else
			current.compile_month=7;
	} else if ( 'A'==buff[3] ) {
		if ( 'P'==buff[4] )
			current.compile_month=4;
		else
			current.compile_month=8;
	} else if ( 'M'==buff[3] ) {
		if ( 'R'==buff[5] )
			current.compile_month=3;
		else
			current.compile_month=5;
	} else if ( 'F'==buff[3] ) {
		current.compile_month=2;
	} else if ( 'S'==buff[3] ) {
		current.compile_month=9;
	} else if ( 'O'==buff[3] ) {
		current.compile_month=10;
	} else if ( 'N'==buff[3] ) {
		current.compile_month=11;
	} else if ( 'D'==buff[3] ) {
		current.compile_month=12;
	} else {
		/* error parsing, shouldn't happen */
		current.compile_month=255;
	}
	current.compile_year =(buff[7]-'0')*10;
	current.compile_year+=(buff[8]-'0');


	/* one periodic interrupt @ 1mS. Generated from system 16 MHz clock */
	/* prescale=16, match=249, postscale=1. Match is 249 because when match occurs, one cycle is lost */
	setup_timer_2(T2_DIV_BY_16,249,1);

	enable_interrupts(INT_TIMER2);
}

int8 read_dip_switch(void) {
	int16 adc;

	set_adc_channel(9);
	delay_ms(1);
	adc=read_adc();

	/* (note that table is sorted by vout reading 
	SW3.1 (LSB) SW3.2 (MSB) VALUE ADC
    OFF         OFF         0     1023
	OFF         ON          2     682
    ON          OFF         1     511
	ON          ON          3     409
	*/

	return adc;

	if ( adc > (1023-64) )
		return 0;
	if ( adc > (682-64) )
		return 2;
	if ( adc > (511-64) )
		return 1;

	return 3;
}

void contactor_on_a(void) {
	timers.contactor_a_powersave=CONTACTOR_POWER_SAVE_MS;

	output_high(BRIDGE_A_A);
	output_low(BRIDGE_A_B);
}

void contactor_off_a(void) {
	timers.contactor_a_powersave=CONTACTOR_POWER_SAVE_MS;

	output_low(BRIDGE_A_A);
	output_high(BRIDGE_A_B);
}

void contactor_on_b(void) {
	timers.contactor_b_powersave=CONTACTOR_POWER_SAVE_MS;

	output_high(BRIDGE_B_A);
	output_low(BRIDGE_B_B);
}

void contactor_off_b(void) {
	timers.contactor_b_powersave=CONTACTOR_POWER_SAVE_MS;

	output_low(BRIDGE_B_A);
	output_high(BRIDGE_B_B);
}

void periodic_millisecond(void) {
	static int8 uptimeticks=0;
	static int16 adcTicks=0;
	static int16 ticks=0;
	int16 adc;


	timers.now_millisecond=0;

	/* LED control */
	if ( 0==timers.led_on_a ) {
		output_low(LED_A);
	} else {
		output_high(LED_A);
		timers.led_on_a--;
	}
	if ( 0==timers.led_on_b ) {
		output_low(LED_B);
	} else {
		output_high(LED_B);
		timers.led_on_b--;
	}

	/* some other random stuff that we don't need to do every cycle in main */
	if ( current.interval_milliseconds < 65535 ) {
		current.interval_milliseconds++;
	}

	/* contactor timeout */
	if ( 0 == timers.contactor_a_powersave ) {
		output_low(BRIDGE_A_A);
		output_low(BRIDGE_A_B);
	} else {
		timers.contactor_a_powersave--;
	}
	if ( 0 == timers.contactor_b_powersave ) {
		output_low(BRIDGE_B_A);
		output_low(BRIDGE_B_B);
	} else {
		timers.contactor_b_powersave--;
	}


	/* seconds */
	ticks++;
	if ( 1000 == ticks ) {
		ticks=0;

		/* command off. 65535 disables */
		if ( 65535 != timers.command_off_seconds ) {
			if ( timers.command_off_seconds > 0 ) {
				/* waiting to power off */
				timers.command_off_seconds--;
			} else {
				/* timer at zero, ready to power off or already powered off */
				if ( ! bit_test(current.power_off_flags,POWER_FLAG_POS_COMMAND_OFF) ) {
					/* not currently set, so we set it and start the countdown */
					bit_set(current.power_off_flags,POWER_FLAG_POS_COMMAND_OFF);
					timers.command_off_hold_seconds=config.command_off_hold_time;
				} else {
					/* set, so we clear it once countdown has elapsed */
					if ( 0==timers.command_off_hold_seconds ) {
						/* countdown elapsed, clear the flag and reset the timer */
						bit_clear(current.power_off_flags,POWER_FLAG_POS_COMMAND_OFF);
						timers.command_off_seconds=65535;
					} else {
						timers.command_off_hold_seconds--;
					}
				}		
			}
		}

		/* LVD. 65535 disables */
		if ( 65535 != config.lvd_disconnect_delay ) {
			adc=adc_get(0);

			if ( adc > config.lvd_reconnect_adc ) {
				if ( timers.lvd_reconnect_delay_seconds > 0 ) {
					timers.lvd_reconnect_delay_seconds--;
				} else {
					bit_clear(current.power_off_flags,POWER_FLAG_POS_LVD);
				}
			} else {
				timers.lvd_reconnect_delay_seconds=5; /* 5 seconds countdown before reconnecting */
			}

			if ( adc < config.lvd_disconnect_adc ) {
				if ( timers.lvd_disconnect_delay_seconds > 0 ) {
					timers.lvd_disconnect_delay_seconds--;
				} else {
					bit_set(current.power_off_flags,POWER_FLAG_POS_LVD);
				}
			} else {
				timers.lvd_disconnect_delay_seconds=config.lvd_disconnect_delay;
			}
		}

		/* HVD. 65535 disables */
		if ( 65535 != config.hvd_disconnect_delay ) {
			adc=adc_get(0);

			if ( adc < config.hvd_reconnect_adc ) {
				if ( timers.hvd_reconnect_delay_seconds > 0 ) {
					timers.hvd_reconnect_delay_seconds--;
				} else {
					bit_clear(current.power_off_flags,POWER_FLAG_POS_HVD);
				}
			} else {
				timers.hvd_reconnect_delay_seconds=5; /* 5 seconds countdown before reconnecting */
			}

			if ( adc > config.hvd_disconnect_adc ) {
				if ( timers.hvd_disconnect_delay_seconds > 0 ) {
					timers.hvd_disconnect_delay_seconds--;
				} else {
					bit_set(current.power_off_flags,POWER_FLAG_POS_HVD);
				}
			} else {
				timers.hvd_disconnect_delay_seconds=config.hvd_disconnect_delay;
			}
		}

		/* TODO: implement Low Temperature Disconnect (LTD) and High Temperature Disconnect (HTD) */

		/* TODO: implement Command On */


#if 0
		/* actually control the power switches */
		if ( current.power_off_flags ) {
			output_low(PI_POWER_EN);
			output_low(WIFI_POWER_EN);
		} else {
			output_high(PI_POWER_EN);
			output_high(WIFI_POWER_EN);
		}
#endif


		
		/* uptime counter */
		uptimeTicks++;
		if ( 60 == uptimeTicks ) {
			uptimeTicks=0;
			if ( current.uptime_minutes < 65535 ) 
				current.uptime_minutes++;
		}
	}

	/* ADC sample counter */
	if ( timers.now_adc_reset_count ) {
		timers.now_adc_reset_count=0;
		adcTicks=0;
	}

	/* ADC sampling trigger */
	adcTicks++;
	if ( adcTicks == config.adc_sample_ticks ) {
		adcTicks=0;
		timers.now_adc_sample=1;
	}

}


void main(void) {
	int8 i;
	int8 last_a, last_b;

	init();


	/* read parameters from EEPROM and write defaults if CRC doesn't match */
	read_param_file();

	if ( config.startup_power_on_delay > 100 )
		config.startup_power_on_delay=100;

	/* flash on startup */
	for ( i=0 ; i<config.startup_power_on_delay ; i++ ) {
		restart_wdt();
		output_high(LED_A);
		delay_ms(200);
		output_low(LED_A);
		delay_ms(200);
	}

	delay_ms(1000);

	fprintf(STREAM_FTDI,"# dcswc_module_latching_contactor %s\r\n",__DATE__);

	timers.led_on_a=500;

	enable_interrupts(GLOBAL);

	/* Prime ADC filter */
	for ( i=0 ; i<30 ; i++ ) {
		adc_update();
	}

	last_a = ! input(SW_OVERRIDE_A);
	last_b = ! input(SW_OVERRIDE_B);


	/* enable I2C slave interrupt */
	enable_interrupts(INT_SSP);

	for ( ; ; ) {
		restart_wdt();

		if ( timers.now_millisecond ) {
			periodic_millisecond();
		}

		if ( kbhit() ) {
			getc();
			fprintf(STREAM_FTDI,"# read_dip_switch()=%u\r\n",read_dip_switch());
			fprintf(STREAM_FTDI,"#    vin adc=%lu\r\n",adc_get(0));
			fprintf(STREAM_FTDI,"#   temp adc=%lu\r\n",adc_get(1));
			fprintf(STREAM_FTDI,"# dip sw adc=%lu\r\n",adc_get(2));
		}

		if ( input(SW_OVERRIDE_A) != last_a ) {
			last_a=input(SW_OVERRIDE_A);

			if ( last_a ) {
				timers.led_on_a=500;
				contactor_on_a();
			} else {
				timers.led_on_a=0;
				contactor_off_a();
			}
		}


		if ( input(SW_OVERRIDE_B) != last_b ) {
			last_B=input(SW_OVERRIDE_B);

			if ( last_b ) {
				timers.led_on_b=500;
				contactor_on_b();
			} else {
				timers.led_on_b=0;
				contactor_off_b();
			}
		}



		if ( timers.now_adc_sample ) {
			timers.now_adc_sample=0;
			adc_update();
		}

		if ( timers.now_write_config ) {
			timers.now_write_config=0;
			write_param_file();
		}
		if ( timers.now_reset_config ) {
			timers.now_reset_config=0;
			write_default_param_file();
		}


	}


}
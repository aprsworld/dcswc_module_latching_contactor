#include "dcswc_module_latching_contactor.h"

typedef struct {
	/* command on hold time */
	int16 command_on_hold_time;

	/* command off hold time */
	int16 command_off_hold_time;

	/* low voltage disconnect */
	int16 lvd_disconnect_adc;
	int16 lvd_disconnect_delay;
	int16 lvd_reconnect_adc;
	int16 lvd_reconnect_delay;

	/* high voltage disconnect */
	int16 hvd_disconnect_adc;
	int16 hvd_disconnect_delay;
	int16 hvd_reconnect_adc;
	int16 hvd_reconnect_delay;

	/* low temperature disconnect */
	int16 ltd_disconnect_adc;
	int16 ltd_disconnect_delay;
	int16 ltd_reconnect_adc;
	int16 ltd_reconnect_delay;

	/* high temperature disconnect */
	int16 htd_disconnect_adc;
	int16 htd_disconnect_delay;
	int16 htd_reconnect_adc;
	int16 htd_reconnect_delay;
} struct_config_channel;

typedef struct {
	/* not much going on here */
	struct_config_channel ch[2];
} struct_config;


typedef struct {
	int8 state;

	int16 command_on_seconds;			/* counts down. Off at zero. */
	int16 command_on_hold_seconds;      /* counts down. Off at zero. */

	int16 command_off_seconds;			/* counts down. Off at zero. */
	int16 command_off_hold_seconds;     /* counts down. Off at zero. */

	int16 lvd_disconnect_delay_seconds;	/* counts down */
	int16  lvd_reconnect_delay_seconds;	/* counts down */

	int16 hvd_disconnect_delay_seconds;	/* counts down */
	int16  hvd_reconnect_delay_seconds;	/* counts down */

	int16 ltd_disconnect_delay_seconds; /* counts down */
	int16 ltd_reconnect_delay_seconds;  /* counts down */

	int16 htd_disconnect_delay_seconds; /* counts down */
	int16 htd_reconnect_delay_seconds;  /* counts down */
} struct_channel;

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


	int8 default_params_written;
} struct_current;

typedef struct {
	/* action flags */
	int1 now_adc_sample;
	int1 now_adc_reset_count;

	int1 now_millisecond;

	int1 now_write_config;
	int1 now_reset_config;

	int1 now_debug_dump;


	/* timers */
	int8 led_blink[2];
	int8 led_on[2];
	int8 led_off[2];

	/* contactors */
	int8 contactor[2]; 	         /* current state */
	int8 contactor_powersave[2]; /* countdown to shut off power to coil at 0 */
} struct_time_keep;

/* global structures */
struct_config config={0};
struct_current current={0};
struct_time_keep timers={0};
struct_channel channel[2]={0};

#include "adc_dcswc_module_latching_contactor.c"
#include "param_dcswc_module_latching_contactor.c"
#include "i2c_handler_dcswc_module_latching_contactor.c"
#include "interrupt_dcswc_module_latching_contactor.c"
#include "debug_dcswc_module_latching_contactor.c"


void contactor_on(int8 c) {
	/* only turn on contactor if it isn't on or needs a refresh */
	if ( 1 == timers.contactor[c] ) {
		return;
	}

	/* we start the powersave count down */
	timers.contactor_powersave[c]=CONTACTOR_POWER_SAVE_MS;

	/* control the actual coil depending on what channel we are on */
	if ( 0==c ) {
		/* contactor A pins */
		output_low(BRIDGE_A_A);
		output_high(BRIDGE_A_B);
	} else {
		/* contactor B pins */
		output_low(BRIDGE_B_A);
		output_high(BRIDGE_B_B);
	}

	/* save state for next time */
	timers.contactor[c]=1;
}

void contactor_off(int8 c) {
	/* only turn off contactor if it isn't on or needs a refresh */
	if ( 0 == timers.contactor[c] ) {
		return;
	}

	/* we start the powersave count down */
	timers.contactor_powersave[c]=CONTACTOR_POWER_SAVE_MS;

	/* control the actual coil depending on what channel we are on */
	if ( 0==c ) {
		output_high(BRIDGE_A_A);
		output_low(BRIDGE_A_B);
	} else {
		output_high(BRIDGE_B_A);
		output_low(BRIDGE_B_B);
	}

	/* save state for next time */
	timers.contactor[c]=0;
}

void contactor_set(int8 c) {
	int8 state=1;

	/* if nothing is set in channel[c].state, contactor is on */
	state=1; 

	if ( channel[c].state & CH_STATE_MASK_ON ) {
		/* if override button (switch) is set or we are commanded on, then we will be on */
		state=1;
	} else if ( channel[c].state & CH_STATE_MASK_OFF ) {
		/* if one of the disconnect bits is set, we will be off */
		state=0;
	}

	if ( 0==state ) {
		contactor_off(c);
	} else {
		contactor_on(c);
	}

}

void contactor_logic(int8 c) {
	int16 adc;

	/* override button / switch */
	if ( (0==c && 0==input(SW_OVERRIDE_A)) || (1==c && 0==input(SW_OVERRIDE_B)) ) {
		bit_set(channel[c].state,CH_STATE_BIT_OVERRIDE);
	} else {
		bit_clear(channel[c].state,CH_STATE_BIT_OVERRIDE);
	}

	/* command on. 65535 disables */
	if ( 65535 != channel[c].command_on_seconds ) {
		if ( channel[c].command_on_seconds > 0 ) {
			/* waiting to power on */
			channel[c].command_on_seconds--;
		} else {
			/* timer at zero, ready to power on or already powered on */
			if ( ! bit_test(channel[c].state,CH_STATE_BIT_CMD_ON) ) {
				/* not currently set, so we set it and start the countdown */
				bit_set(channel[c].state,CH_STATE_BIT_CMD_ON);
				channel[c].command_on_hold_seconds=config.ch[c].command_on_hold_time;
			} else {
				/* set, so we clear it once countdown has elapsed */
				if ( 0==channel[c].command_on_hold_seconds ) {
					/* countdown elapsed, clear the flag and reset the timer */
					bit_clear(channel[c].state,CH_STATE_BIT_CMD_ON);
					channel[c].command_on_seconds=65535;
				} else {
					channel[c].command_on_hold_seconds--;
				}
			}		
		}
	}


	/* command off. 65535 disables */
	if ( 65535 != channel[c].command_off_seconds ) {
		if ( channel[c].command_off_seconds > 0 ) {
			/* waiting to power off */
			channel[c].command_off_seconds--;
		} else {
			/* timer at zero, ready to power off or already powered off */
			if ( ! bit_test(channel[c].state,CH_STATE_BIT_CMD_OFF) ) {
				/* not currently set, so we set it and start the countdown */
				bit_set(channel[c].state,CH_STATE_BIT_CMD_OFF);
				channel[c].command_off_hold_seconds=config.ch[c].command_off_hold_time;
			} else {
				/* set, so we clear it once countdown has elapsed */
				if ( 0==channel[c].command_off_hold_seconds ) {
					/* countdown elapsed, clear the flag and reset the timer */
					bit_clear(channel[c].state,CH_STATE_BIT_CMD_OFF);
					channel[c].command_off_seconds=65535;
				} else {
					channel[c].command_off_hold_seconds--;
				}
			}		
		}
	}


	/* LVD. 65535 disables */
	if ( 65535 != config.ch[c].lvd_disconnect_delay ) {
		adc=adc_get(0);

		if ( adc > config.ch[c].lvd_reconnect_adc ) {
			if ( channel[c].lvd_reconnect_delay_seconds > 0 ) {
				channel[c].lvd_reconnect_delay_seconds--;
			} else {
				bit_clear(channel[c].state,CH_STATE_BIT_LVD);
			}
		} else {
			channel[c].lvd_reconnect_delay_seconds=config.ch[c].lvd_reconnect_delay;
		}

		if ( adc < config.ch[c].lvd_disconnect_adc ) {
			if ( channel[c].lvd_disconnect_delay_seconds > 0 ) {
				channel[c].lvd_disconnect_delay_seconds--;
			} else {
				bit_set(channel[c].state,CH_STATE_BIT_LVD);
			}
		} else {
			channel[c].lvd_disconnect_delay_seconds=config.ch[c].lvd_disconnect_delay;
		}
	}

	/* HVD. 65535 disables */
	if ( 65535 != config.ch[c].hvd_disconnect_delay ) {
		adc=adc_get(0);

		if ( adc < config.ch[c].hvd_reconnect_adc ) {
			if ( channel[c].hvd_reconnect_delay_seconds > 0 ) {
				channel[c].hvd_reconnect_delay_seconds--;
			} else {
				bit_clear(channel[c].state,CH_STATE_BIT_HVD);
			}
		} else {
			channel[c].hvd_reconnect_delay_seconds=config.ch[c].hvd_reconnect_delay;
		}

		if ( adc > config.ch[c].hvd_disconnect_adc ) {
			if ( channel[c].hvd_disconnect_delay_seconds > 0 ) {
				channel[c].hvd_disconnect_delay_seconds--;
			} else {
				bit_set(channel[c].state,CH_STATE_BIT_HVD);
			}
		} else {
			channel[c].hvd_disconnect_delay_seconds=config.ch[c].hvd_disconnect_delay;
		}
	}

	/* LTD. 65535 disables */
	if ( 65535 != config.ch[c].ltd_disconnect_delay ) {
		adc=adc_get(1);

		if ( adc < config.ch[c].ltd_reconnect_adc ) {
			if ( channel[c].ltd_reconnect_delay_seconds > 0 ) {
				channel[c].ltd_reconnect_delay_seconds--;
			} else {
				bit_clear(channel[c].state,CH_STATE_BIT_LTD);
			}
		} else {
			channel[c].ltd_reconnect_delay_seconds=config.ch[c].ltd_reconnect_delay;
		}

		if ( adc > config.ch[c].ltd_disconnect_adc ) {
			if ( channel[c].ltd_disconnect_delay_seconds > 0 ) {
				channel[c].ltd_disconnect_delay_seconds--;
			} else {
				bit_set(channel[c].state,CH_STATE_BIT_LTD);
			}
		} else {
			channel[c].ltd_disconnect_delay_seconds=config.ch[c].ltd_disconnect_delay;
		}
	}


	/* TODO: implement High Temperature Disconnect (HTD) */
}


void led_status_second_update(void) {
	static int1 second=0;
	static int8 cycle=0;
	int8 led;

	/* update every other second */
	if ( second ) {
		second=0;
		return;
	}

	for ( led=0 ; led<2 ; led++ ) {
		if ( cycle < 8 ) {
			/* first 8 cycles are 8 bits of state bit field */

			/* if bit is set, then we blink the number of times+1 of the bit we are in. So bit 1 set means we blink 2 times */
			if ( bit_test(channel[led].state,cycle) ) {
				timers.led_blink[led]=cycle+1;
			} else {
				timers.led_blink[led]=0;
			}
		} else if ( 8 == cycle ) {
			/* solid light to mark start of next cycle */
			timers.led_blink[led]=255;
		} else {
			/* solid on if contactor state is on, otherwise off */
			if ( timers.contactor[led] ) {
				timers.led_blink[led]=255;
			} else {
				timers.led_blink[led]=0;
			}
		}
	}

	fprintf(STREAM_FTDI,"# LED %u A=%03u (0x%02X) B=%03u (0x%02X)\r\n",cycle,timers.led_blink[0],channel[0].state,timers.led_blink[1],channel[1].state);

	if ( 9 == cycle ) {
		cycle=0; 
	} else {
		cycle++;
	}

	second=1;
}

void led_on(int8 c) {
	if ( 0==c ) 
		output_high(LED_A);
	else
		output_high(LED_B);
}

void led_off(int8 c) {
	if ( 0==c ) 
		output_low(LED_A);
	else
		output_low(LED_B);
}

void periodic_millisecond(void) {
	static int8 uptimeticks=0;
	static int16 adcTicks=0;
	static int16 ticks=0;

	static int8 led[2]={0};
	static int8 led_state[2]={0};
	int8 i;


	timers.now_millisecond=0;

	/* set LED output */
	for ( i=0 ; i<2 ; i++ ) {
		if ( 255 == timers.led_blink[i] ) {
			led_on(i);
		} else if ( 0 == timers.led_blink[i] ) {
			led_off(i);
		} else {
			/* should be blinking */
			if ( 1 == led_state[i] ) {
				if ( 0 == led[i] ) {
					/* have been on, now we are going to rest */
					led_state[i]=0;
					led[i]=BLINK_OFF_TIME;
					led_off(i);

					if ( timers.led_blink[i] > 0 ) {
						timers.led_blink[i]--;
					}
				} else {
					led[i]--;
					led_on(i);
				}
			} else {
				if ( 0 == led[i] ) {
					/* have been off, now we are going to turn on */
					led_state[i]=1;
					led[i]=BLINK_ON_TIME;
					led_on(i);
				} else {
					led[i]--;
					led_off(i);
				}
			}
		}
	}


	/* some other random stuff that we don't need to do every cycle in main */
	if ( current.interval_milliseconds < 65535 ) {
		current.interval_milliseconds++;
	}


	/* contactor timeout */
	if ( 0 == timers.contactor_powersave[0] ) {
		output_low(BRIDGE_A_A);
		output_low(BRIDGE_A_B);
	} else {
		timers.contactor_powersave[0]--;
	}
	if ( 0 == timers.contactor_powersave[1] ) {
		output_low(BRIDGE_B_A);
		output_low(BRIDGE_B_B);
	} else {
		timers.contactor_powersave[1]--;
	}




	/* seconds */
	ticks++;
	if ( 1000 == ticks ) {
		ticks=0;

		/* set channe[n].state based on logic */
		contactor_logic(0);
		contactor_logic(1);

		/* set contactor outputs based on their state bit field */
		contactor_set(0);
		contactor_set(1);		

		/* uptime counter */
		uptimeTicks++;
		if ( 60 == uptimeTicks ) {
			uptimeTicks=0;
			if ( current.uptime_minutes < 65535 ) 
				current.uptime_minutes++;
		}

		/* LED state display update */
		led_status_second_update();
	}

	/* ADC sample counter */
	if ( timers.now_adc_reset_count ) {
		timers.now_adc_reset_count=0;
		adcTicks=0;
	}

	/* ADC sampling trigger */
	adcTicks++;
	if ( ADC_SAMPLE_TICKS == adcTicks ) {
		adcTicks=0;
		timers.now_adc_sample=1;
	}

}

void init(void) {
	int8 i;
	int8 buff[10];
	setup_oscillator(OSC_16MHZ);

	setup_adc(ADC_CLOCK_DIV_16);
	/* NTC thermistor on sAN2, input voltage divider on sAN4, DIP switch analog on sAN9
	voltage spans between 0 and Vdd */
	setup_adc_ports(sAN2 | sAN4 | sAN9,VSS_VDD);

	setup_wdt(WDT_512MS);

	set_tris_a(0b00111111);
	set_tris_b(0b01110000);
	set_tris_c(0b10000001);
//               76543210

	port_a_pullups(0b00110000);
	port_b_pullups(0b00000000);
//                   76543210

	/* data structure initialization */
	/* all initialized to 0 on declaration. Just do this if need non-zero */
	for ( i=0 ; i<=1 ; i++ ) {
		channel[i].command_off_seconds=65535;
		channel[i].command_on_seconds =65535;
	}

	/* get our compiled date from constant  
	'5-Feb-22'
	 01234567

	'25-Feb-22'
	 012345678
	*/
	strcpy(buff,__DATE__);
	i=0;
	if ( '-' != buff[1] ) {
		/* day can be one or two digits */
		current.compile_day =(buff[i]-'0')*10;
		i++;
	}
	current.compile_day+=(buff[i]-'0');
	i+=2; /* now points to month */

	/* determine month ... how annoying */
	if ( 'J'==buff[i+0] ) {
		if ( 'A'==buff[i+1] )
			current.compile_month=1;
		else if ( 'N'==buff[i+2] )
			current.compile_month=6;
		else
			current.compile_month=7;
	} else if ( 'A'==buff[i+0] ) {
		if ( 'P'==buff[i+1] )
			current.compile_month=4;
		else
			current.compile_month=8;
	} else if ( 'M'==buff[i+0] ) {
		if ( 'R'==buff[i+2] )
			current.compile_month=3;
		else
			current.compile_month=5;
	} else if ( 'F'==buff[i+0] ) {
		current.compile_month=2;
	} else if ( 'S'==buff[i+0] ) {
		current.compile_month=9;
	} else if ( 'O'==buff[i+0] ) {
		current.compile_month=10;
	} else if ( 'N'==buff[i+0] ) {
		current.compile_month=11;
	} else if ( 'D'==buff[i+0] ) {
		current.compile_month=12;
	} else {
		/* error parsing, shouldn't happen */
		current.compile_month=255;
	}
	current.compile_year =(buff[i+4]-'0')*10;
	current.compile_year+=(buff[i+5]-'0');


	/* one periodic interrupt @ 1mS. Generated from system 16 MHz clock */
	/* prescale=16, match=249, postscale=1. Match is 249 because when match occurs, one cycle is lost */
	setup_timer_2(T2_DIV_BY_16,249,1);

	enable_interrupts(INT_TIMER2);

	/* set I2C slave address, which is always an even number */
	i2c_slaveaddr(STREAM_SLAVE,0x3e + (read_dip_switch()<<1) );
}


void main(void) {
	int8 i;

	init();


	/* read parameters from EEPROM and write defaults if CRC doesn't match */
	read_param_file();


	/* flash on startup */
	for ( i=0 ; i<5 ; i++ ) {
		restart_wdt();
		output_high(LED_A);
		delay_ms(200);
		output_low(LED_A);
		delay_ms(200);
	}

	fprintf(STREAM_FTDI,"# dcswc_module_latching_contactor %s\r\n# ",__DATE__);
	switch ( restart_cause ) {
		case WDT_TIMEOUT:       fprintf(STREAM_FTDI,"WDT TIMEOUT"); break;
		case MCLR_FROM_SLEEP:   fprintf(STREAM_FTDI,"MCLR FROM SLEEP"); break;
		case MCLR_FROM_RUN:     fprintf(STREAM_FTDI,"MCLR FROM RUN"); break;
		case NORMAL_POWER_UP:   fprintf(STREAM_FTDI,"NORMAL POWER UP"); break;
		case BROWNOUT_RESTART:  fprintf(STREAM_FTDI,"BROWNOUT RESTART"); break;
		case WDT_FROM_SLEEP:    fprintf(STREAM_FTDI,"WDT FROM SLEEP"); break;
		case RESET_INSTRUCTION: fprintf(STREAM_FTDI,"RESET INSTRUCTION"); break;
		default:                fprintf(STREAM_FTDI,"UNKNOWN!");
	}
	fprintf(STREAM_FTDI,"\r\n");

	enable_interrupts(GLOBAL);

	/* Prime ADC filter */
	for ( i=0 ; i<30 ; i++ ) {
		adc_update();
	}


	/* enable I2C slave interrupt */
	enable_interrupts(INT_SSP);

	for ( ; ; ) {
		restart_wdt();

		if ( timers.now_millisecond ) {
			periodic_millisecond();
		}

		if ( kbhit() ) {
			getc();
			timers.now_debug_dump=1;
		}

		if ( timers.now_debug_dump ) {
			timers.now_debug_dump=0;

//			debug_dump();
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
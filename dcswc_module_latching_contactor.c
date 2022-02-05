#include "dcswc_module_latching_contactor.h"

typedef struct {
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
	int16 adc_sample_ticks;

	int16 startup_power_on_delay;
	int8 reconnect_delay;

	int16 command_off_hold_time;
	int16 command_on_hold_time;

	struct_config_channel ch[2];
} struct_config;


typedef struct {
	int8 state;

	int16 command_on_seconds;			/* counts down. Off at zero. */
	int16 command_on_hold_seconds;      /* counts down. Off at zero. */

	int16 command_off_seconds;			/* counts down. Off at zero. */
	int16 command_off_hold_seconds;     /* counts down. Off at zero. */

	int16 lvd_disconnect_delay_seconds;	/* counts down */
	int8  lvd_reconnect_delay_seconds;	/* counts down */

	int16 hvd_disconnect_delay_seconds;	/* counts down */
	int8  hvd_reconnect_delay_seconds;	/* counts down */

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

	/* contactor states */
	int1 contactor_a;
	int1 contactor_b;

	/* timers */
	int8 led_on_a;
	int8 led_on_b;

	int8  contactor_a_powersave;        /* counts down. Off at zero. */
	int8  contactor_b_powersave;        /* counts down. Off at zero. */
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

	if ( adc > (1023-64) )
		return 0;
	if ( adc > (682-64) )
		return 2;
	if ( adc > (511-64) )
		return 1;

	return 3;
}

void contactor_on_a(void) {
	/* only turn on contactor if it isn't on or needs a refresh */
	if ( 1 == timers.contactor_a )
		return;

	timers.contactor_a_powersave=CONTACTOR_POWER_SAVE_MS;

	output_high(BRIDGE_A_A);
	output_low(BRIDGE_A_B);

	timers.contactor_a=1;
}

void contactor_off_a(void) {
	/* only turn off contactor if it isn't on or needs a refresh */
	if ( 0 == timers.contactor_a )
		return;

	timers.contactor_a_powersave=CONTACTOR_POWER_SAVE_MS;

	output_low(BRIDGE_A_A);
	output_high(BRIDGE_A_B);

	timers.contactor_a=0;
}

void contactor_on_b(void) {
	/* only turn on contactor if it isn't on or needs a refresh */
	if ( 1 == timers.contactor_b )
		return;

	timers.contactor_b_powersave=CONTACTOR_POWER_SAVE_MS;

	output_high(BRIDGE_B_A);
	output_low(BRIDGE_B_B);

	timers.contactor_b=1;
}

void contactor_off_b(void) {
	/* only turn off contactor if it isn't on or needs a refresh */
	if ( 0 == timers.contactor_b )
		return;

	timers.contactor_b_powersave=CONTACTOR_POWER_SAVE_MS;

	output_low(BRIDGE_B_A);
	output_high(BRIDGE_B_B);

	timers.contactor_b=0;
}

void contactor_logic(int8 c) {
	int16 adc;

	/* TODO: implement Command On.  */

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
				channel[c].command_off_hold_seconds=config.command_off_hold_time;
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
			channel[c].lvd_reconnect_delay_seconds=config.reconnect_delay; /* 5 seconds countdown before reconnecting */
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
			channel[c].hvd_reconnect_delay_seconds=config.reconnect_delay; /* 5 seconds countdown before reconnecting */
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


	/* TODO: implement Low Temperature Disconnect (LTD) and High Temperature Disconnect (HTD) */
}

void contactor_set() {


}


void periodic_millisecond(void) {
	static int8 uptimeticks=0;
	static int16 adcTicks=0;
	static int16 ticks=0;


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

		/* set channe[n].state based on logic */
		contactor_logic(0);
		contactor_logic(1);

		/* set contactor outputs */
		contactor_set();
		
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

void init(void) {
	int8 buff[32];
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
	channel[0].command_off_seconds=65535;
	channel[1].command_off_seconds=65535;
	channel[0].command_on_seconds =65535;
	channel[1].command_on_seconds =65535;


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

	/* set I2C slave address, which is always an even number */
	i2c_slaveaddr(0x3e + (read_dip_switch()<<1) );
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

			for ( i=0 ; i<2 ; i++ ) {
				restart_wdt();
				fprintf(STREAM_FTDI,"# channel[%u]\r\n",i);
				fprintf(STREAM_FTDI,"#                        state=0x%02x\r\n",channel[i].state);
				fprintf(STREAM_FTDI,"#           command_on_seconds=%lu\r\n",channel[i].command_on_seconds);
				fprintf(STREAM_FTDI,"#      command_on_hold_seconds=%lu\r\n",channel[i].command_on_hold_seconds);

				fprintf(STREAM_FTDI,"#          command_off_seconds=%lu\r\n",channel[i].command_off_seconds);
				fprintf(STREAM_FTDI,"#     command_off_hold_seconds=%lu\r\n",channel[i].command_off_hold_seconds);

				fprintf(STREAM_FTDI,"# lvd_disconnect_delay_seconds=%lu\r\n",channel[i].lvd_disconnect_delay_seconds);
				fprintf(STREAM_FTDI,"#  lvd_reconnect_delay_seconds=%u\r\n",channel[i].lvd_reconnect_delay_seconds);

				fprintf(STREAM_FTDI,"# hvd_disconnect_delay_seconds=%lu\r\n",channel[i].hvd_disconnect_delay_seconds);
				fprintf(STREAM_FTDI,"#  hvd_reconnect_delay_seconds=%u\r\n",channel[i].hvd_reconnect_delay_seconds);
			}

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
#inline
char xor_crc(char oldcrc, char data) {
	return oldcrc ^ data;
}

char EEPROMDataRead( int16 address, int8 *data, int16 count ) {
	char crc=0;

	while ( count-- != 0 ) {
		*data = read_eeprom( address++ );
		crc = xor_crc(crc,*data);
		data++;
	}
	return crc;
}

char EEPROMDataWrite( int16 address, int8 *data, int16 count ) {
	char crc=0;

	while ( count-- != 0 ) {
		/* restart_wdt() */
		crc = xor_crc(crc,*data);
		write_eeprom( address++, *data++ );
	}

	return crc;
}

void write_param_file() {
	int8 crc;

	/* write the config structure */
	crc = EEPROMDataWrite(PARAM_ADDRESS,(void *)&config,sizeof(config));
	/* write the CRC was calculated on the structure */
	write_eeprom(PARAM_CRC_ADDRESS,crc);
}

void write_default_param_file() {
	int8 i;

	current.default_params_written=1;

	/* both LEDs on */
	timers.led_blink[0]=255;
	timers.led_blink[1]=255;


	for ( i=0 ; i<2 ; i++ ) {
		/* low voltage disconnect */
		config.ch[i].lvd_disconnect_adc=606;
		config.ch[i].lvd_disconnect_delay=20; /* 65535 disables LVD */
		config.ch[i].lvd_reconnect_adc=616;
		config.ch[i].lvd_reconnect_delay=10;
		
		/* high voltage disconnect */
		config.ch[i].hvd_disconnect_adc=700; 
		config.ch[i].hvd_disconnect_delay=65535; /* 65535 disables HVD */
		config.ch[i].hvd_reconnect_adc=650;
		config.ch[i].hvd_reconnect_delay=60;

		/* low temperature disconnect */
		config.ch[i].ltd_disconnect_adc=786; // 0C / 32F
		config.ch[i].ltd_disconnect_delay=65535; /* 65535 disables LTD */
		config.ch[i].ltd_reconnect_adc=683;  // 10C / 50F
		config.ch[i].ltd_reconnect_delay=4;

		/* high temperature disconnect */
		config.ch[i].htd_disconnect_adc=405; // 35C / 95F
		config.ch[i].htd_disconnect_delay=65535; /* 65535 disables LTD */
		config.ch[i].htd_reconnect_adc=512;  // 25C / 77F
		config.ch[i].htd_reconnect_delay=4;
	}
	

	/* set the two channels differently */
	/* LVD 1 bus (modem, etc) @ Fairway */
	config.ch[0].lvd_disconnect_adc=602; /* 23.5 */
	config.ch[0].lvd_reconnect_adc=627;  /* 24.5 */
 	/* LVD 2 bus (CODAR) @ Fairway */
	config.ch[1].lvd_disconnect_adc=614; /* 24.0 */
	config.ch[1].lvd_reconnect_adc=640;  /* 25.0 */		

	/* write them so next time we use from EEPROM */
	write_param_file();

}


void read_param_file() {
	int8 crc;

	crc = EEPROMDataRead(PARAM_ADDRESS, (void *)&config, sizeof(config)); 
		
	if ( crc != read_eeprom(PARAM_CRC_ADDRESS) ) {
		write_default_param_file();
	}
}



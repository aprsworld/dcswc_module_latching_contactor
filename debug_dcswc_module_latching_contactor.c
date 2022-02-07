void debug_dump(void) {

//	int8 i;

	
	restart_wdt();
#if 0
	fprintf(STREAM_FTDI,"# '%s'\r\n",__DATE__);
	fprintf(STREAM_FTDI,"#  compile_year=%u\r\n",current.compile_year);
	fprintf(STREAM_FTDI,"# compile_month=%u\r\n",current.compile_month);
	fprintf(STREAM_FTDI,"#   compile_day=%u\r\n",current.compile_day);
#endif
	fprintf(STREAM_FTDI,"#       vin adc=%lu\r\n",adc_get(0));
	fprintf(STREAM_FTDI,"#      temp adc=%lu\r\n",adc_get(1));
	fprintf(STREAM_FTDI,"#   ch[0].state=0x%2X\r\n",channel[0].state);
	fprintf(STREAM_FTDI,"#   ch[1].state=0x%2X\r\n",channel[1].state);
//	fprintf(STREAM_FTDI,"# read_dip_sw()=%u\r\n",read_dip_switch());
//	fprintf(STREAM_FTDI,"#    dip sw adc=%lu\r\n",adc_get(2));

#if 0
	for ( i=0 ; i<2 ; i++ ) {
		restart_wdt();
		fprintf(STREAM_FTDI,"#-------\r\n");
		fprintf(STREAM_FTDI,"# config.ch[%u]\r\n",i);
		fprintf(STREAM_FTDI,"# command_off_hold_time=%lu\r\n",config.ch[i].command_off_hold_time);

		fprintf(STREAM_FTDI,"#    lvd_disconnect_adc=%lu\r\n",config.ch[i].lvd_disconnect_adc);
		fprintf(STREAM_FTDI,"#  lvd_disconnect_delay=%lu\r\n",config.ch[i].lvd_disconnect_delay);
		fprintf(STREAM_FTDI,"#     lvd_reconnect_adc=%lu\r\n",config.ch[i].lvd_reconnect_adc);
		fprintf(STREAM_FTDI,"#   lvd_reconnect_delay=%lu\r\n",config.ch[i].lvd_reconnect_delay);

		fprintf(STREAM_FTDI,"#    hvd_disconnect_adc=%lu\r\n",config.ch[i].hvd_disconnect_adc);
		fprintf(STREAM_FTDI,"#  hvd_disconnect_delay=%lu\r\n",config.ch[i].hvd_disconnect_delay);
		fprintf(STREAM_FTDI,"#     hvd_reconnect_adc=%lu\r\n",config.ch[i].hvd_reconnect_adc);
		fprintf(STREAM_FTDI,"#   hvd_reconnect_delay=%lu\r\n",config.ch[i].hvd_reconnect_delay);

		restart_wdt();
		fprintf(STREAM_FTDI,"#    ltd_disconnect_adc=%lu\r\n",config.ch[i].ltd_disconnect_adc);
		fprintf(STREAM_FTDI,"#  ltd_disconnect_delay=%lu\r\n",config.ch[i].ltd_disconnect_delay);
		fprintf(STREAM_FTDI,"#     ltd_reconnect_adc=%lu\r\n",config.ch[i].ltd_reconnect_adc);
		fprintf(STREAM_FTDI,"#   ltd_reconnect_delay=%lu\r\n",config.ch[i].ltd_reconnect_delay);

		fprintf(STREAM_FTDI,"#    htd_disconnect_adc=%lu\r\n",config.ch[i].htd_disconnect_adc);
		fprintf(STREAM_FTDI,"#  htd_disconnect_delay=%lu\r\n",config.ch[i].htd_disconnect_delay);
		fprintf(STREAM_FTDI,"#     htd_reconnect_adc=%lu\r\n",config.ch[i].htd_reconnect_adc);
		fprintf(STREAM_FTDI,"#   htd_reconnect_delay=%lu\r\n",config.ch[i].htd_reconnect_delay);


		restart_wdt();
		fprintf(STREAM_FTDI,"#\r\n");
		fprintf(STREAM_FTDI,"# channel[%u]\r\n",i);
		fprintf(STREAM_FTDI,"#                        state=0x%02x\r\n",channel[i].state);
		fprintf(STREAM_FTDI,"#           command_on_seconds=%lu\r\n",channel[i].command_on_seconds);
		fprintf(STREAM_FTDI,"#      command_on_hold_seconds=%lu\r\n",channel[i].command_on_hold_seconds);

		fprintf(STREAM_FTDI,"#          command_off_seconds=%lu\r\n",channel[i].command_off_seconds);
		fprintf(STREAM_FTDI,"#     command_off_hold_seconds=%lu\r\n",channel[i].command_off_hold_seconds);

		fprintf(STREAM_FTDI,"# lvd_disconnect_delay_seconds=%lu\r\n",channel[i].lvd_disconnect_delay_seconds);
		fprintf(STREAM_FTDI,"#  lvd_reconnect_delay_seconds=%lu\r\n",channel[i].lvd_reconnect_delay_seconds);

		fprintf(STREAM_FTDI,"# hvd_disconnect_delay_seconds=%lu\r\n",channel[i].hvd_disconnect_delay_seconds);
		fprintf(STREAM_FTDI,"#  hvd_reconnect_delay_seconds=%lu\r\n",channel[i].hvd_reconnect_delay_seconds);

		restart_wdt();
		fprintf(STREAM_FTDI,"# ltd_disconnect_delay_seconds=%lu\r\n",channel[i].ltd_disconnect_delay_seconds);
		fprintf(STREAM_FTDI,"#  ltd_reconnect_delay_seconds=%lu\r\n",channel[i].ltd_reconnect_delay_seconds);

		fprintf(STREAM_FTDI,"# htd_disconnect_delay_seconds=%lu\r\n",channel[i].htd_disconnect_delay_seconds);
		fprintf(STREAM_FTDI,"#  htd_reconnect_delay_seconds=%lu\r\n",channel[i].htd_reconnect_delay_seconds);
	}
	#endif
}
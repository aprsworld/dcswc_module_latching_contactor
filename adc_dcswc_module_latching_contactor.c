int16 adc_get(int8 ch) {
	int16 sum;
	int8 i;

	/* pre-compute address of channel adc buffer. Saves computing it 16 times in the loop below */
	int16 *p;
	p = current.adc_buffer[ch];

	// Calculate the mean.  This is done by summing up the
	// values and dividing by the number of elements.
	sum = 0;
	for( i = 0; i < 16 ; i++ ) {
//		sum += current.adc_buffer[ch][i];
		sum += p[i];
	}

	/* divide sum by our 16 samples and round by adding 8 */
	return ( (sum+8) >> 4 );
}


void adc_update(void) {

	/* wrap buffer around */
	current.adc_buffer_index++;
	if ( current.adc_buffer_index >= 16 )
		current.adc_buffer_index=0;

	set_adc_channel(4);
	current.adc_buffer[0][current.adc_buffer_index] = read_adc();


	set_adc_channel(2);
	delay_ms(1);
	current.adc_buffer[1][current.adc_buffer_index] = read_adc();

	set_adc_channel(9);
	delay_ms(1);
	current.adc_buffer[2][current.adc_buffer_index] = read_adc();
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

	if ( adc > (1023-64) )
		return 0;
	if ( adc > (682-64) )
		return 2;
	if ( adc > (511-64) )
		return 1;

	return 3;
}
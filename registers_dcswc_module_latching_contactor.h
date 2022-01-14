
/* first three registers will give enough information for telemetry */
#define I2C_REG_VOLTAGE_INPUT_AVG             0  /* 10 bit input voltage ADC averaged value */
#define I2C_REG_STATE_CONTACTORS              1  /* two 8 bit channel states (A is MSB / B is LSB) */
#define I2C_REG_TEMPERATURE_BOARD_AVG         2  /* 10 bit NTC thermistor ADC averaged value */

#define I2C_REG_VOLTAGE_INPUT_NOW             3  /* 10 bit input voltage ADC latest value */
#define I2C_REG_TEMPERATURE_BOARD_NOW         4  /* 10 bit NTC thermistor ADC latest value */

/* command can only come from I2C interface */
#define I2C_REG_COMMAND_ON_A                  5  /* R/W seconds */
#define I2C_REG_COMMAND_ON_B                  6

#define I2C_REG_COMMAND_OFF_A                 7  /* R/W seconds */
#define I2C_REG_COMMAND_OFF_B                 8

/* low voltage disconnect */
#define I2C_REG_LVD_DISCONNECT_A              9  /* R seconds */
#define I2C_REG_LVD_DISCONNECT_B              10

/* high voltage disconnect */
#define I2C_REG_HVD_DISCONNECT_A              11 /* R seconds */
#define I2C_REG_HVD_DISCONNECT_B              12

/* low temperature disconnect */
#define I2C_REG_LTD_DISCONNECT_A              9  /* R seconds */
#define I2C_REG_LTD_DISCONNECT_B              10

/* high temperature disconnect */
#define I2C_REG_HTD_DISCONNECT_A              11 /* R seconds */
#define I2C_REG_HTD_DISCONNECT_B              12

/* reserved for future, unimplemented */
#define I2C_REG_FUT_DISCONNECT_A              13 /* R seconds */
#define I2C_REG_FUT_DISCONNECT_B              14


#define I2C_REG_SEQUENCE_NUMBER               8  /* R */
#define I2C_REG_TIME_INTERVAL_MILLISECONDS    9
#define I2C_REG_TIME_UPTIME_MINUTES           10
#define I2C_REG_DEFAULT_PARAMS_WRITTEN        13


/* configuration */
#define I2C_REG_CONFIG_HARDWARE_MODEL              128 /* R */
#define I2C_REG_CONFIG_HARDWARE_VERSION            129
#define I2C_REG_CONFIG_SOFTWARE_MODEL              130
#define I2C_REG_CONFIG_SOFTWARE_VERSION            131
#define I2C_REG_CONFIG_SOFTWARE_YEAR               132
#define I2C_REG_CONFIG_SOFTWARE_MONTH              133
#define I2C_REG_CONFIG_SOFTWARE_DAY                134

#define I2C_REG_CONFIG_PARAM_WRITE                 135 /* R / W */

#define I2C_REG_CONFIG_STARTUP_POWER_ON_DELAY      136
#define I2C_REG_CONFIG_RECONNECT_DELAY             137

#define I2C_REG_CONFIG_COMMAND_OFF_HOLD_TIME        44
#define I2C_REG_CONFIG_COMMAND_ON_HOLD_TIME         44


#define I2C_REG_CONFIG_LVD_DISCONNECT_VOLTAGE       49
#define I2C_REG_CONFIG_LVD_DISCONNECT_DELAY         50
#define I2C_REG_CONFIG_LVD_RECONNECT_VOLTAGE        51
#define I2C_REG_CONFIG_HVD_DISCONNECT_VOLTAGE       52
#define I2C_REG_CONFIG_HVD_DISCONNECT_DELAY         53
#define I2C_REG_CONFIG_HVD_RECONNECT_VOLTAGE        54

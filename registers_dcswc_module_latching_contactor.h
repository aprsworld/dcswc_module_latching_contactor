
/* first three registers will give enough information for telemetry */
#define I2C_REG_VOLTAGE_INPUT_AVG             0  /* 10 bit input voltage ADC averaged value */
#define I2C_REG_STATE_CONTACTORS              1  /* two 8 bit channel states (A is MSB / B is LSB) */
#define I2C_REG_TEMPERATURE_BOARD_AVG         2  /* 10 bit NTC thermistor ADC averaged value */

#define I2C_REG_VOLTAGE_INPUT_NOW             3  /* 10 bit input voltage ADC latest value */
#define I2C_REG_TEMPERATURE_BOARD_NOW         4  /* 10 bit NTC thermistor ADC latest value */


/* channel 0 / A */
/* command can only come from I2C interface */
#define I2C_REG_CH0_COMMAND_ON                5  /* R/W seconds */
#define I2C_REG_CH0_COMMAND_ON_HOLD           6

#define I2C_REG_CH0_COMMAND_OFF               7  /* R/W seconds */
#define I2C_REG_CH0_COMMAND_OFF_HOLD          8

/* low voltage disconnect */
#define I2C_REG_CH0_LVD_DISCONNECT            9  /* R seconds */
#define I2C_REG_CH0_LVD_RECONNECT             10 

/* high voltage disconnect */
#define I2C_REG_CH0_HVD_DISCONNECT            11 /* R seconds */
#define I2C_REG_CH0_HVD_RECONNECT             12

/* low temperature disconnect */
#define I2C_REG_CH0_LTD_DISCONNECT            13  /* R seconds */
#define I2C_REG_CH0_LTD_RECONNECT             14

/* high temperature disconnect */
#define I2C_REG_CH0_HTD_DISCONNECT            15 /* R seconds */
#define I2C_REG_CH0_HTD_RECONNECT             16

/* reserved for future, unimplemented */
#define I2C_REG_CH0_FUT_DISCONNECT            17 /* R seconds */
#define I2C_REG_CH0_FUT_RECONNECT             18

/* channel 1 / B */
/* command can only come from I2C interface */
#define I2C_REG_CH1_COMMAND_ON                19 /* R/W seconds */
#define I2C_REG_CH1_COMMAND_ON_HOLD           20

#define I2C_REG_CH1_COMMAND_OFF               21 /* R/W seconds */
#define I2C_REG_CH1_COMMAND_OFF_HOLD          22

/* low voltage disconnect */
#define I2C_REG_CH1_LVD_DISCONNECT            23 /* R seconds */
#define I2C_REG_CH1_LVD_RECONNECT             24 

/* high voltage disconnect */
#define I2C_REG_CH1_HVD_DISCONNECT            25 /* R seconds */
#define I2C_REG_CH1_HVD_RECONNECT             26

/* low temperature disconnect */
#define I2C_REG_CH1_LTD_DISCONNECT            27  /* R seconds */
#define I2C_REG_CH1_LTD_RECONNECT             28

/* high temperature disconnect */
#define I2C_REG_CH1_HTD_DISCONNECT            29 /* R seconds */
#define I2C_REG_CH1_HTD_RECONNECT             30

/* reserved for future, unimplemented */
#define I2C_REG_CH1_FUT_DISCONNECT            31 /* R seconds */
#define I2C_REG_CH1_FUT_RECONNECT             32

/* meta */
#define I2C_REG_SEQUENCE_NUMBER               33 /* R */
#define I2C_REG_TIME_INTERVAL_MILLISECONDS    34
#define I2C_REG_TIME_UPTIME_MINUTES           35
#define I2C_REG_DEFAULT_PARAMS_WRITTEN        36

/* channel configuration */
/* channel 0 / A */
#define I2C_REG_CH0_LVD_DISCONNECT_ADC        64
#define I2C_REG_CH0_LVD_DISCONNECT_DELAY      65
#define I2C_REG_CH0_LVD_RECONNECT_ADC         66
#define I2C_REG_CH0_LVD_RECONNECT_DELAY       67

#define I2C_REG_CH0_HVD_DISCONNECT_ADC        68
#define I2C_REG_CH0_HVD_DISCONNECT_DELAY      69
#define I2C_REG_CH0_HVD_RECONNECT_ADC         70
#define I2C_REG_CH0_HVD_RECONNECT_DELAY       71

#define I2C_REG_CH0_LTD_DISCONNECT_ADC        72
#define I2C_REG_CH0_LTD_DISCONNECT_DELAY      73
#define I2C_REG_CH0_LTD_RECONNECT_ADC         74
#define I2C_REG_CH0_LTD_RECONNECT_DELAY       75

#define I2C_REG_CH0_HTD_DISCONNECT_ADC        76
#define I2C_REG_CH0_HTD_DISCONNECT_DELAY      77
#define I2C_REG_CH0_HTD_RECONNECT_ADC         78
#define I2C_REG_CH0_HTD_RECONNECT_DELAY       79

#define I2C_REG_CH0_FUT_DISCONNECT_VALUE      80
#define I2C_REG_CH0_FUT_DISCONNECT_DELAY      81
#define I2C_REG_CH0_FUT_RECONNECT_VALUE       82
#define I2C_REG_CH0_FUT_RECONNECT_DELAY       83

/* channel 1 / B */
#define I2C_REG_CH1_LVD_DISCONNECT_ADC        84
#define I2C_REG_CH1_LVD_DISCONNECT_DELAY      85
#define I2C_REG_CH1_LVD_RECONNECT_ADC         86
#define I2C_REG_CH1_LVD_RECONNECT_DELAY       87

#define I2C_REG_CH1_HVD_DISCONNECT_ADC        88
#define I2C_REG_CH1_HVD_DISCONNECT_DELAY      89
#define I2C_REG_CH1_HVD_RECONNECT_ADC         90
#define I2C_REG_CH1_HVD_RECONNECT_DELAY       91

#define I2C_REG_CH1_LTD_DISCONNECT_ADC        92
#define I2C_REG_CH1_LTD_DISCONNECT_DELAY      93
#define I2C_REG_CH1_LTD_RECONNECT_ADC         94
#define I2C_REG_CH1_LTD_RECONNECT_DELAY       95

#define I2C_REG_CH1_HTD_DISCONNECT_ADC        96
#define I2C_REG_CH1_HTD_DISCONNECT_DELAY      97
#define I2C_REG_CH1_HTD_RECONNECT_ADC         98
#define I2C_REG_CH1_HTD_RECONNECT_DELAY       99

#define I2C_REG_CH1_FUT_DISCONNECT_VALUE      100
#define I2C_REG_CH1_FUT_DISCONNECT_DELAY      101
#define I2C_REG_CH1_FUT_RECONNECT_VALUE       102
#define I2C_REG_CH1_FUT_RECONNECT_DELAY       103


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

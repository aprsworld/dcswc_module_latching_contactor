
/* first two or three registers will give enough information for telemetry */
/* 10 bit input voltage ADC averaged value */
#define I2C_REG_VOLTAGE_INPUT_AVG             0  
/* two 8 bit channel states (A is MSB / B is LSB) */
#define I2C_REG_STATE_CONTACTORS              1 
/* 10 bit NTC thermistor ADC averaged value */
#define I2C_REG_TEMPERATURE_BOARD_AVG         2

  
/* channel 0 / A */
/* command can only come from I2C interface */
#define I2C_REG_CH0_COMMAND_ON                3 /* R/W seconds */
#define I2C_REG_CH0_COMMAND_ON_HOLD           4

#define I2C_REG_CH0_COMMAND_OFF               5 /* R/W seconds */
#define I2C_REG_CH0_COMMAND_OFF_HOLD          6

/* low voltage disconnect */
#define I2C_REG_CH0_LVD_DISCONNECT            7  /* R seconds */
#define I2C_REG_CH0_LVD_RECONNECT             8 

/* high voltage disconnect */
#define I2C_REG_CH0_HVD_DISCONNECT            9 /* R seconds */
#define I2C_REG_CH0_HVD_RECONNECT             10

/* low temperature disconnect */
#define I2C_REG_CH0_LTD_DISCONNECT            11  /* R seconds */
#define I2C_REG_CH0_LTD_RECONNECT             12

/* high temperature disconnect */
#define I2C_REG_CH0_HTD_DISCONNECT            13 /* R seconds */
#define I2C_REG_CH0_HTD_RECONNECT             14

/* reserved for future, unimplemented */
#define I2C_REG_CH0_FUT_DISCONNECT            15 /* R seconds */
#define I2C_REG_CH0_FUT_RECONNECT             16

/* channel 1 / B */
/* command can only come from I2C interface */
#define I2C_REG_CH1_COMMAND_ON                17 /* R/W seconds */
#define I2C_REG_CH1_COMMAND_ON_HOLD           18

#define I2C_REG_CH1_COMMAND_OFF               19 /* R/W seconds */
#define I2C_REG_CH1_COMMAND_OFF_HOLD          20

/* low voltage disconnect */
#define I2C_REG_CH1_LVD_DISCONNECT            21 /* R seconds */
#define I2C_REG_CH1_LVD_RECONNECT             22 

/* high voltage disconnect */
#define I2C_REG_CH1_HVD_DISCONNECT            23 /* R seconds */
#define I2C_REG_CH1_HVD_RECONNECT             24

/* low temperature disconnect */
#define I2C_REG_CH1_LTD_DISCONNECT            25  /* R seconds */
#define I2C_REG_CH1_LTD_RECONNECT             26

/* high temperature disconnect */
#define I2C_REG_CH1_HTD_DISCONNECT            27 /* R seconds */
#define I2C_REG_CH1_HTD_RECONNECT             28

/* reserved for future, unimplemented */
#define I2C_REG_CH1_FUT_DISCONNECT            29 /* R seconds */
#define I2C_REG_CH1_FUT_RECONNECT             30

/* meta */
#define I2C_REG_SEQUENCE_NUMBER               31 /* R */
#define I2C_REG_TIME_INTERVAL_MILLISECONDS    32
#define I2C_REG_TIME_UPTIME_MINUTES           33
#define I2C_REG_DEFAULT_PARAMS_WRITTEN        34

/* channel configuration */
/* channel 0 / A */
#define I2C_REG_CH0_COMMAND_ON_HOLD_INITIAL   40

#define I2C_REG_CH0_COMMAND_OFF_HOLD_INITIAL  41

#define I2C_REG_CH0_LVD_DISCONNECT_ADC        42
#define I2C_REG_CH0_LVD_DISCONNECT_DELAY      43
#define I2C_REG_CH0_LVD_RECONNECT_ADC         44
#define I2C_REG_CH0_LVD_RECONNECT_DELAY       45

#define I2C_REG_CH0_HVD_DISCONNECT_ADC        46
#define I2C_REG_CH0_HVD_DISCONNECT_DELAY      47
#define I2C_REG_CH0_HVD_RECONNECT_ADC         48
#define I2C_REG_CH0_HVD_RECONNECT_DELAY       49

#define I2C_REG_CH0_LTD_DISCONNECT_ADC        50
#define I2C_REG_CH0_LTD_DISCONNECT_DELAY      51
#define I2C_REG_CH0_LTD_RECONNECT_ADC         52
#define I2C_REG_CH0_LTD_RECONNECT_DELAY       53

#define I2C_REG_CH0_HTD_DISCONNECT_ADC        54
#define I2C_REG_CH0_HTD_DISCONNECT_DELAY      55
#define I2C_REG_CH0_HTD_RECONNECT_ADC         56
#define I2C_REG_CH0_HTD_RECONNECT_DELAY       57

#define I2C_REG_CH0_FUT_DISCONNECT_VALUE      58
#define I2C_REG_CH0_FUT_DISCONNECT_DELAY      59
#define I2C_REG_CH0_FUT_RECONNECT_VALUE       60
#define I2C_REG_CH0_FUT_RECONNECT_DELAY       61

/* channel 1 / B */
#define I2C_REG_CH1_COMMAND_ON_HOLD_INITIAL   62

#define I2C_REG_CH1_COMMAND_OFF_HOLD_INITIAL  63

#define I2C_REG_CH1_LVD_DISCONNECT_ADC        64
#define I2C_REG_CH1_LVD_DISCONNECT_DELAY      65
#define I2C_REG_CH1_LVD_RECONNECT_ADC         66
#define I2C_REG_CH1_LVD_RECONNECT_DELAY       67

#define I2C_REG_CH1_HVD_DISCONNECT_ADC        68
#define I2C_REG_CH1_HVD_DISCONNECT_DELAY      69
#define I2C_REG_CH1_HVD_RECONNECT_ADC         70
#define I2C_REG_CH1_HVD_RECONNECT_DELAY       71

#define I2C_REG_CH1_LTD_DISCONNECT_ADC        72
#define I2C_REG_CH1_LTD_DISCONNECT_DELAY      73
#define I2C_REG_CH1_LTD_RECONNECT_ADC         74
#define I2C_REG_CH1_LTD_RECONNECT_DELAY       75

#define I2C_REG_CH1_HTD_DISCONNECT_ADC        76
#define I2C_REG_CH1_HTD_DISCONNECT_DELAY      77
#define I2C_REG_CH1_HTD_RECONNECT_ADC         78
#define I2C_REG_CH1_HTD_RECONNECT_DELAY       79

#define I2C_REG_CH1_FUT_DISCONNECT_VALUE      80
#define I2C_REG_CH1_FUT_DISCONNECT_DELAY      81
#define I2C_REG_CH1_FUT_RECONNECT_VALUE       82
#define I2C_REG_CH1_FUT_RECONNECT_DELAY       83


/* configuration */
#define I2C_REG_CONFIG_HARDWARE_MODEL         84 /* R */
#define I2C_REG_CONFIG_HARDWARE_VERSION       85
#define I2C_REG_CONFIG_SOFTWARE_MODEL         86
#define I2C_REG_CONFIG_SOFTWARE_VERSION       87
#define I2C_REG_CONFIG_SOFTWARE_YEAR          88
#define I2C_REG_CONFIG_SOFTWARE_MONTH         89
#define I2C_REG_CONFIG_SOFTWARE_DAY           90

#define I2C_REG_CONFIG_PARAM_WRITE            100 /* R / W */


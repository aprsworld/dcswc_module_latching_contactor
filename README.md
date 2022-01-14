# DCSWC Module, Latching Contactor
PIC18F14K22 micro controller

2 x half bridge drivers for latching contactors
1 x temperature sensor
1 x analog input
2 x switch inputs
2 x LED outputs
2 x onboard DIP switch

## I2C Interface

The microcontroller acts as a I2C slave on the DCSWC bus. 

## Hardware Notes

### 8 position screw terminal 

Pin | Function | Note
---|---|---
1|CONTACTOR\_A\_A|LATCHING CONTACTOR A RED WIRE
2|CONTACTOR\_A\_B|LATCHING CONTACTOR A BLACK WIRE
3|CONTACTOR\_B\_A|LATCHING CONTACTOR B RED WIRE
4|CONTACTOR\_B\_B|LATCHING CONTACTOR B BLACK WIRE
5|LED\_A|IN PARALLEL WITH ONBOARD LED
6|LED\_B|IN PARALLEL WITH ONBOARD LED
7|OVERRIDE\_A|EXTERNAL OVERRIDE BUTTON, IN PARALLEL WITH ONBOARD BUTTON
8|OVERRIDE\_B|EXTERNAL OVERRIDE BUTTON, IN PARALLEL WITH ONBOARD BUTTON



#include "pinOutYamaShoEdition.h"

RawSerial pc(USBTX, USBRX);
RawSerial DualShock3(PA_0, PA_1);
CAN myCAN(PA_11, PA_12, 1e6);
DigitalOut myled(LED1);

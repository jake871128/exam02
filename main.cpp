#include "mbed.h"

#include "fsl_port.h"

#include "fsl_gpio.h"

#include <math.h>

#include "mbed_events.h"

#define UINT14_MAX        16383

// FXOS8700CQ I2C address

#define FXOS8700CQ_SLAVE_ADDR0 (0x1E<<1) // with pins SA0=0, SA1=0

#define FXOS8700CQ_SLAVE_ADDR1 (0x1D<<1) // with pins SA0=1, SA1=0

#define FXOS8700CQ_SLAVE_ADDR2 (0x1C<<1) // with pins SA0=0, SA1=1

#define FXOS8700CQ_SLAVE_ADDR3 (0x1F<<1) // with pins SA0=1, SA1=1

// FXOS8700CQ internal register addresses

#define FXOS8700Q_STATUS 0x00

#define FXOS8700Q_OUT_X_MSB 0x01

#define FXOS8700Q_OUT_Y_MSB 0x03

#define FXOS8700Q_OUT_Z_MSB 0x05

#define FXOS8700Q_M_OUT_X_MSB 0x33

#define FXOS8700Q_M_OUT_Y_MSB 0x35

#define FXOS8700Q_M_OUT_Z_MSB 0x37

#define FXOS8700Q_WHOAMI 0x0D

#define FXOS8700Q_XYZ_DATA_CFG 0x0E

#define FXOS8700Q_CTRL_REG1 0x2A

#define FXOS8700Q_M_CTRL_REG1 0x5B

#define FXOS8700Q_M_CTRL_REG2 0x5C

#define FXOS8700Q_WHOAMI_VAL 0xC7

#define pi 3.141592

DigitalOut led1(LED1);

//DigitalOut led2(LED2);

InterruptIn btn(SW2);


EventQueue queue;

I2C i2c( PTD9,PTD8);

Serial pc(USBTX, USBRX);
Timer ta;
int m_addr = FXOS8700CQ_SLAVE_ADDR1;

void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len);

void FXOS8700CQ_writeRegs(uint8_t * data, int len);


void cal(){ 
   float total =0;
   ta.start(); 
   pc.baud(115200);


   uint8_t who_am_i, data[2], res[6];

   int16_t acc16;

   float t[3];


   // Enable the FXOS8700Q


   FXOS8700CQ_readRegs( FXOS8700Q_CTRL_REG1, &data[1], 1);

   data[1] |= 0x01;

   data[0] = FXOS8700Q_CTRL_REG1;

   FXOS8700CQ_writeRegs(data, 2);

    led1=0;
   // Get the slave address

   FXOS8700CQ_readRegs(FXOS8700Q_WHOAMI, &who_am_i, 1);


   //pc.printf("Here is %x\r\n", who_am_i);
   
   while (ta.read()<10) {
       

      FXOS8700CQ_readRegs(FXOS8700Q_OUT_X_MSB, res, 6);


      acc16 = (res[0] << 6) | (res[1] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[0] = ((float)acc16) / 4096.0f;


      acc16 = (res[2] << 6) | (res[3] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[1] = ((float)acc16) / 4096.0f;


      acc16 = (res[4] << 6) | (res[5] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[2] = ((float)acc16) / 4096.0f;


    pc.printf("%1.4f\r\n",t[0]);
    pc.printf("%1.4f\r\n",t[1]);
    pc.printf("%1.4f\r\n",t[2]);
    
    
   float s = 0;
    t[0] = 9.8 * t[0];
    s = 0.5 * t[0] * 0.01;
    total = s + total ;
    /*
    float a1,b1,thex,they;
    a1 = atan(t[0] / sqrt(t[1]*t[1] + t[2]*t[2]));
    b1= atan(t[1] / sqrt(t[0]*t[0]+t[2]*t[2]));
    //  c1= atan(t[2] / sqrt(t[0]*t[0] +t[1]*t[1]));
    thex = a1*180/pi;
    they =b1*180/pi;
    //  thez =c1*180/pi ;
    int tilt=0;
    if(abs(thex) >=45 || abs(they) >=45) tilt=1;
    else tilt=0;
    pc.printf("%d\r\n",tilt);
    */
    int over = 0;
    if (total >= 0.05 )     over = 1;
    else over = 0;
    pc.printf("%d\r\n",over);
    wait(0.1);

    led1=!led1;
    //printf("x:%1.4f,y:%1.4f",thex,they);
    
   }
    ta.reset();
    led1=1;
}

void f(){
    queue.call(&cal);
}

int main() {

    while(1){
        led1=1;
        btn.rise(f);
        queue.dispatch();
    }
}


void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len) {

   char t = addr;

   i2c.write(m_addr, &t, 1, true);

   i2c.read(m_addr, (char *)data, len);

}


void FXOS8700CQ_writeRegs(uint8_t * data, int len) {

   i2c.write(m_addr, (char *)data, len);

}
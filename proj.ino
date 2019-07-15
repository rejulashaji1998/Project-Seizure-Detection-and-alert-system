#include <GSMSim.h>

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 9600


GSMSim gsm;
int gsm_set_count=0,gsm_set=1;
int count_ax=0;int count_ay=0;int count_az=0;int read_flag_ax=0;int read_flag_ay=0; int read_flag_az=0;
// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added multiple output formats
//                 - added seamless Fastwire support
//      2011-10-07 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

unsigned long int milli_time; 
unsigned long lastSensorRead = 0;
unsigned long lastSensorRead_1 = 0;
unsigned long sensorReadInterval = 40; 

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


#define LED_PIN 13
bool blinkState = false;

void setup() {
 gsm.start(); // baud default 9600
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

   
    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
    
     digitalWrite(LED_PIN, 1);
      delay(1000);
       digitalWrite(LED_PIN, 0);
}

void loop() {

  if(millis() - lastSensorRead > sensorReadInterval){
  
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

if(ax>0){
  read_flag_ax=1;
   
  }

  
if(ay>0){
  read_flag_ay=1;
   
  }
  
if(az>0){
  read_flag_az=1;
   
  }
  
if(read_flag_ax==1 && (ax<0))
{
 read_flag_ax=0;
  count_ax++;
 

  
}
if(read_flag_ay==1 && (ay<0))
{
 read_flag_ay=0;
//  count_ay++;


  
}
if(read_flag_az==1 && (az<0))
{
 read_flag_az=0;
  count_az++;
  

  
}
if(count_ax>=5 || count_ay>=4 || count_az>=5)
{
   digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)

   
 gsm.smsTextMode(true); // TEXT or PDU mode. TEXT is readable :)
 char* number = "+918606340992";
 char* message = "Alert !!! "; // message lenght must be <= 160. Only english characters.
  Serial.println("Sending Message --->");
  Serial.println(gsm.smsSend(number, message)); // if success it returns true (1) else false (0)
  delay(250);                       // wait for a second
  char* phone_no = "+918606340992";
  Serial.println("Calling the number " + String(phone_no));
  Serial.println(gsm.call(phone_no));
  delay(250);   

   

  count_ax=0;count_ay=0;count_az=0; read_flag_ax=0;read_flag_ay=0;read_flag_az=0;}

  lastSensorRead += sensorReadInterval;
   #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
    //    Serial.print("a/g:\t");
     //   Serial.print(ax); Serial.print("\t");
    
    #endif


  }



if(millis() - lastSensorRead_1 > 1000){

 count_ax=0;count_ay=0;count_az=0; read_flag_ax=0;read_flag_ay=0;read_flag_az=0;
  
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  
lastSensorRead_1 += 1000;
}

  }

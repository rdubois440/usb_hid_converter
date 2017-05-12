
//  /opt/arduino-1.7.10-linux32/arduino --board arduino:avr:uno --port /dev/ttyACM13 --upload UnoUsbShield2I2C.ino

#include <Max_LCD.h>
#include <Usb.h>
#include <Max3421e_constants.h>
#include <ch9.h>
#include <Max3421e.h>




#include <Wire.h>



int mode = 2;
int MODE_USB = 1;
int MODE_BT = 2;


/* Arduino Keyboard HID driver and USB host shield pass through */

/*-e
 * Copyright (c) 2011 Darran Hunt (darran [at] hunt dot net dot nz)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*  Simply copies the 8 bytes from the usb report to i2c without translation
    Translation is done at the other end of the i2c connection
    Ex:  cherrymx30_usb_uno_i2c_leonardo or  fpad_usb_uno_i2c_leonardo*/
  

/* keyboard data taken from configuration descriptor */
#define KBD_ADDR        1
#define KBD_EP          1
#define KBD_IF          0
#define EP_MAXPKTSIZE   8
#define EP_POLL         0x0a

/* Sticky keys */
#define CAPSLOCK    0x39
#define NUMLOCK     0x53
#define SCROLLLOCK  0x47

/* Sticky keys output report bitmasks */
#define REP_NUMLOCK       0x01
#define REP_CAPSLOCK      0x02
#define REP_SCROLLLOCK    0x04

EP_RECORD ep_record[2];  //endpoint record structure for the keyboard

//uint8_t buf[8] = { 0 };      // keyboard buffer
char buf[8] = { 0 };      // keyboard buffer
//uint8_t old_buf[8] = { 0 };  // last poll

/* Sticky key state */
bool numLock = false;
bool capsLock = false;
bool scrollLock = false;

void setup();
void loop();
bool key_is_new(byte key, byte *report);

MAX3421E Max;
USB Usb;

void setup() 
{
    Serial.begin(9600);
    Max.powerOn();

    Wire.begin();


    delay(200);
}

void loop() 
{
  
  
    Max.Task();
    Usb.Task();

    if (Usb.getUsbTaskState() == USB_STATE_CONFIGURING) {
        kbd_init();
        Usb.setUsbTaskState( USB_STATE_RUNNING);
    }

    if (Usb.getUsbTaskState() == USB_STATE_RUNNING) {
        kbd_poll();
    }
}

/* Initialize keyboard */
void kbd_init( void )
{
    byte rcode = 0;  //return code

    /* Initialize data structures */
    ep_record[0] = *(Usb.getDevTableEntry(0,0));  //copy endpoint 0 parameters
    ep_record[1].MaxPktSize = EP_MAXPKTSIZE;
    ep_record[1].Interval  = EP_POLL;
    ep_record[1].sndToggle = bmSNDTOG0;
    ep_record[1].rcvToggle = bmRCVTOG0;
    Usb.setDevTableEntry(1, ep_record);              //plug kbd.endpoint parameters to devtable

    /* Configure device */
    rcode = Usb.setConf(KBD_ADDR, 0, 1);                    
    if (rcode) {
        while(1);  //stop
    }
    delay(2000);
}

/* Poll USB keyboard for new key presses, send through to host via 
 * the Keyboard HID driver in the atmega8u2
 */
void kbd_poll(void)
{
    char i;
	char mode;
	int retVal;
	int count;
	
    byte rcode = 0;     //return code
    uint8_t ledReport;
    static uint8_t lastLedReport = 0;
    static uint8_t leds = 0;
    static uint8_t lastLeds = 0;

    /* poll keyboard */
    rcode = Usb.inTransfer(KBD_ADDR, KBD_EP, 8, (char *)buf);
    if (rcode != 0) {
        //Serial.println("rcode is NOT 0");
	return;
    }

    /*Serial.print("rcode is :");
    Serial.println(rcode, HEX);

    Serial.print(buf[0], HEX);
    Serial.print(" ");
	
    Serial.print(buf[1], HEX);
    Serial.print(" ");
    Serial.print(buf[2], HEX);
    Serial.print(" ");
    Serial.print(buf[3], HEX);
    Serial.print(" ");
    Serial.print(buf[4], HEX);
    Serial.print(" ");
    Serial.print(buf[5], HEX);
    Serial.print(" ");
    Serial.print(buf[6], HEX);
    Serial.print(" ");
    Serial.print(buf[7], HEX);
    Serial.println(" ");
*/

	Wire.beginTransmission(0x52);
	


	for (count=0; count<8; count++)
	{
  		retVal = Wire.write(buf[count]);
		//Serial.print("retVal is ");
		//Serial.println(retVal, HEX);
	}
  	retVal = Wire.endTransmission();
	////Serial.print("retVal is ");
	//Serial.println(retVal, HEX);

    //Serial.println(" ");

}



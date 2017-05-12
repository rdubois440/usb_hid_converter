// /opt/arduino-1.7.10-linux32/arduino --board arduino:avr:leonardo --port /dev/ttyACM3 --upload micro_i2c_usb_converter.ino 

// Works in coordination with the usb pass through UnoUsbShield2Serial.ino

/* Connections

		Due				Micro		
Black	GND				GND
Red		5V				VCC
Yellow	TX0-1			RX1

*/


#include <Wire.h>



int  serIn; //var that will hold the bytes in read from the serialBuffer


char keyLookup[256];
char hidLookup[256];

/*boolean isLCtrlDown = 0;
boolean isLShiftDown = 0;
boolean isLAltDown = 0;
boolean isLGuiDown = 0;
boolean isRCtrlDown = 0;
boolean isRShiftDown = 0;
boolean isRAltDown = 0;
boolean isRGuiDown = 0;
*/

boolean wasCapsLockDown = 0;
boolean isLeftButtonDown = 0;
boolean isRightButtonDown = 0;


void setup() 
{
  Serial.begin(9600);
  //Serial1.begin(9600);
  Keyboard.begin();
  Mouse.begin();

  //Serial.println("Starting");

  Wire.begin(0x52);                // Start I2C Bus as a Slave (Device Number 0x52)
  Wire.onReceive(receiveEvent); // register event

 
	for(int i=0; i<256; i++)
	{
		keyLookup[i] = 0;
		hidLookup[i] = i;
	}


	keyLookup[0x04] = 'a';
	keyLookup[0x05] = 'b';
	keyLookup[0x06] = 'c';
	keyLookup[0x07] = 'd';
	keyLookup[0x08] = 'e';
	keyLookup[0x09] = 'f';
	keyLookup[0x0a] = 'g';
	keyLookup[0x0b] = 'h';
	keyLookup[0x0c] = 'i';
	keyLookup[0x0d] = 'j';
	keyLookup[0x0e] = 'k';
	keyLookup[0x0f] = 'l';

	keyLookup[0x10] = 'm';
	keyLookup[0x11] = 'n';
	keyLookup[0x12] = 'o';
	keyLookup[0x13] = 'p';
	keyLookup[0x14] = 'q';
	keyLookup[0x15] = 'r';
	keyLookup[0x16] = 's';
	keyLookup[0x17] = 't';
	keyLookup[0x18] = 'u';
	keyLookup[0x19] = 'v';
	keyLookup[0x1a] = 'w';
	keyLookup[0x1b] = 'x';
	keyLookup[0x1c] = 'y';
	keyLookup[0x1d] = 'z';
	keyLookup[0x1e] = '1';
	keyLookup[0x1f] = '2';

	keyLookup[0x20] = '3';
	keyLookup[0x21] = '4';
	keyLookup[0x22] = '5';
	keyLookup[0x23] = '6';
	keyLookup[0x24] = '7';
	keyLookup[0x25] = '8';
	keyLookup[0x26] = '9';
	keyLookup[0x27] = '0';
	keyLookup[0x28] = KEY_RETURN;
	keyLookup[0x29] = KEY_ESC;
	keyLookup[0x2a] = KEY_BACKSPACE;
	keyLookup[0x2b] = KEY_TAB;
	keyLookup[0x2c] = ' ';
	keyLookup[0x2d] = '-';
	keyLookup[0x2e] = '=';
	keyLookup[0x2f] = '[';

	keyLookup[0x30] = ']';
	keyLookup[0x31] = '\\';
	keyLookup[0x32] = 'x';
	keyLookup[0x33] = ';';
	keyLookup[0x34] = '\'';
	keyLookup[0x35] = '\`';
	keyLookup[0x36] = ',';
	keyLookup[0x37] = '.';
	keyLookup[0x38] = '/';
	keyLookup[0x39] = 1;			// Caps Lock - deactivated
	keyLookup[0x3a] = KEY_F1;
	keyLookup[0x3b] = KEY_F2;
	keyLookup[0x3c] = KEY_F3;
	keyLookup[0x3d] = KEY_F4;
	keyLookup[0x3e] = KEY_F5;
	keyLookup[0x3f] = KEY_F6;

	keyLookup[0x40] = KEY_F7;
	keyLookup[0x41] = KEY_F8;
	keyLookup[0x42] = KEY_F9;
	keyLookup[0x43] = KEY_F10;
	keyLookup[0x44] = KEY_F11;
	keyLookup[0x45] = KEY_F12;
	keyLookup[0x46] = 0x46;			//Print Screen
	keyLookup[0x47] = 'x';			// Scroll Lock
	keyLookup[0x48] = 'x';			// Pause Break
	keyLookup[0x49] = 'x';			// Insert
	keyLookup[0x4a] = KEY_HOME;
	keyLookup[0x4b] = KEY_PAGE_UP;
	keyLookup[0x4c] = KEY_DELETE;
	keyLookup[0x4d] = KEY_END;
	keyLookup[0x4e] = KEY_PAGE_DOWN;
	keyLookup[0x4f] = KEY_RIGHT_ARROW;

	keyLookup[0x50] = KEY_LEFT_ARROW;
	keyLookup[0x51] = KEY_DOWN_ARROW;
	keyLookup[0x52] = KEY_UP_ARROW;
	keyLookup[0x53] = 'x';

	keyLookup[0x64] = '\\';
	keyLookup[0x64] = 0x7f; 		// App key

	keyLookup[0xe0] = KEY_LEFT_CTRL;
	keyLookup[0xe1] = KEY_LEFT_SHIFT;
	keyLookup[0xe2] = KEY_LEFT_ALT;
	keyLookup[0xe3] = KEY_LEFT_GUI;
	keyLookup[0xe4] = KEY_RIGHT_CTRL;
	keyLookup[0xe5] = KEY_RIGHT_SHIFT;
	keyLookup[0xe6] = KEY_RIGHT_ALT;
	keyLookup[0xe7] = KEY_RIGHT_GUI;

	// LingYi Customisation - old stype
	keyLookup[0x45] = KEY_DELETE;		// F12
	keyLookup[0x46] = KEY_F12;			//Print Screen
	keyLookup[0x47] = KEY_PAGE_UP;			// Scroll Lock
	keyLookup[0x48] = KEY_PAGE_DOWN;			// Pause Break
	keyLookup[0x49] = KEY_HOME;			// Insert
	keyLookup[0x4a] = KEY_UP_ARROW;		// Home
	keyLookup[0x4b] = KEY_END;		// Page Up
	keyLookup[0x4c] = KEY_LEFT_ARROW;		// Delete 
	keyLookup[0x4d] = KEY_DOWN_ARROW;		// End
	keyLookup[0x4e] = KEY_RIGHT_ARROW;		// Page Down

	//keyLookup[0x45] = KEY_DELETE;		// F12

	// LingYi Customisation - hid reports
	hidLookup[0x45] = 0x4c;		// F12 - is now Delete
	hidLookup[0x46] = 0x45;		//Print Screen is now F12
	hidLookup[0x47] = 0x4b;		// Scroll Lock is now Page Up
	hidLookup[0x48] = 0x4e;		// Pause Break is now Page Down
	hidLookup[0x49] = 0x4a;		// Insert is now Home
	hidLookup[0x4a] = 0x52;		// Home is now up arrow
	hidLookup[0x4b] = 0x4d;		// Page Up is now End
	hidLookup[0x4c] = 0x50;		// Delete  is now left arrow
	hidLookup[0x4d] = 0x5a;		// End is now down arrow
	hidLookup[0x4e] = 0x4f;		// Page Down is now right arrow

}

void loop()
{
	//Serial.println("Looping");
	delay(50);
}

/*
void loop () 
*/

void receiveEvent(int howMany) 
{
  
  	int isCapsLockDown;
  	int isLShiftDown;

	char curByte;

	KeyReport report;
	

  //Serial.println("Received Event");

  	isCapsLockDown = 0;
  	isLShiftDown = 0;

	report.modifiers = Wire.read();  


	if ((report.modifiers & 0x2) >0) 	// Left Shift is pressed 
	{
  		isLShiftDown = 1;
    	//Serial.println("Right Mouse button pressed");
	}
	report.reserved = Wire.read();
  	for (int i=0; i<6; i++)
  	{
		curByte = Wire.read();
		// Special handling for caps lock - do not pass on to the host - pass the next byte instead
		if (curByte == 0x39)
		{
  			isCapsLockDown = 1;
			if (i < 5)
			{
				curByte = Wire.read();
			}
		}

  		report.keys[i] = hidLookup[curByte];  


  	}

  	if (isCapsLockDown == 1)
	{
		// one byte was skipped in the report, restore it
  		report.keys[5] = 0;  
	}


	Keyboard.sendReport(&report);

	if (isCapsLockDown && !isLShiftDown)
	{
		Serial.println("Left mouse pressed");
		Mouse.press(MOUSE_LEFT);
		isLeftButtonDown = 1;
	}
	if (isCapsLockDown && isLShiftDown)
	{
		Serial.println("Right mouse pressed");
		Mouse.press(MOUSE_RIGHT);
		isRightButtonDown = 1;
	}

	if (wasCapsLockDown && !isCapsLockDown)
	{
		Serial.println("Mouse released");
		if (isLeftButtonDown = 1)
		{
			Mouse.release(MOUSE_LEFT);
			isLeftButtonDown = 0;
		}
		if (isLeftButtonDown = 1)
		{
			Mouse.release(MOUSE_RIGHT);
			isRightButtonDown = 0;
		}
	}
	wasCapsLockDown = isCapsLockDown;	
}	

/*

  //Serial.print("Received data ");
  for (i=0; i<8; i++)
  {
    //Serial.print(buf[i], HEX);
    //Serial.print(" - ");

  }
  //Serial.println("");



  char modifier = buf[0];

	if (buf[2] == 0x39)
	{
    	//Serial.println("Mouse button pressed");
		if ((modifier & 0x2) >0) 	// Shift is pressed 
		{
			Mouse.press(MOUSE_RIGHT);
    		//Serial.println("Right Mouse button pressed");
		}
		else
		{
			Mouse.press(MOUSE_LEFT);
    		//Serial.println("Left Mouse button pressed");
		}	

		isMousePressed = 1;
	}

	if 	((buf[2] == 0) || (keyLookup[buf[2]] == 0)) 
	{
		Keyboard.releaseAll();
		if (isMousePressed == 1)
		{
			Mouse.release(MOUSE_LEFT);
			Mouse.release(MOUSE_RIGHT);
			isMousePressed = 0; 
		}
	}



	else
	{
		//Serial.print("Modifier is -- ");
		//Serial.println(modifier, HEX);

		if ((modifier & 0x1) > 0) Keyboard.press(KEY_LEFT_CTRL);
		if ((modifier & 0x2) >0) Keyboard.press(KEY_LEFT_SHIFT);
		if ((modifier & 0x4) > 0)	Keyboard.press(KEY_LEFT_ALT);
		if ((modifier & 0x8) > 0)	Keyboard.press(KEY_LEFT_GUI);
		if ((modifier & 0x10) > 0)	Keyboard.press(KEY_RIGHT_CTRL);
		if ((modifier & 0x20) > 0)	Keyboard.press(KEY_RIGHT_SHIFT);
		if ((modifier & 0x40) > 0)	Keyboard.press(KEY_RIGHT_ALT);
		if ((modifier & 0x80) > 0)	Keyboard.press(KEY_RIGHT_GUI);


		Keyboard.press(keyLookup[buf[2]]);
	}
}



////////////////////////////////


/opt/arduino-1.7.10-linux32/arduino --board arduino:avr:leonardo --port /dev/ttyACM3 --upload hid_test.ino 


//	Low level key report: up to 6 keys and shift, ctrl etc at once

KeyReport reportA;
KeyReport reportClear;


void setup() 
{
	Keyboard.begin();

	reportA.modifiers = 0;
	reportA.reserved = 0;
	reportA.keys[0] = 4;
	reportA.keys[1] = 0;
	reportA.keys[2] = 0;
	reportA.keys[3] = 0;
	reportA.keys[4] = 0;
	reportA.keys[5] = 0;

	reportClear.modifiers = 0;
	reportClear.reserved = 0;
	reportClear.keys[0] = 0;
	reportClear.keys[1] = 0;
	reportClear.keys[2] = 0;
	reportClear.keys[3] = 0;
	reportClear.keys[4] = 0;
	reportClear.keys[5] = 0;

}

void loop() 
{
	Keyboard.sendReport(&reportA);
	Keyboard.sendReport(&reportClear);
	delay(3000);
}

/opt/arduino-1.7.10-linux32/arduino --board arduino:avr:leonardo --port /dev/ttyACM3 --upload hid_test.ino 


//	Low level key report: up to 6 keys and shift, ctrl etc at once



void setup() 
{
	Keyboard.begin();

	reportA.modifiers = 0;
	reportA.reserved = 0;
	reportA.keys[0] = 4;
	reportA.keys[1] = 0;
	reportA.keys[2] = 0;
	reportA.keys[3] = 0;
	reportA.keys[4] = 0;
	reportA.keys[5] = 0;

	reportClear.modifiers = 0;
	reportClear.reserved = 0;
	reportClear.keys[0] = 0;
	reportClear.keys[1] = 0;
	reportClear.keys[2] = 0;
	reportClear.keys[3] = 0;
	reportClear.keys[4] = 0;
	reportClear.keys[5] = 0;

}

void loop() 
{
	Keyboard.sendReport(&reportA);
	Keyboard.sendReport(&reportClear);
	delay(3000);
}
*/

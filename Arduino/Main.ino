#include <CayenneMQTTESP8266.h>
#include <SoftwareSerial.h>


// WiFi network info.
char ssid[] = "xxxxxxx";
char wifiPassword[] = "xxxxxx";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "xxxxx";
char password[] = "xxxxx";
char clientID[] = "xxxxx";

SoftwareSerial BLE(13, 12); // RX, TX  
							// Connect HM10      Arduino Uno
							//     Pin 1/TXD          Pin 13
							//     Pin 2/RXD          Pin 12
bool bleFound = false;
byte outerCounter = 0;
byte innerCounter = 0;

void setup() {
	Serial.begin(115200);
	// If the baudrate of the HM-10 module has been updated,
	// you may need to change 9600 by another value
	// Once you have found the correct baudrate,
	// you can update it using AT+BAUDx command 
	// e.g. AT+BAUD0 for 9600 bauds
	BLE.begin(115200);
	//delay(250);
	//String serialResponse = "";

	//BLE.print("AT");
	//delay(250);
	//if (BLE.available())
	//{
	//	serialResponse = BLE.readStringUntil('\r\n');
	//	Serial.println(serialResponse);
	//}

	//Connect to the WiFi and establish Cayenne cloud session
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	char *buffer;
	char data[100];
	Cayenne.loop(1000);

	Serial.println("OuterLoop: " + String(outerCounter));

	if (!bleFound)
	{
		BLE.print("AT+DISC?");
		delay(2000);
	}

	int dataCount = BLE.available();
	if (dataCount > 0)
	{
		//Get data from UART stream
		BLE.readBytesUntil('\r\n', data, dataCount);
		data[dataCount + 1] = '\0';
		buffer = strstr(data, "NAME:");
		if (buffer)
		{
			Serial.println("InnerLoop: " + String(innerCounter));
			innerCounter++;
			bleFound = true;
			//int nameLength = strlen(buffer - 5);
			char slaveName[13];
			char slaveAddress[13];

			strcpy(slaveName, buffer + 5);
			slaveName[12] = '\0';
			Serial.println(String(slaveName));

			buffer = strstr(data, "DIS0:");
			strncpy(slaveAddress, buffer + 5, 12);
			slaveAddress[12] = '\0';
			Serial.println(String(slaveAddress));

			BLE.print("AT+CON" + String(slaveAddress));
			delay(1000);
			Serial.println(BLE.readStringUntil('\r\n'));
			BLE.print("AT+PIO21");
			delay(1000);
			Serial.println(BLE.readStringUntil('\r\n'));
			BLE.print("AT+ADC4?");
			delay(1000);
			String hum = BLE.readStringUntil('\r\n');
			Serial.println(hum);
			long humidity = hum.substring(8).toFloat() * 1000;
			Serial.println(hum.substring(8).toFloat());
			//byte humidity = hum.substring(8).toFloat() * 100;
			BLE.print("AT");
			Cayenne.virtualWrite(0, humidity,TYPE_VOLTAGE, UNIT_MILLIVOLTS);
		}
		else
			bleFound = false;
	}
	else
		bleFound = false;

	delay(10000);
	outerCounter++;
	memset(data, 0, sizeof(data));
}

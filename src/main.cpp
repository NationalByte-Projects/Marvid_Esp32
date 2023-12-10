/*
Nota: Before running this code, check that the GPS of the SIM7600G module is activated automatically. 
You can check it with the following code:
    https://github.com/MauricioOrtega10/SIm7600G_auto_GPS.git
*/
#include <Arduino.h>
#include <Preferences.h>
#include <BluetoothSerial.h>
#include "ESP32Time.h" 

#include "max6675.h"

// Your GPRS credentials (Telcel)
const char apn[]      = "Broadband";
const char gprs_user[] = "webgprs";
const char gprs_pass[] = "webgprs2002";

// Server details
const char server[] = "transpositive-deput.000webhostapp.com";
const char resource[] = "/PHP/Insert_Data.php";
const int  port = 80;
const String api_key_value = "1D0LUQLB4CO9LM9C"; //Not needed in this case

// SIM 7600G  
#define MODEM_SLEEP 5
#define MODEM_TX    17
#define MODEM_RX    16

// MAX6675 Thermocouple 
#define THERMO_SO  12
#define THERMO_CLK 14
#define THERMO_CS  15

// Multifunction button
#define BTN 23

// Set serial for debug console
#define SerialMon Serial

// Set serial for AT commands
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

//baud rate value for SerialAT an SerialMON
#define BAUD_RATE 115200

//Conversions 
#define KNOTS_TO_KM_H 1.852f
#define MS_TO_SEG     1000

#include <TinyGsmClient.h>

//constructors
MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_SO);
ESP32Time rtc;
Preferences preferences;
TinyGsm modem(SerialAT);

// TinyGSM Client for Internet connection
TinyGsmClient client(modem);

// Alarm values
const unsigned long temperature_alarm = 100.0; // Celsius degrees
const unsigned long speed_alarm       = 100.0; // Km/h

// Auxiliary timer variables in seconds
const unsigned long http_time         = 60.0;
const unsigned long eeprom_time       = 180.0;
const unsigned long bluetooth_time    = 20.0;
const unsigned long alarm_time        = 20.0;
const unsigned long long_press_time   = 5.0;
unsigned long now;
unsigned long http_last_trigger;
unsigned long eeprom_last_trigger;
unsigned long alarm_last_trigger;
unsigned long start_time;
unsigned long pulse_time;

// GPS variables 
float lat_, lon_, speed_, alt_, accuracy_;
int vsat_, usat_, year_, month_, day_, hour_, min_, sec_;

float temperature;
String name;

// Prototype functions
bool updateName();
void updateOnTime();
void resetData();
bool httpRequest();
int httpResponse(const String &string);
void totalOnTime(int & hrs, int & mins, int & segs);

void setup()
{
  pinMode(BTN, INPUT_PULLUP);
  SerialMon.begin(BAUD_RATE);

  //Set SLEEP Pin low , wakeup modem .
  pinMode(MODEM_SLEEP, OUTPUT);
  digitalWrite(MODEM_SLEEP, LOW);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(BAUD_RATE, SERIAL_8N1, MODEM_RX, MODEM_TX, false);
  delay(5000);

  // Restart SIM module, it takes quite some time
  SerialMon.println("Initializing modem...");
  modem.restart();
  modem.init();

  rtc.setTime(0, 0, 0, 1, 1, 2023);

  // Open Preferences with "Marvid" namespace.
  // RW-mode (second parameter has to be false).
  preferences.begin("Marvid", false);

  // Get the counter value
  name = preferences.getString("name", "Marvid1");
  preferences.getInt("segs", 0);
  preferences.getInt("mins", 0);
  preferences.getInt("hrs", 0);

  // Close the Preferences
  preferences.end();

  // Delay needed to properly initialize SIM/GPS
  delay(20000L);

  // Initializing triggers
  http_last_trigger = millis();
  eeprom_last_trigger = millis();
  alarm_last_trigger = millis();
  SerialMon.println("Initializing APP!!!");
}

void loop()
{
  //Depending on the time the button was held down, one function or another will be executed.
  if (digitalRead(BTN) == LOW)
  {
    pulse_time = millis();
    while (digitalRead(BTN) == LOW)
    {
    }
    start_time = millis();
    pulse_time = start_time - pulse_time;
    if (pulse_time < (long_press_time * MS_TO_SEG))
    {
      SerialMon.println("Short press");
      if(!updateName())
      {
        SerialMon.println("Time is up, disconnecting device...");
      }
      else
      {
       SerialMon.println("Name updated succesfully, disconnecting device..."); 
      }
    }
    else
    {
      SerialMon.println("Long press");
      resetData();
    }
  }

  now = millis();

  // If one of the alarms is activated, httpRequest will be executed in a shorter interval
  if ((now - alarm_last_trigger > (alarm_time * MS_TO_SEG)))
  {
    modem.getGPS(&lat_, &lon_, &speed_, &alt_, &vsat_, &usat_, &accuracy_,
        &year_, &month_, &day_, &hour_, &min_, &sec_);
    temperature = thermocouple.readCelsius();

    if (temperature > temperature_alarm || (speed_ * KNOTS_TO_KM_H) > speed_alarm)
    {
      while(!httpRequest())
      {
      }
      alarm_last_trigger = now;
    }   
  }

  // http request will be executed at a defined interval
  if ((now - http_last_trigger > (http_time * MS_TO_SEG)))
  {
    while(!httpRequest())
    {
    }
    http_last_trigger = now;   
  }

  // Total on time will be saved in the eeprom at a defined interval
  if ((now - eeprom_last_trigger > (eeprom_time * MS_TO_SEG)))
  {
    updateOnTime();
    eeprom_last_trigger = now;  
  }
}

// Resets the total on time of the eeprom
void resetData()
{
  preferences.begin("Marvid", false);
  preferences.putInt("segs", 0);
  preferences.putInt("mins", 0);
  preferences.putInt("hrs", 0);
  rtc.setTime(0, 0, 0, 1, 1, 2023);
  preferences.end();
}

// Saves the total on time in the eeprom
void updateOnTime()
{
  SerialMon.print("Update time: ");
  int hrs, mins, segs;
  totalOnTime(hrs, mins, segs);
  preferences.begin("Marvid", false);
  preferences.putInt("segs", segs);
  preferences.putInt("mins", mins);
  preferences.putInt("hrs", hrs);
  rtc.setTime(0, 0, 0, 1, 1, 2023);
  preferences.end();
  SerialMon.println("finish");
}

/* Activates Bluetooth to be able to change the name and save it in the eeprom, if the name is equal to "" the name will not change, 
if the name is not changed in "x" time a false is returned and the name will not be changed */
bool updateName()
{
  BluetoothSerial SerialBT;
  preferences.begin("Marvid", false);
  SerialBT.begin("Marvid");
  SerialMon.println("The device has been started, now you can pair it with bluetooth!");
  String waiting_msg = "Waiting for a New Name...\n";
  String return_msg;

  unsigned long bth_now = millis();
  unsigned long bluetoothlastTrigger = millis();
  while (!SerialBT.available())
  {
    if ((bth_now - bluetoothlastTrigger > (bluetooth_time * MS_TO_SEG)))
    {
      return_msg = "\nTime is up, disconnecting device...";
      for (char caracter : return_msg)
      {
        SerialBT.write(caracter);
      }
      preferences.end();
      SerialBT.flush();
      SerialBT.end();
      return false;
    }
    SerialMon.println(waiting_msg);
    for (char caracter : waiting_msg)
    {
      SerialBT.write(caracter);
    }
    delay(1000);
    bth_now = millis();
  }
  char incomingChar;
  name = "";
  // Note: The character string must end with an ending line New line
  while ((incomingChar = SerialBT.read()) != '\n')
  {
    name += String(incomingChar);
  }
  if (name == "")
  {
    return_msg = "\nName was not updated, disconnecting device...";
    for (char caracter : return_msg)
    {
      SerialBT.write(caracter);
    }
  }
  else
  {
    return_msg = "Name updated succesfully to: " + name + ", disconnecting device...";
    for (char caracter : return_msg)
    {
      SerialBT.write(caracter);
    }
    preferences.putString("name", name);
  }
  name = preferences.getString("name", "Marvid1");
  preferences.end();
  SerialBT.flush();
  SerialBT.end();
  return true;
}

// Finds and returns the http request value as an integer and if it does not find it, returns a -1
int httpResponse(const String &string) {
  // Find the position of "HTTP/1.1 "
  int start_position = string.indexOf("HTTP/1.1 ");
  
  if (start_position != -1) 
  {
    // Extract the part of the string after "HTTP/1.1 "
    String value_str = string.substring(start_position + 9); // "HTTP/1.1 " long is 9 char
    
    // Convert extracted string to integer
    int value = value_str.toInt();
    
    return value;
  } 
  else 
  {
    return -1;
  }
}

// Executes an http request and if there is a failure or the http response is different than 200 it returns false and if not it returns true.
bool httpRequest() 
{
  int hrs, mins, segs;
  totalOnTime(hrs, mins, segs);

  SerialMon.print("Requesting current GPS/GNSS/GLONASS location: ");
  if (!modem.getGPS(&lat_, &lon_, &speed_, &alt_, &vsat_, &usat_, &accuracy_,
        &year_, &month_, &day_, &hour_, &min_, &sec_))
  {
    SerialMon.println("NOK");
    return false;
  }
  else
  { 
    SerialMon.println("OK");
    temperature = thermocouple.readCelsius();
    SerialMon.print("Connecting to APN: ");
    SerialMon.print(apn);
    if(!modem.gprsConnect(apn, gprs_user, gprs_pass)) 
    {
      SerialMon.println(" NOK");
      modem.gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
      return false;
    }
    else
    {
      SerialMon.println(" OK");
      SerialMon.print("Connecting to ");
      SerialMon.print(server);
      if(!client.connect(server, port)) 
      {
        SerialMon.println(" NOK");
        client.stop();
        SerialMon.println(F("Server disconnected"));
        return false;
      }
      else
      {
        SerialMon.println(" OK");  
        SerialMon.println("Performing HTTP POST request...");
        // Prepare your HTTP POST request data
        String httpRequestData = "api_key=" + api_key_value + "&Data=" + String("Marvid") + "," + 
          String(name) + "," + String(temperature) + "," + String(hrs) + "," + String(speed_ * KNOTS_TO_KM_H) + "," + String(lat_, 5) + "," + String(lon_, 5) + "";
        client.print(String("POST ") + resource + " HTTP/1.1\r\n");
        client.print(String("Host: ") + server + "\r\n");
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println(httpRequestData.length());
        client.println();
        client.println(httpRequestData);
        unsigned long timeout = millis();
        String response = "";
        while (millis() - timeout < 1000L) 
        {
          while (client.available()) 
          {
            char c = client.read();
            response += String(c);
            timeout = millis();
          }
        }
        SerialMon.println(response);
        SerialMon.println();
        SerialMon.println("Fin de la respuesta");
        SerialMon.println(httpResponse(response));
        SerialMon.println();
        // Close client and disconnect
        client.stop();
        SerialMon.println(F("Server disconnected"));
        modem.gprsDisconnect();
        SerialMon.println(F("GPRS disconnected"));
        if (httpResponse(response) != 200)
        {
          return false;
        }
        else
        {
          return true;
        }
      }
    }
  }
}

// Adds the total on time saved in the eeprom and in the rtc
void totalOnTime(int & hrs, int & mins, int & segs)
{
  preferences.begin("Marvid", false);
  segs = rtc.getSecond() + preferences.getInt("segs", 0);
  mins = rtc.getMinute() + preferences.getInt("mins", 0);
  hrs = rtc.getHour() + preferences.getInt("hrs", 0);
  if (segs >= 60)
  {
    mins += abs(segs / 60);
    segs %= 60;
  }
  if (mins >= 60)
  {
    hrs += abs(mins / 60);
    mins %= 60;
  }
  preferences.end();
}
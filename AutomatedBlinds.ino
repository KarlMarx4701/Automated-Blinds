#include <Time.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

HTTPClient http;
int httpCode = -1;
int i = 0;
tmElements_t tmSet;
bool successfulSetup = true;
bool areBlindsOpen = false;

const char* ssid = "SSID HERE";
const char* password = "PASSWORD HERE";

DynamicJsonDocument jsonDoc(1024);

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up device...");
  
  setupWifi();
  pinMode(5, OUTPUT);

  // This will automatically pull the time and ensure it is synced every 5 seconds.
  setSyncProvider(setCurrentTimeFromAPI);
  setSyncInterval(5);
  
  Serial.println("Setup completed!");
}

time_t setCurrentTimeFromAPI() {
  Serial.println("Syncing time...");

  // World Time API will return a JSON response with the regions current time.
  // More information: http://worldtimeapi.org/

  // Make GET request to World Time API to get the current time for my region.
  http.begin("http://worldtimeapi.org/api/timezone/America/Indianapolis");
  httpCode = http.GET();

  // If the GET request failed.
  if (httpCode <= 0) {
    Serial.println("Error in making GET request!");
    return NULL;
  }

  // This will turn the JSON response into usable data for us.
  deserializeJson(jsonDoc, http.getString());

  // This will get the "datetime" string from World Time's response.
  const char* dateTimeChars = jsonDoc["datetime"];
  String dateTimeString = String(dateTimeChars);

  // Set the elements of a time object
  tmSet.Hour = dateTimeString.substring(11,13).toInt();
  tmSet.Minute = dateTimeString.substring(14,16).toInt();
  tmSet.Second = dateTimeString.substring(17,19).toInt();
  tmSet.Day = dateTimeString.substring(8,10).toInt();
  tmSet.Month = dateTimeString.substring(5,7).toInt();
  tmSet.Year = dateTimeString.substring(0,4).toInt();

  // Free up the http resources for other things.
  http.end();

  // Return the time object created by parsing the World Time API.
  return makeTime(tmSet);
}

void setupWifi() {
  Serial.println("Setting up WiFi...");
  int tries = 0;

  // Start the WiFi library with provided SSID and Password
  WiFi.begin(ssid, password);

  // Wait until th WiFi is connected or we have tried 10 times.
  while (WiFi.status() != WL_CONNECTED && tries < 11) {
    delay(2000);
    Serial.println("Connecting to WiFi...");
    tries++;
  }

  // If the WiFi connection fails, don't allow code in the loop to run.
  // Otherwise, setup is complete.
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi unable to connect!");
    successfulSetup = false;
  } else {
    Serial.println("WiFi connected successfully!");
    Serial.println(WiFi.localIP());
  }
}

void openBlinds() {
  // This code will need to be completed once an H-Bridge is acquired to control
  // the motor bi-directionally.
  Serial.println("Opening blinds!");
  digitalWrite(5, HIGH);
  delay(3000);
  digitalWrite(5, LOW);
  areBlindsOpen = true;
}

void closeBlinds() {
  // This code will need to be completed once an H-Bridge is acquired to control
  // the motor bi-directionally.
  Serial.println("Closing blinds!");
  areBlindsOpen = false;
}

void loop() {
  if (successfulSetup) {
    // Sync the time status if needed.
    timeStatus();
    // If the time is between 7:00 PM and 6:59 AM, close the blinds
    if ((hour(now()) > 18 || hour(now()) < 7) && areBlindsOpen) {
      closeBlinds();
    // If the time is between 7:00 AM and 6:59 PM, open the blinds
    } else if ((hour(now()) <= 18 && hour(now()) >= 7) && !areBlindsOpen) {
      openBlinds();
    }
  }

}

# Automated-Blinds
This project is an attempt to build my own automated blinds. This should close and open the blinds at a user defined time. This project is insipred by my inability to wake up without light in my room.  
Author: [Ben Lawson](mailto:lawson470189@gmail.com)  
Last Updated: 05/21/2019  

## Requirements
1. Control a set of blinds using an ESP-32, an H-Bridge, and a motor.
2. Have a web-interface for setting the open and close times of the blinds.
3. The opening and closing of the blinds should require no user input after the setting of the open and close times.
4. The setup process should be easy for the user.

## Todo
- Write code for H-Bridge circuit to control the motor bi-directionally.
- Write code for web interface.
- Change WiFi setup process to web interface.

## Libraries Used
- [Time.h / TimeLib.h](https://github.com/PaulStoffregen/Time)
    - This library allows for the automatic syncing of the time.
- WiFi.h
    - This library is to enable the ESP-32 to use the WiFi capabilities.
- HTTPClient.h
    - To make GET requests to the World Time API.
- [ArduinoJson.h](https://arduinojson.org/)
    - To parse the response from World Time's API.

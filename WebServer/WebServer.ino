#include <Arduino.h>
#include <WebServer.h>
#include <uri/UriRegex.h>
#include <WiFi.h>
#include <iostream>
#include <string>
#include <ESP32Servo.h>
#include "secrets.h"  // add WLAN Credentials in here.
#include <FS.h>        // File System for Web Server Files

// name of the server. You reach it using http://webserver
#define HOSTNAME "webserver"

// need a WebServer for http access on port 80.
WebServer server(80);

// The text of builtin files are in this header file
#include "builtinfiles.h"

#define GPIO_LED 9
#define GPIO_SERVO 18

// ===== Simple functions used to answer simple GET requests =====

// This function is called when the sysInfo service was requested.

int brightness = 0;
int angle = 0;
Servo motor;

void control() {
  String led = server.arg("a");
  String servo = server.arg("b");

  brightness = led.toInt();
  angle = servo.toInt();

  server.send(200, "application/json","{\"status\":\"ok\"\nb\":" + String(brightness) + "\n\"angle\":" + String(angle) + "\n\"success\": 1}");
}  // handleSysInfo()


// Setup everything to make the webserver work.
void setup(void) {
  delay(3000);  // wait for serial monitor to start completely.

  // Use Serial port for some trace information from the example
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  // allow to address the device by the given name e.g. http://webserver
  WiFi.setHostname(HOSTNAME);
  WiFi.setSleep(false);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, passPhrase);

  // register some REST services
  server.on("/ctrl", HTTP_GET, control);
  // enable CORS header in webserver results
  server.enableCORS(true);

  server.on("/", []() {
    server.send(200, "text/html", FPSTR(ctrlContent));
  });

  server.begin();

  motor.attach(GPIO_SERVO);

  ledcAttach(GPIO_LED, 5000, 8);
}  // setup

// run the server...
void loop(void) {
  server.handleClient();

  ledcAttach(GPIO_LED, 5000, 8);
  bool res = ledcWrite(GPIO_LED, brightness);

  motor.write(angle);
  
}  // loop()

// end.

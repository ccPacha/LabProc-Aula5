#include <Arduino.h>
#include <WebServer.h>
#include <uri/UriRegex.h>
#include <WiFi.h>
#include <iostream>
#include <string>
#include <ESP32Servo.h>
#include "secrets.h"  // add WLAN Credentials in here.
#include <FS.h>        // File System for Web Server Files

// mark parameters not used in example
#define UNUSED __attribute__((unused))

// TRACE output simplified, can be deactivated here
#define TRACE(...) Serial.printf(__VA_ARGS__)

// name of the server. You reach it using http://webserver
#define HOSTNAME "webserver"

// local time zone definition (Berlin)
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

// need a WebServer for http access on port 80.
WebServer server(80);

// The text of builtin files are in this header file
#include "builtinfiles.h"


// ===== Simple functions used to answer simple GET requests =====

// This function is called when the sysInfo service was requested.

Servo motor;

void control() {
  String led = server.arg("a");
  String servo = server.arg("b");

  int brightness = led.toInt();
  int angle = servo.toInt();

  ledcWrite(8, brightness);
  angle = constrain(angle, 0, 180);
  motor.write(angle);

  server.send(200, "application/json","{\"status\":\"ok\"}");
}  // handleSysInfo()


// Setup everything to make the webserver work.
void setup(void) {
  delay(3000);  // wait for serial monitor to start completely.

  // Use Serial port for some trace information from the example
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  TRACE("Starting WebServer example...\n");

  // allow to address the device by the given name e.g. http://webserver
  WiFi.setHostname(HOSTNAME);

  WiFi.setSleep(false);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, passPhrase);

  delay(1000);

  TRACE("connected.\n");

  // Ask for the current time using NTP request builtin into ESP firmware.
  TRACE("Setup ntp...\n");
  configTzTime(TIMEZONE, "pool.ntp.org");

  TRACE("Register redirect...\n");

  TRACE("Register service handlers...\n");

  // register some REST services
  server.on("/ctrl", HTTP_GET, control);

  TRACE("Register file system handlers...\n");

  // enable CORS header in webserver results
  server.enableCORS(true);

  TRACE("Register default (not found) answer...\n");

  server.on("/", []() {
    server.send(200, "text/html", FPSTR(ctrlContent));
  });

  server.begin();

  TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());

  ledcAttach(8, 5000, 8);
  motor.setPeriodHertz(50);
  motor.attach(18);
}  // setup

// run the server...
void loop(void) {
  server.handleClient();
  TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());
}  // loop()

// end.

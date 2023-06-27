#include <Arduino.h>
#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"

// Create a simple wifi Access point and provide a web server on it.
// The access point has no internet connection, and the web server
// Used is ESP8266

const char *ssid = "ESP-TestStation";
const char *password = "";

DNSServer dnsServer;
AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  dnsServer.start(53, "*", WiFi.softAPIP());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Display the HTML web page
    request->send(200, "text/html", R"(
        <!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>ESP-TestStation</title>
    <style>
        /* CSS for the website layout */
        body {
            font-family: Arial, sans-serif;
            background-color: #f2f2f2;
            margin: 0;
            padding: 0;
        }

        #container {
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100vh;
        }

        #table {
            position: relative;
            width: 300px;
            height: 300px;
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
            animation: tableAnimation 2s infinite alternate;
        }

        @keyframes tableAnimation {
            0% {
                transform: translateY(0);
            }
            100% {
                transform: translateY(-10px);
            }
        }

        #content {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            color: #333;
            text-align: center;
        }
    </style>
</head>
<body>
    <div id="container">
        <div id="table">
            <div id="content">
                <h1>ESP-TestStation</h1>
                <p>Jetzt kann man seinen ESP hiermit verbinden</p>
            </div>
        </div>
    </div>
</body>
</html>
)");
    });

  server.begin();

  // Set up a callback function to trigger when a new client connects to the access point
  WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected &event)
{
    Serial.print("Station connected: ");
});
}

void loop()
{
  dnsServer.processNextRequest();
}
#include <Arduino.h>
#include <DNSServer.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"

DNSServer dnsServer;
AsyncWebServer server(80);

String user_name;
String proficiency;
bool name_received = false;
bool proficiency_received = false;

bool started = false;

const char index_html[] PROGMEM = R"rawliteral(
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

)rawliteral";

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        // request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/html", index_html);
    }
};

void setupServer()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
      request->send_P(200, "text/html", index_html); 
      Serial.println("Client Connected"); });

}

void setup()
{
    // your other setup stuff...
    Serial.begin(115200);
    Serial.println();
    Serial.println("Setting up AP Mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP-TestStation");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("Setting up Async WebServer");
    setupServer();
    Serial.println("Starting DNS Server");
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
    // more handlers...
    server.begin();
    Serial.println("All Done!");
}

void loop()
{
    dnsServer.processNextRequest();
    if (name_received && proficiency_received)
    {
        Serial.print("Hello ");
        Serial.println(user_name);
        Serial.print("You have stated your proficiency to be ");
        Serial.println(proficiency);
        name_received = false;
        proficiency_received = false;
        Serial.println("We'll wait for the next client now");
    }
}
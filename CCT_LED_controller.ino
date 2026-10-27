#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

const int hotPIN = 32;
const int coldPIN = 33;

WebServer server(80);

void handleDynamicPaths() {
  String uri = server.uri();

  if (uri.startsWith("/hot/")) {
    int sliderValue = uri.substring(5).toInt();
    int pwmValue = map(sliderValue, 0, 100, 0, 255);
    analogWrite(hotPIN, pwmValue);
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", "OK");
    return;
  }

  if (uri.startsWith("/cold/")) {
    int sliderValue = uri.substring(6).toInt();
    int pwmValue = map(sliderValue, 0, 100, 0, 255);
    analogWrite(coldPIN, pwmValue);
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", "OK");
    return;
  }

  server.sendHeader("Connection", "close");
  server.send(404, "text/plain", "Not found");
}


void setup() {
  Serial.begin(115200);

  pinMode(hotPIN, OUTPUT);
  pinMode(coldPIN, OUTPUT);
  analogWrite(hotPIN, 0);
  analogWrite(coldPIN, 0);

  if(!LittleFS.begin(true)){
    Serial.println("Wystapil blad podczas montowania dysku LittleFS");
    return;
  }
  Serial.println("Dysk LittleFS zamontowany pomyslnie.");

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

  server.serveStatic("/", LittleFS, "/index.html");
  server.serveStatic("/style.css", LittleFS, "/style.css");
  server.serveStatic("/script.js", LittleFS, "/script.js");

  server.onNotFound(handleDynamicPaths);

  server.begin();
  Serial.println("Serwer HTTP wystartowal!");
}

void loop() {
  server.handleClient();
}
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "mamaniamniam_5G";
const char* password = "bombo06klat";

const int coldPIN = 33;
const int hotPIN = 32;
String ColdState = "off";
String HotState = "off";

WebServer server(80);

void handleColdOn()
{
  ColdState = "on";
  digitalWrite(coldPIN, HIGH);
  handleRoot();
}

void handleColdOff()
{
  ColdState = "off";
  digitalWrite(coldPIN, LOW);
  handleRoot();
}

void handleHotOn()
{
  HotState = "on";
  digitalWrite(hotPIN, HIGH);
  handleRoot();
}

void handleHotOff()
{
  HotState = "off";
  digitalWrite(hotPIN, LOW);
  handleRoot();
}

void handleRoot()
{
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"; //meta name content... to zeby na wszystkich urzadzeniach wyswietlalo sie odpowiednio
  html += "<link rel=\"icon\" href=\"data:,\">"; //trik by komputer nie szukał ikony
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-allign: center;}";
  html += ".button1 { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #555555; border: none; color: black; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;} </style></head>";
  html += "<body><h1>ESP32 LED Controller</h1>";

  html += "<p> HOT LED - State " + HotState + "</p>";
  if(HotState == "off")
  {
    html += "<p><a href=\"/hot/on\"><button class=\"button1\">ON</button></a></p>";
  }
  else
  {
    html += "<p><a href=\"/hot/off\"><button class=\"button2\">OFF</button></a></p>";
  }

  html += "<p> COLD LED - State " + ColdState + "</p>";
  if(ColdState == "off")
  {
    html += "<p><a href=\"/cold/on\"><button class=\"button1\">ON</button></a></p>";
  }
  else
  {
    html += "<p><a href=\"/cold/off\"><button class=\"button2\">OFF</button></a></p>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleNotFound() 
{
  server.send(404, "text/plain", "Not found");
}

void setup() 
{
  Serial.begin(115200);

  pinMode(coldPIN, OUTPUT);
  pinMode(hotPIN, OUTPUT);

  digitalWrite(coldPIN, LOW);
  digitalWrite(hotPIN, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/hot/on", handleHotOn);
  server.on("/hot/off", handleHotOff);
  server.on("/cold/on", handleColdOn);
  server.on("/cold/off", handleColdOff);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

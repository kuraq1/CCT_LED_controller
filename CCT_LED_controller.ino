#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "mamaniamniam_5G";
const char* password = "bombo06klat";

const int coldPIN = 33;
const int hotPIN = 32;
String ColdState = "off";
String HotState = "off";

WebServer server(80);

// --- FUNKCJE STERUJĄCE (Nie ładują już całej strony!) ---

void handleColdOn() {
  ColdState = "on";
  digitalWrite(coldPIN, HIGH);
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "OK"); // Zwracamy tylko tekst "OK"
}

void handleColdOff() {
  ColdState = "off";
  digitalWrite(coldPIN, LOW);
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "OK");
}

void handleHotOn() {
  HotState = "on";
  digitalWrite(hotPIN, HIGH);
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "OK");
}

void handleHotOff() {
  HotState = "off";
  digitalWrite(hotPIN, LOW);
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "OK");
}

// Funkcja obsługująca braki ukrytych plików z telefonu (żeby nie muliło)
void handleNotFound() {
  server.sendHeader("Connection", "close");
  server.send(404, "text/plain", "Not found");
}

// --- GŁÓWNA STRONA ---

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".btn-on { background-color: #4CAF50; }";
  html += ".btn-off { background-color: #555555; }</style>";
  
  // Magia JavaScript (AJAX) - Wysyłanie w tle
  html += "<script>";
  html += "function sendCommand(url, elementId, newState) {";
  html += "  fetch(url).then(response => {"; // Wyślij żądanie do ESP32
  html += "    if(response.ok) {"; // Jeśli ESP32 odpowiedziało 200 (OK)
  html += "      document.getElementById(elementId).innerText = newState;"; // Zmień tekst stanu na stronie
  html += "    }";
  html += "  });";
  html += "}";
  html += "</script></head>";
  
  html += "<body><h1>ESP32 LED Controller</h1>";

  // Sekcja HOT LED
  // Używamy tagu <span> aby łatwo znaleźć to miejsce i podmienić tekst przez JavaScript
  html += "<p> HOT LED - State <span id=\"hot-state\">" + HotState + "</span></p>";
  html += "<p>";
  // Kiedy przycisk zostaje kliknięty, uruchamia funkcję sendCommand()
  html += "<button class=\"button btn-on\" onclick=\"sendCommand('/hot/on', 'hot-state', 'on')\">ON</button>";
  html += "<button class=\"button btn-off\" onclick=\"sendCommand('/hot/off', 'hot-state', 'off')\">OFF</button>";
  html += "</p>";

  // Sekcja COLD LED
  html += "<p> COLD LED - State <span id=\"cold-state\">" + ColdState + "</span></p>";
  html += "<p>";
  html += "<button class=\"button btn-on\" onclick=\"sendCommand('/cold/on', 'cold-state', 'on')\">ON</button>";
  html += "<button class=\"button btn-off\" onclick=\"sendCommand('/cold/off', 'cold-state', 'off')\">OFF</button>";
  html += "</p>";

  html += "</body></html>";
  server.sendHeader("Connection", "close"); // Rozłącz od razu
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  pinMode(coldPIN, OUTPUT);
  pinMode(hotPIN, OUTPUT);

  digitalWrite(coldPIN, LOW);
  digitalWrite(hotPIN, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  WiFi.setSleep(false); // WYŁĄCZ OSZCZĘDZANIE ENERGII WIFI - klu do szybkości na telefonach!

  while (WiFi.status() != WL_CONNECTED) {
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
  server.onNotFound(handleNotFound); // Rejestracja funkcji obsługi błędów 404

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
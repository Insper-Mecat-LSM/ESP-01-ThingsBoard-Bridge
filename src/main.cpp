/*
  Mensagens possíveis:
  1 - "100" - WiFi não conectado
  2 - "101" - WiFi conectado, mas JSON com erro
  3 - "200" - Conexão com o Thingsboard bem sucedida

  Outros mensagens serão Erros de HTTP, como 404, 500, etc.
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

const char *ssid = "RT-AC51U_24_2G";    // Substitua pela sua SSID
const char *password = "keyboard_4573"; // Substitua pela sua senha

#define serverName "http://demo.thingsboard.io/api/v1/"




void sendHTTPJson(String json, String serverID)
{
  WiFiClient client;
  HTTPClient http;
  String url = serverName + serverID + "/telemetry";

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(json);

  Serial.println(httpResponseCode);
  http.end();
}

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Serial.println("");
  // Serial.print("Connected to WiFi network with IP Address: ");
  // Serial.println(WiFi.localIP());
}
// {"ID":"vNhzKieKCPuVFbtcRRNI","data":{"temperature":12.5, "Hugo":"funfa", "Boleana":false}}


void loop()
{
  if (Serial.available())
  {

    if (WiFi.status() == WL_CONNECTED)
    {
      String Received = Serial.readStringUntil('\n').c_str();
      JSONVar JsonReceived = JSON.parse(Received);

      // Testa se o JSON foi recebido corretamente
      if (JSON.typeof(JsonReceived) == "undefined")
      {
        Serial.println("101 - JSON parse error");
        Serial.println("Received: " + Received);
        return;
      }

      // Testa se o JSON tem o campo "ID" e "data"
      if (JsonReceived.hasOwnProperty("ID") && JsonReceived.hasOwnProperty("data"))
      {
        sendHTTPJson(JSON.stringify(JsonReceived["data"]) , (String) JsonReceived["ID"]);
      }
    }
    else
    {
      Serial.println("100 - WiFi not connected");
      Serial.println("Trying to connect to:");
      Serial.print("SSID: ");
      Serial.println(ssid);
      Serial.print("Pass: ");
      Serial.println(password);
    }
  }
}

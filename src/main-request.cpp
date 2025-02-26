/**
 * @file main-request.cpp
 * @author alifeee (alifeee@alifeee.net)
 * @brief Sends Discord a POST request to post to a webhook
 * @version 0.1
 * @date 2024-04-17
 *
 * @copyright Use freely
 */
#include <Arduino.h>

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#include <ESP8266HTTPClient.h>
// #include <WiFiClient.h>
// #include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>

#include <secrets.h>

#define SWITCH_PIN D3 // GPIO 0

// #define JSON_MESSAGE "{\"content\": \"hello there\"}"
#define JSON_MESSAGE                                                           \
  "{\"content\":\"The space is now OPEN!\",\"username\":\"[occupancy "         \
  "button]\",\"avatar_url\":\"https://server.alifeee.net/static/shhm/"         \
  "logo.png\",\"embeds\":[{\"author\":{\"name\":\"SHHM\",\"url\":\"https://"   \
  "www.sheffieldhackspace.org.uk/\",\"icon_url\":\"https://"                   \
  "server.alifeee.net/static/shhm/logo.png\"},\"title\":\"the space is "       \
  "OPEN\",\"url\":\"https://www.sheffieldhackspace.org.uk/"                    \
  "\",\"description\":\"the space is now "                                     \
  "OPEN\",\"image\":{\"url\":\"https://www.sheffieldhackspace.org.uk/assets/"  \
  "images/woodturning-header.jpg\"}}]}"

void setup() {
  // SETUP BUTTON
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // SETUP LED
  pinMode(LED_BUILTIN, OUTPUT);

  // SETUP SERIAL
  Serial.begin(9600);
  while (!Serial) {
    delay(100);
  }
  Serial.print("Set up!");

  // SETUP wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // turn light off
  digitalWrite(LED_BUILTIN, HIGH);
  int buttonState = digitalRead(SWITCH_PIN);
  // Check WiFi connection and reconnect if needed
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  } else if (buttonState == 1) {
    // button not pressed, skip
  } else {
    // turn light on while request runs
    digitalWrite(LED_BUILTIN, LOW);
    // copied from
    // https://randomnerdtutorials.com/esp8266-nodemcu-https-requests/

    std::unique_ptr<BearSSL::WiFiClientSecure> client(
        new BearSSL::WiFiClientSecure);
    // Ignore SSL certificate validation
    //  technically anyone could be pretending to be who we want, but we don't
    //  really mind
    client->setInsecure();
    // create an HTTPClient instance
    HTTPClient https;

    https.setReuse(true);
    https.setTimeout(6000);
    https.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    Serial.printf("POST TO %s", FILE_URL);
    Serial.println();
    Serial.printf("DATA %s", JSON_MESSAGE);
    Serial.println();

    // Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");

    // test SSL with "https://www.howsmyssl.com/a/check"
    if (https.begin(*client, FILE_URL)) { // HTTPS
      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header
      https.addHeader("Content-Type", "application/json");
      int httpCode = https.POST(JSON_MESSAGE);
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
        // file found at server
        // if (httpCode == HTTP_CODE_OK ||
        // httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);
        // }
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n",
                      https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}

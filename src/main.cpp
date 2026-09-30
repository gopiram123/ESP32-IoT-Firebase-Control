#include <Arduino.h>


#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Firebase
#define API_KEY ""
#define DATABASE_URL ""

// Firebase Authentication
#define USER_EMAIL ""
#define USER_PASSWORD ""

// LED pin
#define LED_PIN 2

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase starting...");
}

void loop()
{
  if (Firebase.ready())
  {
    // Read LED state from Firebase
    if (Firebase.RTDB.getInt(&fbdo, "/LED/state"))
    {
      int state = fbdo.intData();

      Serial.print("Firebase LED State: ");
      Serial.println(state);

      if (state == 1)
      {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON");
      }
      else
      {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF");
      }
    }
    else
    {
      Serial.print("Firebase Error: ");
      Serial.println(fbdo.errorReason());
    }
  }

  delay(500);
}
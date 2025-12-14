#include <ESP8266WiFi.h>
#include <espnow.h>

// Definice pinů tlačítek
#define BUTTON1_PIN 14  // D5
#define BUTTON2_PIN 12  // D6
#define BUTTON3_PIN 13  // D7
#define BUTTON4_PIN 4  // D8

// MAC adresa příjemce
uint8_t broadcastAddress[] = {0xEC, 0xFA, 0xBC, 0x42, 0x3D, 0x20};

// Struktura odesílaných dat
typedef struct struct_message {
  bool button1;
  bool button2;
  bool button3;
  bool button4;
} struct_message;

struct_message myData;

// Callback po odeslání
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Odesláno: ");
  Serial.println(sendStatus == 0 ? "OK" : "CHYBA");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(BUTTON4_PIN, INPUT_PULLUP);

  if (esp_now_init() != 0) {
    Serial.println("Chyba inicializace ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("Sender se 4 tlačítky připraven");
}

void loop() {
  // Čtení tlačítek (aktivní v LOW)
  myData.button1 = !digitalRead(BUTTON1_PIN);
  myData.button2 = !digitalRead(BUTTON2_PIN);
  myData.button3 = !digitalRead(BUTTON3_PIN);
  myData.button4 = !digitalRead(BUTTON4_PIN);

  // Odeslání dat
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  // Debug výpis
  Serial.print("B1: "); Serial.print(myData.button1);
  Serial.print(" | B2: "); Serial.print(myData.button2);
  Serial.print(" | B3: "); Serial.print(myData.button3);
  Serial.print(" | B4: "); Serial.println(myData.button4);

  delay(100);
}

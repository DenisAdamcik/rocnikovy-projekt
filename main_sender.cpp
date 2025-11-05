#include <ESP8266WiFi.h>
#include <espnow.h>
#define BUTTON1_PIN 5  // D1
#define BUTTON2_PIN 12  // D5
#define BUTTON3_PIN 14  // D6
uint8_t broadcastAddress[] = {0xEC, 0xFA, 0xBC, 0x42, 0x3D, 0x20};

typedef struct struct_message {
  int a0Value;      // hodnota joysticku
  bool button1;     // tlačítko D1
  bool button2;     // tlačítko D2
  bool button3;     // tlačítko D3
} struct_message;

struct_message myData;

// Callback po odeslání
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Odeslano: ");
  Serial.println(sendStatus == 0 ? "OK" : "CHYBA");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);

  if (esp_now_init() != 0) {
    Serial.println("Chyba inicializace ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("Joystick + tlacitka sender pripraven");
}

void loop() {

  myData.a0Value = analogRead(A0);

  myData.button1 = !digitalRead(BUTTON1_PIN);
  myData.button2 = !digitalRead(BUTTON2_PIN);
  myData.button3 = !digitalRead(BUTTON3_PIN);

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  Serial.print("A0: "); Serial.print(myData.a0Value);
  Serial.print(" | B1: "); Serial.print(myData.button1);
  Serial.print(" | B2: "); Serial.print(myData.button2);
  Serial.print(" | B3: "); Serial.println(myData.button3);

  delay(100);  // 10x za sekundu
}

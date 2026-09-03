#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

// Diagnostic wiring used by this project.
// Both ESP32 boards use the same SPI pins.
static constexpr uint8_t NRF_CE = 4;
static constexpr uint8_t NRF_CSN = 5;
static constexpr uint8_t NRF_SCK = 18;
static constexpr uint8_t NRF_MISO = 19;
static constexpr uint8_t NRF_MOSI = 23;
static constexpr uint8_t BUZZER_PIN = 25;

RF24 radio(NRF_CE, NRF_CSN);
const byte ADDRESS[6] = "ARF01";

static constexpr uint8_t RF_CHANNEL = 76;
static constexpr uint16_t LINK_TIMEOUT_MS = 3000;

struct TestPacket {
  uint32_t sequence;
  uint32_t uptimeMs;
};

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  delay(1000);

  Serial.println();
  Serial.println("=== Adaptive RF - RX Diagnostic ===");
  Serial.println("Pins: CE=4 CSN=5 SCK=18 MISO=19 MOSI=23 Buzzer=25");

  SPI.begin(NRF_SCK, NRF_MISO, NRF_MOSI, NRF_CSN);

  if (!radio.begin()) {
    Serial.println("FAIL: nRF24L01 initialization failed.");
    Serial.println("Check 3.3V, GND, SPI, CE and CSN wiring.");
    while (true) {
      tone(BUZZER_PIN, 1200, 150);
      delay(1000);
      Serial.println("Waiting for nRF24 hardware...");
    }
  }

  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(true);
  radio.openWritingPipe(ADDRESS);
  radio.openReadingPipe(1, ADDRESS);
  radio.startListening();

  Serial.println("PASS: nRF24L01 initialized.");
  Serial.println("RX test started. Waiting for packets...");
}

void loop() {
  static uint32_t lastPacketMs = millis();
  static uint32_t received = 0;
  static bool timeoutReported = false;

  if (radio.available()) {
    TestPacket packet{};
    radio.read(&packet, sizeof(packet));
    lastPacketMs = millis();
    received++;
    timeoutReported = false;

    noTone(BUZZER_PIN);

    Serial.print("RX seq=");
    Serial.print(packet.sequence);
    Serial.print(" uptime=");
    Serial.print(packet.uptimeMs);
    Serial.print(" received=");
    Serial.println(received);
  }

  if (millis() - lastPacketMs > LINK_TIMEOUT_MS) {
    if (!timeoutReported) {
      Serial.println("WARN: no packet received for 3 seconds.");
      tone(BUZZER_PIN, 1800, 150);
      timeoutReported = true;
    }
  }

  delay(2);
}

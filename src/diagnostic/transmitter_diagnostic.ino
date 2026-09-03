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

RF24 radio(NRF_CE, NRF_CSN);
const byte ADDRESS[6] = "ARF01";

static constexpr uint8_t RF_CHANNEL = 76;
static constexpr uint16_t PACKET_INTERVAL_MS = 1000;

struct TestPacket {
  uint32_t sequence;
  uint32_t uptimeMs;
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("=== Adaptive RF - TX Diagnostic ===");
  Serial.println("Pins: CE=4 CSN=5 SCK=18 MISO=19 MOSI=23");

  SPI.begin(NRF_SCK, NRF_MISO, NRF_MOSI, NRF_CSN);

  if (!radio.begin()) {
    Serial.println("FAIL: nRF24L01 initialization failed.");
    Serial.println("Check 3.3V, GND, SPI, CE and CSN wiring.");
    while (true) {
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
  radio.stopListening();

  Serial.println("PASS: nRF24L01 initialized.");
  Serial.println("TX test started. Waiting for ACK from receiver...");
}

void loop() {
  static uint32_t sequence = 0;

  TestPacket packet{sequence++, millis()};
  bool ok = radio.write(&packet, sizeof(packet));

  Serial.print("TX seq=");
  Serial.print(packet.sequence);
  Serial.print(" result=");
  Serial.println(ok ? "ACK_OK" : "NO_ACK");

  delay(PACKET_INTERVAL_MS);
}

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

// ESP32 + nRF24L01 default wiring used by this reference firmware.
// Verify these pins against the physical prototype before uploading.
static constexpr uint8_t NRF_CE = 4;
static constexpr uint8_t NRF_CSN = 5;
static constexpr uint8_t NRF_SCK = 18;
static constexpr uint8_t NRF_MISO = 19;
static constexpr uint8_t NRF_MOSI = 23;
static constexpr uint8_t BUZZER_PIN = 25;

RF24 radio(NRF_CE, NRF_CSN);
const byte ADDRESS[6] = "ARF01";

static constexpr uint8_t RF_CHANNEL = 76;
static constexpr uint16_t LINK_TIMEOUT_MS = 500;

struct Packet {
  uint32_t sequence;
  uint8_t type;             // 0=data, 1=rate proposal
  uint8_t requestedRate;    // 0=250 kbps, 1=1 Mbps, 2=2 Mbps
  uint8_t retryProfile;
  uint32_t uptimeMs;
};

const rf24_datarate_e DATA_RATES[] = {
  RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
};

uint8_t currentRateIndex = 1;
uint32_t lastPacketMs = 0;
uint32_t receivedPackets = 0;
uint32_t lastSequence = 0;
bool linkAlive = false;

const char *rateName(uint8_t index) {
  switch (index) {
    case 0: return "250kbps";
    case 1: return "1Mbps";
    case 2: return "2Mbps";
    default: return "unknown";
  }
}

void applyRadioRate(uint8_t index) {
  if (index > 2) return;
  if (!radio.setDataRate(DATA_RATES[index])) {
    Serial.println("WARN: requested data rate was not accepted");
    return;
  }
  currentRateIndex = index;
  Serial.print("RF data rate -> ");
  Serial.println(rateName(index));
}

void indicateLink(bool active) {
  if (active == linkAlive) return;
  linkAlive = active;
  if (active) {
    noTone(BUZZER_PIN);
    Serial.println("LINK: active");
  } else {
    tone(BUZZER_PIN, 1800, 150);
    Serial.println("LINK: lost / timeout");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  delay(500);

  SPI.begin(NRF_SCK, NRF_MISO, NRF_MOSI, NRF_CSN);

  if (!radio.begin()) {
    Serial.println("ERROR: nRF24L01 not detected. Check power, SPI and CE/CSN wiring.");
    while (true) {
      tone(BUZZER_PIN, 1200, 100);
      delay(1000);
    }
  }

  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(true);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(ADDRESS);
  radio.openReadingPipe(1, ADDRESS);
  applyRadioRate(currentRateIndex);
  radio.startListening();

  lastPacketMs = millis();
  Serial.println("Adaptive RF receiver ready");
}

void loop() {
  if (radio.available()) {
    Packet packet{};
    radio.read(&packet, sizeof(packet));
    lastPacketMs = millis();
    receivedPackets++;
    indicateLink(true);

    Serial.print("RX seq=");
    Serial.print(packet.sequence);
    Serial.print(" rate=");
    Serial.print(rateName(packet.requestedRate));
    Serial.print(" retryProfile=");
    Serial.println(packet.retryProfile);

    if (packet.type == 1 && packet.requestedRate <= 2) {
      // The proposal was received over the currently valid link. Consume the
      // packet first, then switch so the transmitter can follow shortly after.
      delay(15);
      applyRadioRate(packet.requestedRate);
    }

    lastSequence = packet.sequence;
  }

  if (millis() - lastPacketMs > LINK_TIMEOUT_MS) {
    indicateLink(false);
  }

  delay(2);
}

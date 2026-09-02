#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

// ESP32 + nRF24L01 default wiring used by this firmware.
// Verify these pins against the physical prototype before uploading.
static constexpr uint8_t NRF_CE = 4;
static constexpr uint8_t NRF_CSN = 5;
static constexpr uint8_t NRF_SCK = 18;
static constexpr uint8_t NRF_MISO = 19;
static constexpr uint8_t NRF_MOSI = 23;

RF24 radio(NRF_CE, NRF_CSN);
const byte ADDRESS[6] = "ARF01";

static constexpr uint8_t RF_CHANNEL = 76;
static constexpr uint16_t PACKET_INTERVAL_MS = 100;
static constexpr uint8_t FAILURE_THRESHOLD = 3;
static constexpr uint16_t RECOVERY_THRESHOLD = 20;

struct RetryProfile { uint8_t delay; uint8_t count; };
const RetryProfile RETRY_PROFILES[] = {
  {2, 3}, {4, 7}, {6, 10}, {8, 15}
};

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
uint8_t retryProfileIndex = 0;
uint8_t consecutiveFailures = 0;
uint16_t consecutiveSuccesses = 0;
uint32_t sequenceNumber = 0;

void applyRadioRate(uint8_t index) {
  if (index > 2) return;
  if (!radio.setDataRate(DATA_RATES[index])) {
    Serial.println("WARN: requested data rate was not accepted");
    return;
  }
  currentRateIndex = index;
  Serial.print("RF data rate -> ");
  Serial.println(index == 0 ? "250 kbps" : (index == 1 ? "1 Mbps" : "2 Mbps"));
}

void applyRetryProfile(uint8_t index) {
  const uint8_t maxIndex = sizeof(RETRY_PROFILES) / sizeof(RETRY_PROFILES[0]);
  if (index >= maxIndex) index = maxIndex - 1;
  retryProfileIndex = index;
  radio.setRetries(RETRY_PROFILES[index].delay, RETRY_PROFILES[index].count);
  Serial.print("Retry profile -> ");
  Serial.println(index);
}

bool sendPacket(const Packet &packet) {
  radio.stopListening();
  return radio.write(&packet, sizeof(packet));
}

void setup() {
  Serial.begin(115200);
  delay(500);
  SPI.begin(NRF_SCK, NRF_MISO, NRF_MOSI, NRF_CSN);

  if (!radio.begin()) {
    Serial.println("ERROR: nRF24L01 not detected. Check power, SPI and CE/CSN wiring.");
    while (true) delay(1000);
  }

  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(true);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(ADDRESS);
  radio.openReadingPipe(1, ADDRESS);
  applyRadioRate(currentRateIndex);
  applyRetryProfile(retryProfileIndex);
  radio.stopListening();

  Serial.println("Adaptive RF transmitter ready");
}

void loop() {
  Packet packet{};
  packet.sequence = sequenceNumber++;
  packet.type = 0;
  packet.requestedRate = currentRateIndex;
  packet.retryProfile = retryProfileIndex;
  packet.uptimeMs = millis();

  const bool delivered = sendPacket(packet);

  if (delivered) {
    consecutiveFailures = 0;
    consecutiveSuccesses++;

    if (consecutiveSuccesses >= RECOVERY_THRESHOLD && currentRateIndex < 2) {
      Packet proposal = packet;
      proposal.type = 1;
      proposal.requestedRate = currentRateIndex + 1;

      if (sendPacket(proposal)) {
        // Receiver changes after reading this packet; then transmitter follows.
        delay(25);
        applyRadioRate(currentRateIndex + 1);
        consecutiveSuccesses = 0;
      }
    }
  } else {
    consecutiveSuccesses = 0;
    consecutiveFailures++;

    if (consecutiveFailures >= FAILURE_THRESHOLD) {
      if (retryProfileIndex < 3) {
        applyRetryProfile(retryProfileIndex + 1);
      }

      // Rate changes are proposed and acknowledged at the current rate so that
      // the two radios do not switch independently and lose synchronization.
      if (currentRateIndex > 0) {
        Packet proposal = packet;
        proposal.type = 1;
        proposal.requestedRate = currentRateIndex - 1;
        if (sendPacket(proposal)) {
          delay(25);
          applyRadioRate(currentRateIndex - 1);
        }
      }
      consecutiveFailures = 0;
    }
  }

  delay(PACKET_INTERVAL_MS);
}

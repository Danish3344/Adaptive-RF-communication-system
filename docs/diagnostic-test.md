# RF Diagnostic Test

This diagnostic stage isolates the nRF24L01 link before the adaptive communication firmware is tested.

## Wiring

Both ESP32 boards use:

| nRF24L01 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| CE | GPIO 4 |
| CSN | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| IRQ | Not connected |

Receiver buzzer:

| Buzzer | ESP32 |
|---|---|
| + | GPIO 25 |
| - | GND |

## Test procedure

1. Install the **RF24** library in Arduino IDE.
2. Upload `src/diagnostic/transmitter_diagnostic.ino` to ESP32 #1.
3. Upload `src/diagnostic/receiver_diagnostic.ino` to ESP32 #2.
4. Open both Serial Monitors at **115200 baud**.
5. Power both boards and keep the nRF24 antennas separated by a short distance during the first test.
6. Check that both boards report `PASS: nRF24L01 initialized.`
7. On the transmitter, look for `ACK_OK`.
8. On the receiver, look for increasing `RX seq=` values.

## Expected result

A healthy basic link should show:

- Transmitter: `TX seq=... result=ACK_OK`
- Receiver: `RX seq=... received=...`
- Receiver buzzer: normally silent while packets arrive.

## Failure interpretation

### Initialization failure
If either board reports `nRF24L01 initialization failed`, stop there and check:

- 3.3 V supply
- common ground
- CE/CSN wiring
- SCK/MISO/MOSI wiring
- loose breadboard/jumper connections

### `NO_ACK` on transmitter
If initialization passes but the transmitter repeatedly reports `NO_ACK`, check the receiver power, receiver serial output, antenna/module connections, and matching RF configuration.

### Receiver sees no packets
If the transmitter reports `NO_ACK` and the receiver never prints `RX seq=`, the basic RF link has not been established. Do not proceed to adaptive testing yet.

## Important

This test intentionally does **not** use the adaptive rate/retry algorithm. It is a hardware and basic-link validation stage. Actual RF performance should only be documented after repeatable physical measurements are collected.

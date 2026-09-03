# Adaptive RF Communication System

A two-node wireless communication prototype built with **ESP32** microcontrollers and **nRF24L01 PA/LNA** 2.4 GHz transceiver modules. The project explores adaptive communication techniques for maintaining reliable packet exchange when RF-link conditions change.

## Project Objective

The system is designed as a feedback loop:

**Transmit → Receive → Measure → Detect degradation → Adapt → Continue**

The transmitter sends numbered packets and uses acknowledgement success/failure as a practical link-health indicator. When repeated failures occur, the reference firmware increases the retry profile and can negotiate a lower RF data rate. During sustained successful communication, it can negotiate a higher data rate.

> **Important:** The firmware in this repository is a **reference implementation developed for this repository**. It is not claimed to be a byte-for-byte recovery of the original prototype firmware. The physical prototype previously produced unexpected test behaviour, so final hardware performance is not claimed until the diagnostic stage passes repeatably.

## Hardware

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 development board | 2 | Wireless node controllers |
| nRF24L01 PA/LNA transceiver | 2 | 2.4 GHz RF communication |
| Buzzer | 1 | Receiver status indication |
| Breadboard / jumper wires | As required | Prototyping |

## System Architecture

```text
                 2.4 GHz RF Link
       ┌──────────────────────────────┐
       │                              │
┌──────▼──────┐                  ┌────▼───────┐
│   ESP32 A   │                  │  ESP32 B   │
│ Transmitter │                  │  Receiver  │
└──────┬──────┘                  └────┬───────┘
       │ SPI                          │ SPI
┌──────▼──────┐                  ┌────▼───────┐
│ nRF24L01 A  │◄────────────────►│ nRF24L01 B │
└─────────────┘                  └────┬───────┘
                                      │ GPIO 25
                                 ┌────▼────┐
                                 │ Buzzer  │
                                 └─────────┘
```

## Reference Wiring

Both ESP32 nodes use the same nRF24L01 wiring:

| nRF24L01 | ESP32 |
|---|---|
| VCC | 3.3 V |
| GND | GND |
| CE | GPIO 4 |
| CSN | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| IRQ | Not connected |

Receiver buzzer: **GPIO 25 → buzzer signal/positive, buzzer negative → GND**.

See [`docs/hardware.md`](docs/hardware.md) for the complete wiring checklist.

## Adaptive Behaviour

The reference implementation adapts two communication parameters:

1. **Retry profile** — progressively increases the nRF24L01 retry delay/count after repeated packet failures.
2. **RF data rate** — proposes a lower data rate after repeated failures and a higher rate after sustained successful communication.

The data-rate transition is coordinated through a proposal packet so the two radios do not independently switch configuration.

These are implementation details of the repository's reference firmware and should not be presented as measured characteristics of the physical prototype until tested.

## Diagnostic-First Development

Because the previous physical test produced unexpected Serial Monitor behaviour and continuous buzzer activity, the project now uses a staged validation process:

**Stage 1 — Hardware initialization**  
Confirm both nRF24L01 modules initialize correctly.

**Stage 2 — Basic RF link**  
Confirm the transmitter receives ACKs and the receiver prints increasing packet sequence numbers.

**Stage 3 — Status indication**  
Confirm the receiver buzzer is silent during a healthy link and only gives a short indication after a timeout.

**Stage 4 — Adaptive firmware**  
Only after the basic RF link is stable, test adaptive retry and data-rate behaviour.

Diagnostic firmware is in `src/diagnostic/` and the procedure is documented in [`docs/diagnostic-test.md`](docs/diagnostic-test.md).

## Firmware

The project is written in Arduino-compatible C++ for ESP32 using the **RF24** library.

### Reference configuration

- RF channel: 76
- CRC: 16-bit
- Auto-acknowledgement: enabled
- Dynamic payloads: enabled in adaptive firmware
- Initial data rate: 1 Mbps
- Initial retry profile: delay 2 / count 3
- Adaptive packet interval: 100 ms
- Adaptive receiver link timeout: 500 ms
- Diagnostic packet interval: 1000 ms
- Diagnostic receiver timeout: 3000 ms

## Repository Structure

```text
Adaptive-RF-communication-system/
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   ├── transmitter/
│   │   └── transmitter.ino
│   ├── receiver/
│   │   └── receiver.ino
│   └── diagnostic/
│       ├── transmitter_diagnostic.ino
│       └── receiver_diagnostic.ino
└── docs/
    ├── system-architecture.md
    ├── hardware.md
    ├── communication-protocol.md
    ├── adaptive-algorithm.md
    └── diagnostic-test.md
```

## Setup

1. Install ESP32 board support in Arduino IDE.
2. Install the **RF24** library.
3. Rebuild the hardware using the pin table in `docs/hardware.md`.
4. Verify both nRF24 modules use 3.3 V and share ground with their ESP32.
5. Upload the diagnostic transmitter to Node A.
6. Upload the diagnostic receiver to Node B.
7. Open both Serial Monitor windows at **115200 baud**.
8. Confirm `PASS: nRF24L01 initialized.` on both boards.
9. Confirm transmitter `ACK_OK` and receiver `RX seq=` output.
10. Only then upload the adaptive transmitter/receiver firmware.

## Testing Plan

Physical validation should be performed before claiming final system performance.

Suggested experiments:

- Baseline test at short range.
- Increase distance in controlled steps.
- Introduce obstacles between nodes.
- Repeat tests under different RF environments.
- Record packet delivery success/failure and adaptation events.
- Measure recovery behaviour after the link degrades.
- Compare fixed-rate operation against adaptive operation.

Recommended metrics include packet delivery ratio, consecutive failures, recovery time, selected data rate, retry profile and throughput/latency where measurable.

## Development Status

| Area | Status |
|---|---|
| Repository structure | ✅ Complete |
| Documentation baseline | ✅ Complete |
| Reference transmitter firmware | ✅ Complete |
| Reference receiver firmware | ✅ Complete |
| Diagnostic firmware | ✅ Complete |
| Fixed reference GPIO wiring | ✅ Defined in firmware/docs |
| Previous physical prototype test | ⚠️ Unexpected output observed |
| Basic RF diagnostic validation | ⏳ Pending next hardware assembly |
| Adaptive hardware validation | ⏳ Blocked until diagnostic passes |
| Measured RF performance | ⏳ Requires repeatable experiment |
| Final results/plots | ⏳ Requires measured data |

## Future Improvements

- Add structured CSV/serial logging for experiments.
- Add packet delivery and recovery statistics.
- Compare adaptive and fixed configurations quantitatively.
- Add repeatable test procedures and plots.
- Document verified hardware revisions.
- Add watchdog/fault-recovery behaviour where appropriate.
- Evaluate additional adaptation strategies such as channel selection after controlled testing.

## Author

**Danish**  
Embedded Systems / RF Communication Project

## License

This project is released under the MIT License. See `LICENSE` for details.

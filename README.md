# Adaptive RF Communication System

A two-node wireless communication prototype built with **ESP32** microcontrollers and **nRF24L01** 2.4 GHz transceiver modules. The project explores adaptive communication techniques for maintaining reliable packet exchange when RF-link conditions change.

## Project Objective

The system is designed as a feedback loop:

**Transmit → Receive → Measure → Detect degradation → Adapt → Continue**

The transmitter sends numbered packets and uses acknowledgement success/failure as a practical link-health indicator. When repeated failures occur, the reference firmware increases the retry profile and can negotiate a lower RF data rate. During sustained successful communication, it can negotiate a higher data rate.

> **Important:** The firmware in this repository is a **reference implementation developed for this repository**. It is not claimed to be a byte-for-byte recovery of the original prototype firmware. GPIO assignments and performance results remain subject to physical verification.

## Hardware

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 development board | 2 | Wireless node controllers |
| nRF24L01 transceiver | 2 | 2.4 GHz RF communication |
| Buzzer | 1 | Local communication/status indication |
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
                                      │ GPIO
                                 ┌────▼────┐
                                 │ Buzzer  │
                                 └─────────┘
```

## Adaptive Behaviour

The reference implementation adapts two communication parameters:

1. **Retry profile** — progressively increases the nRF24L01 retry delay/count after repeated packet failures.
2. **RF data rate** — proposes a lower data rate after repeated failures and a higher rate after sustained successful communication.

The data-rate transition is coordinated through a proposal packet so the two radios do not independently switch configuration.

These are implementation details of the repository's reference firmware and should not be presented as measured characteristics of the physical prototype until tested.

## Firmware

The project is written in Arduino-compatible C++ for ESP32 using the **RF24** library.

### Reference configuration

- RF channel: 76
- CRC: 16-bit
- Auto-acknowledgement: enabled
- Dynamic payloads: enabled
- Initial data rate: 1 Mbps
- Initial retry profile: delay 2 / count 3
- Packet interval: 100 ms
- Receiver link timeout: 500 ms

The exact GPIO mapping is intentionally marked provisional until it is checked against the physical wiring.

## Repository Structure

```text
Adaptive-RF-communication-system/
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   ├── transmitter/
│   │   └── transmitter.ino
│   └── receiver/
│       └── receiver.ino
└── docs/
    ├── system-architecture.md
    ├── hardware.md
    ├── communication-protocol.md
    └── adaptive-algorithm.md
```

## Setup

1. Install ESP32 board support in Arduino IDE.
2. Install the **RF24** library.
3. Verify the nRF24L01 power supply and common ground.
4. Verify CE, CSN and SPI wiring against the physical prototype.
5. Upload `src/transmitter/transmitter.ino` to Node A.
6. Upload `src/receiver/receiver.ino` to Node B.
7. Open both Serial Monitor windows at **115200 baud**.
8. Observe packet sequence numbers, RF rate, retry profile and link state.

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
| ESP32 transmitter firmware | ✅ Reference implementation |
| ESP32 receiver firmware | ✅ Reference implementation |
| Adaptive retry/rate logic | ✅ Implemented in reference firmware |
| Exact physical GPIO verification | ⏳ Requires prototype check |
| Hardware integration test | ⏳ Requires physical test |
| Measured RF performance | ⏳ Requires experiment |
| Final results/plots | ⏳ Requires measured data |

The remaining items cannot be honestly marked complete without access to the physical prototype and measured test results.

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

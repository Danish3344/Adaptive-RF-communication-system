# Adaptive RF Communication System

A two-node wireless communication system built with **ESP32** microcontrollers and **nRF24L01** 2.4 GHz transceiver modules. The project explores adaptive communication techniques for maintaining reliable data exchange when RF link conditions change.

## Project Overview

Wireless links do not always operate under stable conditions. Distance, interference, obstacles, power conditions, and channel activity can reduce communication reliability.

This project is designed around two ESP32-based RF nodes:

- **Node A — Transmitter:** generates and sends application data.
- **Node B — Receiver:** receives packets, evaluates communication status, and provides local feedback through a buzzer.
- **nRF24L01:** provides the RF data link between the two nodes.

The long-term objective is to make the communication layer respond to deteriorating link conditions instead of relying on a fixed configuration.

> **Implementation note:** This repository documents the actual build incrementally. Adaptive parameters and measured performance will only be documented after they are verified on the hardware.

## Hardware

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 development board | 2 | Wireless node controllers |
| nRF24L01 transceiver | 2 | 2.4 GHz RF communication |
| Buzzer | 1 | Local communication/status indication |
| Jumper wires / breadboard | As required | Prototyping and connections |

## System Architecture

```text
                 RF Link
       ┌─────────────────────────┐
       │                         │
┌──────▼──────┐             ┌────▼───────┐
│   ESP32 A   │             │  ESP32 B   │
│ Transmitter │             │  Receiver  │
└──────┬──────┘             └────┬───────┘
       │                         │
┌──────▼──────┐             ┌────▼───────┐
│ nRF24L01 A  │◄───────────►│ nRF24L01 B │
└─────────────┘             └────┬───────┘
                                  │
                             ┌────▼────┐
                             │ Buzzer  │
                             └─────────┘
```

## Core Engineering Concepts

- SPI communication between ESP32 and nRF24L01
- Packet-based wireless communication
- Transmit/receive acknowledgement
- Link-quality observation
- Fault detection and recovery
- Adaptive RF communication strategy
- Embedded C/C++ development using Arduino IDE

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

## Development Status

**Stage 1 — Repository and system documentation:** In progress

**Stage 2 — Verified transmitter and receiver firmware:** Pending hardware verification

**Stage 3 — Adaptive communication logic:** Pending verification of the implemented adaptation mechanism

**Stage 4 — Experimental measurements and results:** Pending testing

## Getting Started

The firmware will be developed for the ESP32 using the Arduino IDE and an nRF24L01-compatible RF library.

Before uploading firmware, verify:

1. ESP32 board support is installed in Arduino IDE.
2. The nRF24L01 modules are powered correctly.
3. SPI and control-pin wiring matches the documented hardware configuration.
4. Both nodes use compatible RF settings.
5. The transmitter and receiver are programmed with their respective firmware.

## Testing Approach

The system will be evaluated by changing the RF link conditions and observing communication behaviour. Relevant measurements may include packet delivery, acknowledgement success, response/recovery behaviour, and the selected adaptive state.

No performance figures are claimed in this README until they are measured on the physical prototype.

## Future Development

- Complete verified transmitter and receiver firmware
- Implement and document the adaptive decision mechanism
- Add structured packet and status reporting
- Record repeatable RF-link experiments
- Add measured performance plots/results
- Improve fault recovery and diagnostics
- Document hardware revisions and test conditions

## Author

**Danish**

Embedded Systems / RF Communication Project

---

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.

# System Architecture

## Purpose

The system uses two ESP32 nodes connected through nRF24L01 transceivers. The communication layer monitors packet delivery and adapts RF behaviour when the link becomes unreliable.

## Nodes

### Node A — Transmitter

- Generates sequential application packets.
- Sends packets using nRF24L01 auto-acknowledgement.
- Tracks consecutive transmission failures and successful transmissions.
- Increases the retry profile after repeated failures.
- Proposes a lower RF data rate when the link degrades.
- Can propose a higher data rate after sustained successful communication.

### Node B — Receiver

- Listens for packets from the transmitter.
- Records received sequence numbers and packet information.
- Detects a communication timeout.
- Provides buzzer feedback when the link is lost.
- Applies a new data rate when a rate-change proposal is received.

## Communication Flow

```text
Transmitter
    |
    |  Data packet + nRF24L01 ACK
    v
Receiver
    |
    |  Link status / rate proposal handling
    v
Adaptive decision
    |
    +---- repeated failures ----> stronger retry profile
    |
    +---- repeated failures ----> lower data rate proposal
    |
    +---- sustained success ----> higher data rate proposal
```

## Adaptive State Machine

```text
                 sustained success
              +----------------------+
              |                      |
              v                      |
        [Higher Rate] <--------- [Stable Link]
              ^                      |
              |                      |
              +----------------------+
                     repeated failures
                           |
                           v
                    [Lower Rate]
                           |
                           v
                  [Recovery / Retry]
```

The firmware currently uses a configurable three-level data-rate set (250 kbps, 1 Mbps, and 2 Mbps) and four retry profiles. These are implementation parameters, not measured performance results.

## Design Principle

The two radios must not independently change their RF data rate. A rate transition is therefore sent as a proposal over the currently working configuration, and the receiver applies the proposed rate before the transmitter follows it.

## Hardware Interface

The firmware uses the ESP32 hardware SPI peripheral for the nRF24L01 interface. CE and CSN are controlled by GPIO pins. The receiver also drives a buzzer from a GPIO output.

**Important:** the pin definitions in the firmware are a baseline and must be checked against the physical prototype before deployment. The project's original circuit photographs should be treated as the source of truth for final wiring documentation.

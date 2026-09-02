# System Architecture

## 1. Purpose

The Adaptive RF Communication System is a two-node embedded wireless prototype intended to study reliable packet communication under changing RF-link conditions.

## 2. Node Architecture

### Node A — Transmitter

- ESP32 microcontroller
- nRF24L01 RF transceiver
- Application/data generation
- Packet transmission
- Communication-status handling

### Node B — Receiver

- ESP32 microcontroller
- nRF24L01 RF transceiver
- Packet reception
- Communication-status evaluation
- Buzzer-based local indication

## 3. Communication Flow

```text
Application data
      |
      v
   ESP32 A
      |
      | SPI
      v
 nRF24L01 A
      |
      | 2.4 GHz RF link
      v
 nRF24L01 B
      |
      | SPI
      v
   ESP32 B
      |
      +----> Status / fault indication
      |
      +----> Buzzer
```

## 4. Adaptive Layer

The adaptive layer is intended to observe communication behaviour and respond when the RF link becomes unreliable.

Examples of observable indicators include acknowledgement success/failure, packet delivery behaviour, retries, and communication timeouts. The exact adaptive parameters used by the physical prototype will be recorded here after firmware and hardware verification.

## 5. Design Principle

The system separates three concerns:

1. **RF transport** — nRF24L01 packet communication.
2. **Control and decision making** — ESP32 firmware.
3. **User feedback** — buzzer/status indication.

This separation makes the prototype easier to test, modify, and extend.

# Communication Protocol

## Current Prototype Model

The system uses the nRF24L01 packet-based radio interface between two ESP32 nodes.

```text
Transmitter                         Receiver
-----------                         --------
ESP32 A                             ESP32 B
   |                                   ^
   v                                   |
nRF24L01 A  =====  RF packets  =====  nRF24L01 B
   |                                   |
   +---- acknowledgement / status ----+
```

## Packet Design

The final application packet format will be defined together with the verified firmware. A structured packet should provide enough information for the receiver to distinguish valid application data from communication/status information.

Potential fields include:

| Field | Purpose |
|---|---|
| Sequence number | Detect missing or repeated packets |
| Payload | Application data |
| Status/flags | Communicate node state |
| Optional diagnostic data | Support RF-link experiments |

Only fields actually implemented in the firmware will be listed as part of the final protocol specification.

## Reliability Observation

The receiver/transmitter pair can use communication events such as successful acknowledgements, failed transmissions, retries, and timeouts as indicators of link behaviour.

These indicators form the basis for the adaptive-control layer.

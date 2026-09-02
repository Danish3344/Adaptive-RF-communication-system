# Communication Protocol

## Radio Configuration

The current firmware uses nRF24L01 Enhanced ShockBurst-style packet communication with automatic acknowledgement enabled.

| Parameter | Current firmware value |
|---|---|
| RF channel | 76 |
| CRC | 16-bit |
| Auto acknowledgement | Enabled |
| Payload mode | Dynamic payloads enabled |
| Data-rate states | 250 kbps, 1 Mbps, 2 Mbps |

These are firmware configuration values, not measured RF performance values.

## Packet Structure

The firmware uses the following fixed application/control structure:

| Field | Type | Purpose |
|---|---|---|
| `sequence` | `uint32_t` | Identifies packet order |
| `type` | `uint8_t` | `0` = data, `1` = rate proposal |
| `requestedRate` | `uint8_t` | Requested RF data-rate state |
| `retryProfile` | `uint8_t` | Transmitter retry-profile state |
| `uptimeMs` | `uint32_t` | Sender uptime at packet creation |

## Data Packet

Normal packets use `type = 0`. The sequence number increases for each generated packet. Successful `radio.write()` completion is treated by the transmitter as successful delivery with the nRF24L01 acknowledgement mechanism.

## Rate Proposal Packet

A packet with `type = 1` proposes a data-rate transition. The receiver consumes the proposal while the current link configuration is still active, then changes its local radio data rate. The transmitter follows after a short transition delay.

The purpose of this handshake-like ordering is to avoid both radios changing data rate independently.

## Reliability Indicators

The current implementation uses:

- successful/failed packet transmission,
- consecutive transmission failures,
- consecutive successful transmissions, and
- receiver packet timeout.

These indicators drive the adaptive control logic described in `adaptive-algorithm.md`.

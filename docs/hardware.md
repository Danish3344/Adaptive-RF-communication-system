# Hardware Configuration

## Components

| Component | Quantity | Role |
|---|---:|---|
| ESP32 development board | 2 | Controller for each wireless node |
| nRF24L01 | 2 | RF transceiver |
| Buzzer | 1 | Communication/status feedback |
| Breadboard and jumper wires | As required | Prototype interconnection |

## Interface

The nRF24L01 communicates with the ESP32 through the SPI interface and uses dedicated control lines for chip-enable and chip-select functions.

The exact GPIO mapping must match the physical prototype. It will be documented here from the verified wiring diagram before the final firmware is published.

## Power Considerations

The nRF24L01 is a 3.3 V device. The RF module supply and wiring should be checked carefully before powering the prototype. Stable power is important for reliable RF operation.

## Buzzer

The buzzer is connected to an ESP32 GPIO and is used as a local indication mechanism. Its final GPIO assignment and indication patterns will be documented with the verified firmware.

## Verification Checklist

- [ ] Confirm ESP32 board model
- [ ] Confirm nRF24L01 module variant
- [ ] Confirm CE GPIO
- [ ] Confirm CSN GPIO
- [ ] Confirm SPI SCK/MISO/MOSI GPIOs
- [ ] Confirm buzzer GPIO
- [ ] Verify common ground
- [ ] Verify 3.3 V RF-module supply

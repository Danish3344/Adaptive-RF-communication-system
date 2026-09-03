# Hardware Configuration

## Components

| Component | Quantity | Role |
|---|---:|---|
| ESP32 development board | 2 | Controller for each wireless node |
| nRF24L01 PA/LNA transceiver | 2 | 2.4 GHz RF transceiver |
| Buzzer | 1 | Receiver status feedback |
| Breadboard and jumper wires | As required | Prototype interconnection |

## Reference Wiring

The repository firmware uses the following fixed GPIO configuration on **both ESP32 boards**. This is the wiring to use when rebuilding the prototype.

| nRF24L01 signal | ESP32 GPIO |
|---|---:|
| VCC | 3.3 V |
| GND | GND |
| CE | GPIO 4 |
| CSN | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| IRQ | Not connected |

Receiver buzzer:

| Buzzer connection | Receiver ESP32 |
|---|---:|
| Positive / signal | GPIO 25 |
| Negative | GND |

The transmitter has no buzzer connection in the reference design.

## SPI Configuration

The firmware explicitly initializes the ESP32 SPI bus with:

- SCK: GPIO 18
- MISO: GPIO 19
- MOSI: GPIO 23
- CSN: GPIO 5
- CE: GPIO 4

Both nodes use the same GPIO assignment so the two firmware sketches can be rebuilt consistently.

## Power Considerations

The nRF24L01 is a **3.3 V device**. Connect VCC only to the ESP32 3.3 V supply and connect the module GND to ESP32 GND. Keep the RF module supply wiring short and stable. The PA/LNA module can be more sensitive to supply instability than the basic nRF24L01 module.

Do not connect nRF24L01 VCC directly to 5 V.

## Rebuild Checklist

- [ ] ESP32 #1 connected to nRF24L01 using the reference pin table
- [ ] ESP32 #2 connected to nRF24L01 using the reference pin table
- [ ] Both nRF24 modules powered from 3.3 V
- [ ] Common ground verified
- [ ] CE/CSN verified on GPIO 4/5
- [ ] SPI verified on GPIO 18/19/23
- [ ] Receiver buzzer connected to GPIO 25
- [ ] No nRF24 IRQ connection required
- [ ] Antennas installed before RF transmission
- [ ] Diagnostic firmware tested before adaptive firmware

## Validation Status

This pin configuration is the **repository-defined reference wiring**. It has not yet been revalidated on the dismantled physical prototype after the previous unexpected test behavior. Physical validation remains a required test step.

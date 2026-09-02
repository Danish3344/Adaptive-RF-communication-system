# Adaptive Communication Algorithm

## Objective

The adaptive layer responds to changing packet-delivery conditions instead of keeping a single RF configuration under all conditions.

## Current Decision Logic

```text
                    Start
                      |
                      v
               Send data packet
                      |
                +-----+-----+
                |           |
              success     failure
                |           |
                v           v
        reset failure   increment failure
          counter           counter
                |           |
                |      threshold reached?
                |           |
                |          yes
                |           v
                |    increase retry profile
                |           |
                |    propose lower data rate
                |           |
                +-----<-----+
                      |
                      v
                 continue loop
```

## Degradation Response

The transmitter counts consecutive failed packet transmissions. After the configured failure threshold is reached:

1. The transmitter moves to a stronger retry profile when available.
2. If the current data rate is above the minimum state, it sends a rate-change proposal at the currently working rate.
3. The receiver applies the proposed lower rate.
4. The transmitter follows the receiver after the transition delay.

## Recovery Response

When communication remains successful for a sustained number of packets, the transmitter can propose moving to a higher data rate. This allows the system to recover toward higher throughput after the link becomes stable.

## Current Firmware Parameters

| Parameter | Value |
|---|---:|
| Failure threshold | 3 consecutive failures |
| Recovery threshold | 20 consecutive successes |
| Data-rate states | 250 kbps / 1 Mbps / 2 Mbps |
| Retry profiles | 4 levels |

These thresholds are initial firmware control parameters. They are **not experimental claims** and should be tuned using measured test data.

## Important Limitation

The nRF24L01 provides packet-delivery/acknowledgement information, but the current prototype does not claim direct RSSI measurement from the nRF24L01. Therefore, link degradation is inferred from communication outcomes rather than a measured RSSI value.

Future versions can add an external RF-monitoring method if direct signal-strength or interference measurements are required.

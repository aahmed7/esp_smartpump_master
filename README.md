## ESP-32 based smart pump
This is the code for the master esp node, from my archives, done back in about 2018. The master is installed on a plant and monitors the soil moisture levels. Built using platformio.

## Master Features(https://github.com/aahmed7/esp_smartpump_master)
- stores config files on the esp32 spi flash mem in a JSON format(using arduinojson library)
- uses esp-now protocol using wifi to connect to the slave
- uses a 16x2 LCD to display the current time and moisture state
- uses analog soil moisture sensor to detect soil moisture from the plant, and signals the slave to send water when the moisture drops below threshold

## Slave features(https://github.com/aahmed7/esp32_smartpump_slave)
- Communicates with the master using esp-now protocol using wifi
- controls the flow of water to the plant
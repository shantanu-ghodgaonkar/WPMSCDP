# Wearable Physiological Monitoring System Using ClusterDuck Protocol

## Project Overview

This project involves the development of a **Wearable Physiological Monitoring System** that uses the **ClusterDuck Protocol (CDP)** to facilitate communication between wearable health monitoring devices and a base station. The system is designed to measure vital physiological parameters, such as heart rate and body temperature, and wirelessly transmit the data over **LoRa** communication.

### Key Features:
- **Low-Power Wearable Device**: Monitors heart rate and body temperature in real-time.
- **Wireless Communication**: Uses **LoRa** technology to communicate over long distances with low power.
- **ClusterDuck Protocol**: Facilitates communication and message routing among devices in disaster or emergency situations.
- **ESP32-based Implementation**: The wearable system is implemented using the ESP32 microcontroller and a set of sensors.

## Hardware Components

The table below summarizes the hardware components used in the Wearable Physiological Monitoring System, along with their descriptions, images, and links to datasheets.

| **Sl. No.** | **Component Name**                 | **Component Description**                                                                                                                                                                                                 | **Image**                                                                                                                   | **Link to Datasheet**                                                                                 |
|-------------|------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| 1           | **ESP32-WROOM-32D**                | - Microcontroller with Wi-Fi and Bluetooth capabilities.<br> - Handles sensor data acquisition and wireless communication.<br> - Supports low-power modes for efficient energy consumption.                                  | <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/esp32_wroom32d.png" alt="ESP32-WROOM-32D" width="100"/> | [ESP32-WROOM-32D Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf) |
| 2           | **RFM95 LoRa Transceiver**         | - Long-range wireless communication module.<br> - Uses LoRa modulation for robust transmission.<br> - Ideal for low-power applications requiring long-range communication.                                                | <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/rfm95_lora_transceiver.png" alt="RFM95 LoRa Transceiver" width="100"/> | [RFM95 LoRa Datasheet](https://www.hoperf.com/modules/lora/RFM95.html)                                                   |
| 3           | **MAX30102 Pulse Oximeter**        | - Measures heart rate and blood oxygen levels.<br> - Uses photoplethysmography (PPG) to detect blood flow.<br> - Ideal for wearable health monitoring systems.                                                            | <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/max30102_pulse_oximeter.png" alt="MAX30102 Pulse Oximeter" width="100"/> | [MAX30102 Datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf)                                           |
| 4           | **MCP9808 Temperature Sensor**     | - High-accuracy digital temperature sensor.<br> - Low power consumption suitable for battery-operated devices.<br> - Provides temperature data via I2C interface.                                                          | <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/mcp9808_temperature_sensor.png" alt="MCP9808 Temperature Sensor" width="100"/> | [MCP9808 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Temperature/MCP9808.pdf)                                 |
| 5           | **MCP1700 LDO**                    | - Low dropout voltage regulator.<br> - Powers the system components efficiently.<br> - Provides a stable voltage supply for sensor operations.                                                                              | <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/mcp1700_ldo.png" alt="MCP1700 LDO" width="100"/> | [MCP1700 Datasheet](https://www.microchip.com/wwwproducts/en/MCP1700)                                                     |
| 6           | **TP4056 Li-Ion Battery Charger**  | - Lithium-ion battery charger IC.<br> - Ensures efficient charging and protection.<br> - Used to charge the wearable device's battery pack.                                                                                | <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/tp4056_liion_charger.png" alt="TP4056 Li-Ion Battery Charger" width="100"/> | [TP4056 Datasheet](https://datasheet.lcsc.com/szlcsc/1809191016_TP4056_C126264.pdf)                                        |
| 7           | **Battery Pack**                   | - Rechargeable battery pack used to power the system.<br> - Provides continuous power for the sensors and microcontroller.<br> - Integrated into the wristband for compact design.                                          | <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/battery_pack.png" alt="Battery Pack" width="100"/> | N/A                                                                                                                       |


## System Overview and Working Procedure

The following section provides an integrated overview of the system's architecture, communication protocols, hardware connections, power supply, and application interface. This includes the **Block Diagram of the Proposed Model**, **ClusterDuck Protocol Functional Diagram**, and detailed descriptions of the system's working procedure.

### Block Diagram and ClusterDuck Protocol

The **Block Diagram of the Proposed Model** provides an overall view of the system architecture. It includes the ESP32 microcontroller, LoRa communication module (RFM95), pulse oximeter (MAX30102), temperature sensor (MCP9808), and the power supply connections.

The **ClusterDuck Protocol (CDP)** enables communication in scenarios where traditional infrastructure may be unavailable, such as in remote or disaster-stricken areas. The protocol helps devices communicate and relay messages efficiently through a series of nodes, such as "Mama Ducks" and "Papa Ducks." This design enables the system to maintain low-power communication over long distances using LoRa.

#### Block Diagram of the Proposed Model

<p align="center">
  <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/block_diagram_proposed_model.png" alt="Block Diagram of the Proposed Model" width="400"/>
</p>

<p align="center">
  <b>Fig 1:</b> Block Diagram of the Proposed Model.
</p>

#### ClusterDuck Protocol Functional Diagram

<p align="center">
  <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/clusterduck_protocol_diagram.png" alt="ClusterDuck Protocol Functional Diagram" width="400"/>
</p>

<p align="center">
  <b>Fig 2:</b> ClusterDuck Protocol Functional Diagram.
</p>

### System Connections and Power Supply

The **ESP32** serves as the main processing unit, interfacing with the RFM95 for communication, the MAX30102 for heart rate and SpO2 monitoring, and the MCP9808 for body temperature measurement. The system uses a **TP4056 battery charger** to charge the **Li-Ion battery pack**, ensuring continuous power for all components.

#### ESP32 and RFM95 Connections

The diagram below illustrates the connection between the ESP32 microcontroller and the RFM95 LoRa transceiver. These components are essential for transmitting sensor data over long distances using LoRa technology.

<p align="center">
  <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/esp32_rfm95_connections.png" alt="ESP32 and RFM95 Connections" width="400"/>
</p>

<p align="center">
  <b>Fig 3:</b> ESP32 and RFM95 Connections.
</p>

#### Power Supply

The system's power supply setup is designed for efficiency, ensuring long battery life. The **TP4056** charges the **Li-Ion battery pack**, which powers the ESP32, sensors, and LoRa transceiver. The **MCP1700** low dropout regulator ensures a stable voltage supply to the system.

<p align="center">
  <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/esp32_power_supply_connections.png" alt="ESP32 Power Supply Connections" width="400"/>
</p>

<p align="center">
  <b>Fig 4:</b> ESP32 Power Supply Connections.
</p>

### Application Interface

The wearable system communicates with an Android application that displays the sensor data in real-time. Below is a table summarizing the key applications used to monitor the data.

<table>
  <tr>
    <th style="width:10%;">Application</th>
    <th style="width:20%;">Description</th>
    <th style="width:70%;">Image</th>
  </tr>
  <tr>
    <td><b>Papa Duck App</b></td>
    <td>Displays real-time sensor data including heart rate, SpO2, and temperature. Suitable for individual users.</td>
    <td><img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/papa_duck_android_output.png" alt="Papa Duck Android Application Output" style="max-width:100%; height:auto;"/></td>
  </tr>
  <tr>
    <td><b>Mama Duck App</b></td>
    <td>Monitors multiple devices and visualizes data from all connected nodes in a network. Ideal for health monitoring in a group or disaster scenario.</td>
    <td><img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/mama_duck_android_output.png" alt="Mama Duck Android Application Output" style="max-width:100%; height:auto;"/></td>
  </tr>
</table>




### Detailed Working Procedure

The **Wearable Physiological Monitoring System** operates by continuously monitoring a user’s vital signs and transmitting this data over long distances using LoRa and the **ClusterDuck Protocol**. Here’s how the system works step by step:

1. **Data Acquisition**:
   - The **MAX30102 Pulse Oximeter** continuously measures the heart rate and SpO2 levels of the user, while the **MCP9808 Temperature Sensor** measures the user’s body temperature.
   - These sensors are connected to the **ESP32 microcontroller**, which reads and processes the data from the sensors.

2. **Data Transmission**:
   - The processed sensor data is transmitted from the ESP32 to the **RFM95 LoRa Transceiver**.
   - Using LoRa communication, the data is sent over long distances to a base station or relayed through other nodes using the **ClusterDuck Protocol (CDP)**. 
   - The CDP enables devices to form a network where data can be relayed over multiple "Mama Duck" or "Papa Duck" nodes, ensuring reliable transmission even in remote areas.

3. **Power Supply**:
   - The system is powered by a **Li-Ion battery pack** that ensures the device can run continuously for long periods.
   - The **TP4056 Li-Ion Battery Charger** manages the charging of the battery pack, while the **MCP1700 Low Dropout Regulator** ensures a stable power supply to the ESP32 and sensors.

4. **Data Visualization**:
   - The transmitted data can be visualized using the **Papa Duck Android application**, which displays the heart rate, temperature, and oxygen levels in real time.
   - The **Mama Duck Android application** can also visualize and monitor data from multiple devices in a network, making it suitable for applications in health monitoring during disasters or in remote areas.

This system is designed for robust and efficient health monitoring, especially in scenarios where traditional infrastructure may not be available. The combination of LoRa, low-power components, and the ClusterDuck Protocol ensures that the device can function in challenging environments with minimal power consumption.


## Prototype Design

The wearable system integrates the sensors and microcontroller into a wristband form factor.

### Sensor Placement

The placement of the **MCP9808** and **MAX30102** sensors within the wristband is shown below.

<p align="center">
  <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/sensor_placement_wrist_band.png" alt="Sensor Placement in Wrist Band" width="400"/>
</p>

<p align="center">
  <b>Fig 14:</b> MCP9808 and MAX30102 sensor placement in wrist band.
</p>

### Prototype Demonstration

Below are images of the prototype in action, worn around the wrist of a subject.

<p align="center">
  <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/prototype_on_wrist_subject.png" alt="Prototype on Subject's Wrist" width="400"/>
</p>

<p align="center">
  <b>Fig 15:</b> Prototype of the proposed model placed on the wrist of a subject.
</p>

<p align="center">
  <img src="https://github.com/shantanu-ghodgaonkar/WPMSCDP/blob/a21fdd2ad9406647542eeebbb47c6fa8daf0ebd3/img/prototype_worn_around_wrist.png" alt="Prototype Worn Around Wrist" width="400"/>
</p>

<p align="center">
  <b>Fig 16:</b> Prototype worn around wrist.
</p>

## Conclusion

This **Wearable Physiological Monitoring System** demonstrates the ability to monitor heart rate and body temperature using low-power devices that can communicate over long distances with the help of **LoRa** and the **ClusterDuck Protocol**. The integration of these technologies provides a solution for remote monitoring, especially in disaster situations where conventional communication infrastructure might fail.

## Future Enhancements

1. **Additional Sensors**: Integrating more physiological sensors, such as ECG, could extend the system’s capabilities.
2. **Data Analysis**: Future iterations could incorporate machine learning algorithms for real-time analysis of health data.
3. **Energy Efficiency**: Further optimization of the power consumption could extend the battery life, enabling continuous operation for longer periods.

## References

1. Datasheet for ESP32-WROOM-32D. [Link](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
2. Datasheet for MAX30102 Pulse Oximeter. [Link](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf)
3. ClusterDuck Protocol. [Link](https://www.clusterduckprotocol.org/)
4. Datasheet for MCP9808 Temperature Sensor. [Link](https://cdn.sparkfun.com/datasheets/Sensors/Temperature/MCP9808.pdf)
5. Datasheet for TP4056 Li-Ion Battery Charger. [Link](https://datasheet.lcsc.com/szlcsc/1809191016_TP4056_C126264.pdf)

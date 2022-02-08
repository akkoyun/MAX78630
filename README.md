## Description
The MAX78630 is an energy measurement processor for polyphase power-monitoring systems. It is designed for real-time monitoring for a variety of typical three-phase configurations in industrial applications. The MAX78630 provides up to six analog inputs for interfacing to voltage and current sensors. Scaled voltages from the sensors are fed to the single converter front-end using a high-resolution delta-sigma converter. Supported current sensors include current transformers (CTs), Rogowski coils, and resistive shunts.  

In this Arduino Library we can read all data of energy parameters.


















## Communication Protocol

**Request**

Library is connected to **MAX78630** with serial connection (but also we connect I2C and SPI). MAX78630 has a addressed UART protocol.  

When want to get a measurement from **MAX78630**, library send a request command to IC. The request command structure is below.  
  
| ID     | Name			            | Data | Description                                                                               |
|--------|--------------------------|------|-------------------------------------------------------------------------------------------|
| Byte 1 | Header 		            | 0xAA | This byte is named HEADER byte to connect the MAX78630 IC. This address is fixed to 0xAA  |
| Byte 2 | Total Sended Byte Count  | 0x07 | Total send byte count with the pack. This byte is fixed to 0x07 (7) to send command.      |
| Byte 3 | Command Byte Count       | 0xA3 | Total send byte count to send command. In measurement command this byte is 0xA3 (2 bytes) |
| Byte 4 | Command Byte 1 	        |      | This bytes are the core bytes of request. In this 2 bytes we determine requested data.    |
| Byte 5 | Command Byte 2           |      | This bytes are the core bytes of request. In this 2 bytes we determine requested data.    |
| Byte 6 | Requested Byte 	        | 0xE3 | This byte is requested byte count. 0xE3 is means request 3 byte.                          |
| Byte 7 | Checksum Correction	    | CHK  | This byte is Checksum byte for command. (MOD256)                                          |
  
***

**Response**  

When we send request data command, we receive answer. The recieved answer is 6 byte long data packege, RAW data is 3 byte long as we set sended command (byte 6)  
  
| ID     | Name			             | Description |
|--------|---------------------------|-------------|
| Byte 1 | Header 		             | 0xAA 	   |
| Byte 2 | Total Recieved Byte Count | 6 Byte 	   |
| Byte 3 | Recieved Data 1           | ?           |
| Byte 4 | Recieved Data 2	         | ?		   |
| Byte 5 | Recieved Data 3           | ?		   |
| Byte 6 | Checksum Correction	     | MOD 256     |
  
Raw Data = (Byte 3) + (Byte 4 x 256) + (Byte 5 x 65536)  
  
***

According the Byte1 we determine the answer type such as;
  
The type of slave packet depends upon the type of command from the master device and the successful execution by the slave device. Standard replies include “Acknowledge” and “Acknowledge with Data”.  
  
| Acknowledge without data. |
|---------------------------|
| 0xAD                      |

| Acknowledge with data | Byte Count | Read Data      | Check Sum |
|-----------------------|------------|----------------|-----------|
| 0xAA                  | 0x06       | 0x15 0xB8 0x2C | 0x57      |
  
If no data is expected from the slave or there is a fail code, a single byte reply is sent. If a successfully decoded command is expected to reply with data, the slave sends a packet format similar to the master packet where the header is replaced with a Reply Code and the payload contains the read data.
  
| Reply Code | Definition                            |
|------------|-------------------------------------- |
| 0xAA       | Acknowledge with data                 | 
| 0xAB       | Acknowledge with data (half duplex)   |
| 0xAD       | Acknowledge without data.             | 
| 0xB0       | Negative Acknowledge (NACK).          | 
| 0xBC       | Command not implemented.              | 
| 0xBD       | Checksum failed.                      | 
| 0xBF       | Buffer overflow (or packet too long). | 

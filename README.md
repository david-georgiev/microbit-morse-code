# README.md

## Building Instructions
Files to Download
/source/
main.cpp - Main application code
module.json - Contains instructions for yotta on how to build
Commands Required Initially
yt target bbc-microbit-classic-gcc
yt build
Running Instructions
The executable can be found inside /build/bbc-microbit-classic-gcc/source, with the executable being named project_name-combined.hex
Copy this file to /media/student/MICROBIT

### Instructions
To connect the 2 Microbits we need 2 alligator clips. For the data transmission we use digital signal, which means that both Microbits need to share the same GROUND and PIN in order to establish communication. In our code we use Pin number 0 but any Pin is fine to use as soon as we change the Pin number in the code to the pin number of your choice. To start we need to compile the code for the receiver with the Boolean named receiver as false and then do the same for the second Microbit this time changing the value to true.
### Data encoding and decoding
This program is used to send morse code information between one micro:bit to another. The sender will write one character in morse code at the time, and then press a button to send it to the other device. The receiving device will be responsible to understand the message received and decode it into an ASCII letter. To perform this operation the ASCII characters are saved into a tree, so that the decoding of the message will be easier.
TREE IMAGE

### Transmission
 First The Microbit detects  if the a long or short click occur to separate the dot and dash symbol. The dot symbol indicate that the direction that we need to take on the tree is left when the dash symbol points to the right direction. For convenience we use button B to trigger the transmission of the Morse code that has been recorded up to that point. When an event on button B occurs the signal is encrypted after which the parity check algorithm is used to indicate if an error has occurred and finally the message is sent to the receiver where the message is decoded and decrypted.





### Data packet
The message is stored in a 9 bit data packet where the first 3 bits represent the length of the message, followed by the parity bit and the last 5 bits that represent the actual message(array).
| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| 0    | Are  | Cool |
|:----:|:----:|:----:|

We choose this approach knowing that our tree has 5 layers so anything more than 5 inputs would 
be an error. To represent an-layer tree where n < 9 we can simply grow the message array and adjust the error messages but anything more than that will require another bit added to the length since the maximum number represented by 3 bits is 8.
	
#### Dot
When button A is pressed for short duration(approx. anything less than 1 sec) and released it triggers an event whose pulse is calculated by the Difference of the total sleep minus the sleep time for the dot. 
#### Dash
When button A is pressed for long duration(approx. anything more than 1 sec) and released it triggers an event whose pulse is  calculated by the Difference of the total sleep minus the sleep time for the dash.
	
#### Encryption
The encryption is performed using XOR. The message section of the packet, before 		being sent, is encrypted by executing a bitwise XOR operation between the 			message and the key. The packet header is left decrypted for the receiver to 	“understand” it.
	
#### Parity Checksum
A checksum is accomplished using a ParityBit. Before being sent, the number of 1s 		in the packet is counted, and the parity bit will be set accordingly to have the total 	number of 1s even.
Upon the packets arrival, the total number of ones will be checked again, and an 		error during transmission will be revealed if the resulting count is odd. 
This method is effective for checking small packets of data where multiple errors 		during a single transmission are unlikely to happen. However this method will not 		be able to fully guarantee the integrity of the data.
	
#### Decryption
After gathering and decoding the header information from the packet, the message 	section is decrypted.To do this a bitwise XOR is performed between the encrypted 		message and the key, revealing the information of the message.

### Modes
To change mode the sender must simply press the button B when the buffer is empty.

NOTE:
1 Since a debugging tool wasn’t use for that code many of the print statement used for testing are commented but still inside the code and others are left for demonstration purposes.
2 The granularity of the sleep function is 6 ms so the actual time of the sleep functions should be calculated when the number is divided by 6. E.g (sleep(120) is actually  a  20 ms sleep)
3 Encryption occurs only on the last 5 bits of the message which holds the actual message 


Deyvid Gueorguiev
Giacomo Pellizzari

Building Instructions
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

###Instructions

To connect the 2 microbits we need 2 alligator clips. For the data transmission we use digital signal, which means that both microbits need to share the same GROUND and PIN in order to establish communication. In our code we use Pin number 0 but any Pin is fine to use as soon as we change the Pin number in the code to the pin number of your choice. To start we need to compile the code for the receiver with the Boolean named receiver as false and then do the same for the second microbit this time changing the value to true.

###Transmission 

###Protocol
For the transmission of the data we used data packets of 9 bits. The data has a header of 4 bits and the remaining 5 represent the message. The header is also divided into 3 bits that represent the length of the message and 1 parity bit to check the loss of information.

##Encription:





###Message
The message is stored in a 9 bit array where the first 3 bits are the length(represented by another array)followed by the parity bit and the last 5 bits that represent the actual message(array). We choose this approach knowing that our tree has 5 layers so anything more than 5 inputs would 
be an error. To represent an-layer tree where n<9 we can simply grow the message array and adjust the error messages but anything more than that will require another bit added to the length since the maximum number represented by 3 bits is 8.
	
	#Dot
	
	#Dash
	
	#Encryption
The encryption is performed using XOR. The message section of the packet, before being sent, is encrypted by executing a bitwise XOR operation between the message and the key. The packet header is left decrypted for the receiver to “understand” it.	
	#Checksum
A checksum is accomplished using a ParityBit. Before being sent, the number of 1s in the packet is counted, and the parity bit will be set accordingly to have the total number of 1s even.
Upon the packets arrival, the total number of ones will be checked again, and an error during transmission will be revealed if the resulting count is odd. 
This method is effective for checking small packets of data where multiple errors during a single transmission are unlikely to happen. However this method will not be able to fully guarantee the integrity of the data.
	
	
#Decryption
After gathering the header information from the packet, the message section is decrypted.To do this a bitwise XOR is performed between the encrypted message and the key, revealing the information of the message.
###Modes
To change mode the sender must simply press the button B when the buffer is empty.
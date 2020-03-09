/*
* File: main.cpp
* Authors: Deyvid Gueorguiev, Giacomo Pellizzari
* Date: 08/03/2020
* Desc: IOT Challenge 2
* Note: Change the boolean value for receiver to false when building the sender
*/
#define LOW  0
#define HIGH 1
 
#define T_DOT  60
#define T_DASH 180
#define T_EOT 360
#define T_DISP 600
 
#define PARITYBITPOSITION 3
#define SYMBOLSTARTINGPOSITION 4
#define KEYLENGTH 5
#define MESSAGELENGTH 9
 
 
#include "MicroBit.h"
#include <array>
 
MicroBit uBit;
 
MicroBitButton buttonB(MICROBIT_PIN_BUTTON_B, MICROBIT_ID_BUTTON_B);
MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);
MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_DIGITAL);
 
//Sender if false/ receiver if true
bool receiver = false;
 
// Node class
class Node{
public:
    char letter;
    Node *left;
    Node *right;
    //constructor
    Node(char _letter, Node *_left, Node *_right) : letter(_letter),left(_left),right(_right) {}
};
 
// Binary tree class
class BinaryTree{
private:
    Node *root;
    Node *current_node;
 
public:
    BinaryTree();
    Node* get_current_node();
    char get_current_letter();
    void step_left();
    void step_right();
    void reset();
};
 
Node* BinaryTree::get_current_node(){
    return current_node;
}
 
char BinaryTree::get_current_letter(){
    return current_node->letter;
}
 
void BinaryTree::step_left(){
    current_node = current_node->left;
}
 
void BinaryTree::step_right(){
    current_node = current_node->right;
}
 
void BinaryTree::reset(){
    current_node = root;
}
 
// Deufault constactor initializes a morse code tree
BinaryTree::BinaryTree(){
 
    //layer 5
    Node* under =  new Node('_', NULL, NULL);
    Node* five  =  new Node('5', NULL, NULL);
    Node* four  =  new Node('4', NULL, NULL);
    Node* three =  new Node('3', NULL, NULL);
    Node* two   =  new Node('2', NULL, NULL);
    Node* one   =  new Node('1', NULL, NULL);
    Node* six   =  new Node('6', NULL, NULL);
    Node* equal =  new Node('=', NULL, NULL);
    Node* seven =  new Node('7', NULL, NULL);
    Node* eight =  new Node('8', NULL, NULL);
    Node* nine  =  new Node('9', NULL, NULL);
    Node* zero  =  new Node('0', NULL, NULL);
 
    // Layer 4
    Node* h   =   new Node('H', five ,four );
    Node* v   =   new Node('V', NULL ,three);
    Node* f   =   new Node('F', under,NULL );
    Node* mul =   new Node('*', NULL ,two  );
    Node* l   =   new Node('L', NULL ,NULL );
    Node* div =   new Node('/', NULL ,NULL );
    Node* p   =   new Node('P', NULL ,NULL );
    Node* j   =   new Node('J', NULL ,one  );
    Node* b   =   new Node('B', six  ,equal);
    Node* x   =   new Node('X', NULL ,NULL );
    Node* c   =   new Node('C', NULL ,NULL );
    Node* y   =   new Node('Y', NULL ,NULL );
    Node* z   =   new Node('Z', seven,NULL );
    Node* q   =   new Node('Q', NULL ,NULL );
    Node* add =   new Node('+', eight,NULL );
    Node* sub =   new Node('-', nine, zero );
 
    // Layer 3
    Node* s = new Node('S', h  , v  );
    Node* u = new Node('U', f  , mul);
    Node* r = new Node('R', l  , div);
    Node* w = new Node('W', p  , j  );
    Node* d = new Node('D', b  , x  );
    Node* k = new Node('K', c  , y  );
    Node* g = new Node('G', z  , q  );
    Node* o = new Node('O', add, sub);
 
    // Layer 2
    Node* i = new Node('I', s, u);
    Node* a = new Node('A', r, w);
    Node* n = new Node('N', d, k);
    Node* m = new Node('M', g, o);
 
    // Layer 1
    Node* e = new Node('E', i, a);
    Node* t = new Node('T', n, m);
 
    root = new Node('@', e, t);
    current_node = root;
}
BinaryTree morse = BinaryTree();
 
int key[9] = {1,1,0,0,1};
int messageToSend[9] = {0,0,0,0,0,0,0,0,0};
int messagePosition = SYMBOLSTARTINGPOSITION;
string finalMessage = "";
 
/*
*Purpose: Reset message to send (message to send next time)
*Accepts: N/A
*Returns: N/A
*/
void resetSenderMessage(){
    for(int i=0;i<MESSAGELENGTH;i++){
        messageToSend[i] = 0;
    }
    messagePosition = SYMBOLSTARTINGPOSITION;
}
 
/*
*Purpose: Performs xor between the message to be sent and the key
*Accepts: N/A
*Returns: N/A
*/
void encrypt(){
    for(int i = SYMBOLSTARTINGPOSITION; i<SYMBOLSTARTINGPOSITION+KEYLENGTH; i++){
        messageToSend[i] = messageToSend[i]^key[i-SYMBOLSTARTINGPOSITION];
    }
}
 
 /*
*Purpose: Performs xor between the received message nd the key
*Accepts: pointer to the message
*Returns: N/A
*/
void decrypt(int *message){
    for(int i = SYMBOLSTARTINGPOSITION; i<SYMBOLSTARTINGPOSITION+KEYLENGTH; i++){
        *(message+i) = *(message+i)^key[i-SYMBOLSTARTINGPOSITION];
    }
}
 
/*
*Purpose: Event handler for button A (stores a valuie depending on the click)
*Accepts: Event object
*Returns: N/A
*/
void onButtonA(MicroBitEvent e){
    if (e.value == MICROBIT_BUTTON_EVT_CLICK){
        uBit.display.print(".");
        //if path is valid
        if((messagePosition < 9) && (morse.get_current_node()!= NULL)){
            messageToSend[messagePosition] = 0;
            messagePosition++;
            uBit.sleep(T_DISP-T_DOT);
            uBit.display.clear();
        }else{
            uBit.display.scroll("NULL");
            resetSenderMessage();
        }
    }
 
    if(e.value == MICROBIT_BUTTON_EVT_LONG_CLICK){
        uBit.display.print("-");
        //if path is valid
        if((messagePosition < 9) && (morse.get_current_node()!= NULL)){
            messageToSend[messagePosition] = 1;
            messagePosition++;
            uBit.sleep(T_DISP-T_DASH);
            uBit.display.clear();
        }else{
            uBit.display.scroll("NULL");
            resetSenderMessage();
        }
    }
}

/*
*Purpose: Loops through an array and sends it to the receiver
*Accepts: N/A
*Returns: N/A
*/
void send()
{
    for(int i=0; i<MESSAGELENGTH; i++){
        //send a 0
        if (messageToSend[i] == 0){
            P0.setDigitalValue(HIGH);
            uBit.display.print(".");
            uBit.sleep(T_DOT);
            P0.setDigitalValue(LOW);
            //keep screen on
            uBit.sleep(T_DISP-T_DOT);
            uBit.display.clear();
            uBit.sleep(30);
        }
        //send a 1
        if (messageToSend[i] == 1){
            P0.setDigitalValue(HIGH);
            uBit.display.print("-");
            uBit.sleep(T_DASH);
            P0.setDigitalValue(LOW);
            //keep screen on
            uBit.sleep(T_DISP-T_DASH);
            uBit.display.clear();
            uBit.sleep(30);
        }
    }
}

/*
*Purpose: Adds 3 bits to the start of the message//check
*Accepts: N/A
*Returns: boolean indicating mode change
*/
bool addLengthOfMessage(){
    int arr[3] = {0,0,0};
    int i=0,r;
    bool isChanging = true;
    messagePosition = messagePosition - SYMBOLSTARTINGPOSITION;
    uBit.display.print(messagePosition);
    uBit.sleep(90);
    //convert decimal to binary
    while(messagePosition!=0){
        r = messagePosition%2;
        arr[i++] = r;
        messagePosition /= 2;
    }
 
    //put it at the start of the message
    for(int i=0; i<PARITYBITPOSITION; i++){
        messageToSend[i]=arr[2-i];
        if(messageToSend[i]==1){
            isChanging = false;
        }
    }
    return isChanging;
}

 /*
*Purpose: Copies the old message into an array on it and a parity bit at the end
*Accepts: N/A
*Returns: N/A
*/
void addParityBit(){
    int numberOfOnes = 0;
   
    //count the number of ones
    for(int i = 0; i<MESSAGELENGTH; i++){
        if(i!=PARITYBITPOSITION){
            if(messageToSend[i]==1){
                numberOfOnes++;
            }
        }
    }
    //add 0 or 1 to the parity bit position
    if(numberOfOnes%2==0){
        messageToSend[PARITYBITPOSITION] = 0;
    }else{
        messageToSend[PARITYBITPOSITION] = 1;
    }
}
 
 /*
*Purpose: Event handler for button B
*Accepts: Event object
*Returns: N/A
*/
void onButtonB(MicroBitEvent e)
{
    if (e.value == MICROBIT_BUTTON_EVT_CLICK){
        bool isChanging = false;
        //add length to message (getLengthOfMessage)
        isChanging = addLengthOfMessage();
 
        //encrypt message
        encrypt();
 
        //add parity bit
        addParityBit();
 
        //send message
        send();

        //reset message to send (message to send next time)
        resetSenderMessage();
 
        if(isChanging==true){
            receiver = true;
            //uBit.display.scroll("ch");
        }
    }
}
 
/*
*Purpose: Decodes the incoming message and puts it into an array
*Accepts: Pointer to the message and an index
*Returns: False //check
*/
bool decode(int *message, int index)
{
    int counter = 0;
    while(P0.getDigitalValue() == 1){
        counter++;
        uBit.sleep(6);
    }
    //uBit.display.scroll(counter);
    if(counter < 20){
        *(message+index)=0;
        return false;
    }else if(counter > 20 && counter < 45){
        *(message+index)=1;
        return false;
    }else{
        //message finished
        return false;
    }      
    return false;
}
 /*
*Purpose: Counts the lenght of the message
*Accepts: Pointer to an array
*Returns: Lenght of message
*/
int getLengthOfMessage(int *array){
    int bitValue = 4;
    int result = 0;
    //get the first three bits of the received message and converts them into a number
    for(int i=0; i<PARITYBITPOSITION; i++){
        if(*(array+i) == 1){
            result += bitValue;
        }
        bitValue = bitValue/2;
    }
    return result;
}

/*
*Purpose: Traverses the binary tree to find a node
*Accepts: Pointer to the  message and length of the message
*Returns: N/A
*/ 
void traverseTree(int *incomingMessage, int length){
    int symbol = 0;
    //loop starting from after the message header
    for(int i=SYMBOLSTARTINGPOSITION; i<SYMBOLSTARTINGPOSITION+length; i++){
        symbol = *(incomingMessage+i);
        if (symbol == 1){
            morse.step_right();
           
            // Check if we are still on a valid path
            if (morse.get_current_node() == NULL){
                uBit.display.print("NULL");
                uBit.sleep(600);
                uBit.display.clear();
                morse.reset();
            }
        }
        else if (symbol == 0){
            morse.step_left();
 
            // Check if we are still on a valid path
            if (morse.get_current_node() == NULL){
                uBit.display.print("NULL");
                uBit.sleep(600);
                uBit.display.clear();
                morse.reset();
            }
        }
    }
    //print the letter and append to message if its not the rooT
    if(morse.get_current_letter()!='@'){
        uBit.display.print(morse.get_current_letter());
        finalMessage += morse.get_current_letter();
        uBit.sleep(600);
    }
    uBit.display.clear();
    morse.reset();
}

 /*
*Purpose: Resets all of the receiver variables
*Accepts: pointers to the message , message index , message received
*Returns: N/A
*/
void resetReceiverVariables(int *incomingMessage, int *messageIndex, bool *messageReceived){
    //function that resets the incomingMessage array
    for(int i=0; i<MESSAGELENGTH; i++){
        *(incomingMessage+i) = 0;
    }
    //reset the incomingMessageIndex
    *messageIndex = 0;
    //reset the messageReceived bool
    *messageReceived = false;
}

/*
*Purpose: Parity checksum
*Accepts: pointer to the message and lenght of the message
*Returns: boolean indicating odd or even number of 1's
*/
bool checksum(int *message, int messageLength){
    bool result = true;
    int numberOfOnes = 0;
 
    //count the number of ones in the message
    for(int i=0; i<messageLength; i++){
        if(*(message+i)==1){
            numberOfOnes++;
        }
    }
    //check if message is ok
    if(numberOfOnes%2 != 0){
        result = false;
    }
    return result;
}
 
 /*
*Purpose: Keeps the programm constantly running
*Accepts: N/A
*Returns: N/A
*/
void alive(){
    int incomingMessage[MESSAGELENGTH] = {0,0,0,0,0,0,0,0,0};
    int incomingMessageIndex = 0;
    bool messageReceived = false;
   
 
    //receiver variables
    int length = 0;
    bool messageIsOk = false;
    resetReceiverVariables(incomingMessage, &incomingMessageIndex, &messageReceived);
   
 
    //receiver code
    while(receiver){
        //if message is arriving:
        if(P0.getDigitalValue() == 1){
           
            messageReceived = decode(incomingMessage, incomingMessageIndex);
            incomingMessageIndex++;
            //uBit.display.print(incomingMessageIndex);
 
            //all bits of message received
            if(incomingMessageIndex>8){
                messageReceived = true;
            }
 
            //if all of the message has arrived then travers the tree for each dot or dash (yet to do)
            if(messageReceived){
                length = 0;
                messageIsOk = false;
                uBit.display.scroll("M-R");
                uBit.sleep(6);
 
                //sumcheck
                messageIsOk = checksum(incomingMessage, MESSAGELENGTH);
 
                //if message is received correctly
                if(messageIsOk){
                    uBit.display.scroll("OK");

                    //decrypt
                    decrypt(incomingMessage);
 
                    //get length of message
                    length = getLengthOfMessage(incomingMessage);
                    //uBit.display.print(length);
 
                    //if it's termination message then change state
                    if(length==0){
                        receiver = false;
                        for (char const &c: finalMessage) {
                            uBit.display.scroll(c);
                            uBit.sleep(0);
                        }
                        uBit.display.clear();
                        //uBit.display.print(finalMessage);
                        finalMessage = "";
                    }else{
                        //traverse tree and show variables
                        traverseTree(incomingMessage, length);
                    }
                }else{
                    uBit.display.scroll("CORRUPT");
                    uBit.sleep(600);
                }
 
                //reset all of the variables
                resetReceiverVariables(incomingMessage, &incomingMessageIndex, &messageReceived);
            }
 
        }
        uBit.sleep(6);
    }
 
    //sender code
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButtonA, MESSAGE_BUS_LISTENER_DROP_IF_BUSY);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, onButtonB, MESSAGE_BUS_LISTENER_DROP_IF_BUSY);
 
    while(!receiver){
        uBit.sleep(120);
    }
    //disable buttons
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButtonA);
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, onButtonB); 
}
 
int main()
{
 
    //write first message
    if(receiver)
    {
        uBit.display.print("receiver");
    }else{
        uBit.display.print("sender");
    }
 
    // Initialise the micro:bit runtime.
    uBit.init();
 
   
    //keep the program running
    while(1){
        alive();
    }
 
    release_fiber();
}
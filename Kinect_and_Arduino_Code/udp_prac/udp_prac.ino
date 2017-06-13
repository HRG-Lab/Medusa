// Demonstrates usage of the new udpServer feature.
//You can register the same function to multiple ports, and multiple functions to the same port.
//
// 2013-4-7 Brian Lee <cybexsoft@hotmail.com>

#include <EtherCard.h>
#include <IPAddress.h>

#define STATIC 1  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,1,203 };
// gateway ip address
static byte gwip[] = { 192,168,1,200 };
static byte gw_netmask[] = {255,255,255,0};

//dns ip address
static byte dnsip[] = {192,168,1,200};
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

//callback that prints received packets to the serial port
void udpSerialPrint(word port, byte ip[4], const char *data, word len) {
  IPAddress src(ip[0], ip[1], ip[2], ip[3]);
  Serial.println(src);
  Serial.println(port);
  Serial.println(data);
  Serial.println(len);
  
  char payload[] = "My UDP message";
  uint16_t nSourcePort = 1234;
  uint16_t nDestinationPort = 6000;
  uint8_t ipDestinationAddress[4];
  ether.parseIp(ipDestinationAddress, "192.168.1.200");

  ether.sendUdp(payload, sizeof(payload), nSourcePort, gwip, nDestinationPort);

}

void setup(){
  Serial.begin(57600);
  Serial.println(F("\n[backSoon]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
#if STATIC
  ether.staticSetup(myip, gwip,dnsip,gw_netmask);
#else
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  //register udpSerialPrint() to port 1337
  ether.udpServerListenOnPort(&udpSerialPrint, 1337);

  //register udpSerialPrint() to port 42.
  ether.udpServerListenOnPort(&udpSerialPrint, 42);
}

void loop(){
  //this must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
}


/*
//Processing sketch to send test UDP packets.

import hypermedia.net.*;

 UDP udp;  // define the UDP object


 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true );     // <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message
 }

 void draw()
 {
 }

 void keyPressed() {
 String ip       = "192.168.0.200";  // the remote IP address
 int port        = 1337;    // the destination port

 udp.send("Greetings via UDP!", ip, port );   // the message to send

 }

 void receive( byte[] data ) {       // <-- default handler
 //void receive( byte[] data, String ip, int port ) {  // <-- extended handler

 for(int i=0; i < data.length; i++)
 print(char(data[i]));
 println();
 }
*/
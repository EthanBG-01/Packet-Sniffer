#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>
#include<net/ethernet.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/if.h>
/*
  Must be run with sudo privilidges.
*/

void identify_protocol(unsigned char* , int);
int total=0;

int main(int argc, char **argv) {
  //Raw sockets operate at the network OSI level; TCP, UDP not decoded.

  //IPv4, Raw Socket, All Packets.
  int r_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (r_sock == -1){
    perror("Socket creation failed.\n");
    exit(1);
  }

  int packet_size;
  //String buffer to hold incoming packets:
  unsigned char *buffer = (unsigned char *)malloc(65536);
  struct sockaddr_in source_socket_address, dest_socket_address;

  while(1) {
    //Recieves a message from socket.
    /*Socket, Buffer:message stored, 65536:max IP packet size (buffer length),
     Flags:0 - type of reception, address:sending address is to be stored. address_len: length of sockaddr structure*/
    packet_size = recvfrom(r_sock, buffer, 65536, 0 ,NULL, NULL);
    if (packet_size == -1) {
      printf("Failed to get packets\n");
      return 1;
    }
    identify_protocol(buffer, packet_size);



  }

  return 0;
}

void identify_protocol(unsigned char* buffer, int size){
    //Just gets the IP header part: how do you get the ethernet?
  struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
  ++total;
  switch(iph->protocol){
    case 1:
      printf("ICMP");
      break;
    case 2:
      printf("IGMP");
      break;
    case 6:
      printf("TCP");
      break;
    case 17:
      printf("UDP");
      break;
    default:
      break;
  }
}



/*
https://squidarth.com/networking/systems/rc/2018/05/28/using-raw-sockets.html
http://www.microhowto.info/howto/capture_ethernet_frames_using_an_af_packet_socket_in_c.html

IPROTO_RAW: You can interact with layer 3 (IP); not just 4 (TCP/UDP)
AF_INET: IPv4 (type of addresses socket can communicate with.)
AF_PACKET: Captures ethernet frames.
SOCK_RAW: Results in a raw packet;
IPPROTO_RAW.


*/

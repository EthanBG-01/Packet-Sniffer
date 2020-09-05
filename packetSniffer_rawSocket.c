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

void read_packet(unsigned char* , int);
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
    read_packet(buffer, packet_size);



  }

  return 0;
}

void read_packet(unsigned char* buffer, int size){
  //Big/ Little Endian???

  //Getting the Ethernet Header:
  /*
    struct ethhdr {
      unsigned char h_dest[ETH_ALEN];   //destination eth addr #define ETH_ALEN	6		/* Octets in one ethernet addr
      unsigned char h_sourec[ETH_ALEN]; //source ether addr
      __be16 h_proto;                   //packet type D field

    }
  */
  printf("Ethernet Layer: \n");
  struct ethhdr * eth = (struct ethhdr *)(buffer);
  printf("\t | -source MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
  printf("\t | -destination MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
  printf("\t | -protocol : %d\n", eth->h_proto);

  /*
  struct iphdr {
  #if defined(__LITTLE_ENDIAN_BITFIELD)
      __u8    ihl:4,
              version:4;
  #elif defined (__BIG_ENDIAN_BITFIELD)
      __u8    version:4,
              ihl:4;
  #else
      #error  "Please fix <asm/byteorder.h>"
  #endif
       __u8   tos;
       __u16  tot_len;
       __u16  id;
       __u16  frag_off;
       __u8   ttl;
       __u8   protocol;
       __u16  check;
       __u32  saddr;
       __u32  daddr;

      };
  */

  printf("IP Header: \n");
  struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
  printf("\t |- Version: %d\n", (unsigned int)iph->version);
  printf("\t |- Protocol: %d\n", (unsigned int)iph->protocol);

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

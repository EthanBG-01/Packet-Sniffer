#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>    //IP Header
#include<net/ethernet.h>  //for htons ETH_P_ALL
#include<sys/socket.h>    //Sockets API
#include<arpa/inet.h>     //inet_ntoa

void extract_ethernet_frame(unsigned char* , int);
void extract_ip_header(unsigned char* buffer, int size);
void extract_icmp_packet(unsigned char* buffer, int size);

int total=0;
struct sockaddr_in source, dest;

int main(int argc, char **argv) {

  //Creates a raw socket, that accepts packets.
  int r_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (r_sock == -1){
    perror("Socket creation failed.\n");
    exit(1);
  }

  //Create a buffer to accept incomming packets.
  int packet_size;
  unsigned char *buffer = (unsigned char *)malloc(65536);

  //Loop & Accept packets:
  while(1) {
    packet_size = recvfrom(r_sock, buffer, 65536, 0 ,NULL, NULL);
    if (packet_size == -1) {
      printf("Failed to get packets\n");
      return 1;
    }
    //Extract data from each packet.
    extract_ethernet_frame(buffer, packet_size);
  }
  return 0;
}

/*Ethernet Frame Structure :
    (Preamable + SDF = 8) | (6) Destination * | (6) Source * | (2) Type *
*/
void extract_ethernet_frame(unsigned char* buffer, int size){
  //Declare pointer called eth to an ethhdr structure;
  struct ethhdr * eth = (struct ethhdr *)(buffer);

  printf("Ethernet Frame: \n");
  printf("\t  | Source MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
  printf("    | Destination MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
  printf("    | Protocol : %d\n", eth->h_proto);

  //Extract the next layer: (IP)
  extract_ip_header(buffer, size);
}

/*IP Header Structure :
  (4) Version * | (4) Header Length * | (8) Type * |
  (16) Length Total | (16) Trusted Host ID | (3) Flags | (13) Fragment Offset
  (8) TTL * | (8) Protocol * | (16) Checksum * | (32) Src Addr * | (32) Dest Addr *
  (x * 32) Options & Padding.
*/
void extract_ip_header(unsigned char* buffer, int size){
  printf("IP Header: \n");
  //Buffer is a pointer; iphdr located after ethernet header
  struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr)); //Buffer is a pointer;

  //Reset the address value; fill with saddr from the ip header.
  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = iph->saddr;
  memset(&dest, 0, sizeof(dest));
  dest.sin_addr.s_addr = iph->daddr;

  printf("\t |IP Version: %d", (unsigned int)iph->version);
  printf("   |Header Length: %d Bytes", ((unsigned int)(iph->ihl))*4);
  printf("   |TTL: %d", (unsigned int)iph->ttl);
  printf("   |Protocol: %d\n", (unsigned int)iph->protocol);

  //inet_ntoa: Accepts Internet address (32-bit quantity in network byte order)
  //Returns string in dotted notation.
  printf("\t |SourceIP : %s", inet_ntoa(source.sin_addr));
  printf("   |Destination IP: %s\n", inet_ntoa(dest.sin_addr));

  int protocol  = (unsigned int)iph->protocol;
  //Different protocols result in different structures.
  if (protocol == 1){ //ICMP
      extract_icmp_packet(buffer, size);
  }
}

void extract_icmp_packet(unsigned char* buffer, int size){
  printf("ICMP Packet: ")
}

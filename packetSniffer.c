#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>

int main() {
		//Requires installation of pcap lib:
		//sudo apt-get install libpcap-dev

		char *device; //Name of device
		char error_buffer[PCAP_ERRBUF_SIZE];

		//Find a network device:
		device = pcap_lookupdev(error_buffer);


	return 0;
}

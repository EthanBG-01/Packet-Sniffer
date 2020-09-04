#include <stdio.h>
#include <pcap.h>

/*Tutorial that helped make this;
https://www.devdungeon.com/content/using-libpcap-c#intro */

int main(int argc, char **argv) {
		//Requires installation of pcap lib:
		//sudo apt-get install libpcap-dev

		char *device; //Name of device
		char error_buffer[PCAP_ERRBUF_SIZE];

		//Find a network device:
		device = pcap_lookupdev(error_buffer);
		if (device == NULL) {
			printf("Error finding a device: %s\n", error_buffer);
			return 1;
		}

		printf("Network device found: %s\n", device);

		//Device Info:
		int lookup_return_code;
		bpf_u_int32 ip_raw;
		bpf_u_int32 subnet_mask_raw;

		lookup_return_code = pcap_lookupnet(
	         device,
	         &ip_raw,
	         &subnet_mask_raw,
	         error_buffer
	   );

		if (lookup_return_code == -1){
			printf("%s\n", error_buffer);
			return 1;
		}

		//Get IP in human form:
		address.s_addr = ip_raw;
		    strcpy(ip, inet_ntoa(address));
		    if (ip == NULL) {
		        perror("inet_ntoa"); /* print error */
		        return 1;
		    }

	return 0;
}

// Compile with : gcc packetSniffer.c -o packetSniffer -lpcap

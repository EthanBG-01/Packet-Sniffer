# Linux Command Line Packet-Sniffer in C
A Packet sniffer that runs on Linux Terminal [Requires root privilidges].

## Motivations
This year at University, I have an OS and Networking module, which also teaches programming in C. Therefore, as preparation, I created this basic packet sniffer to gain a better understanding in this area. This project contains two files; `packetSniffer.c`, which makes use of `libpcap`, and  `packetSniffer_rawSocket.c`: both achieve the same results through different means.

## Features
- Extracts data from each layer (Ethernet Frame --> Data)
- Displays data in a readable format on the command line.

## How this Works (Raw Sockets, and Packet Details)
#### Raw Sockets: 
- Raw sockets accept packets directly from the Network OSI level.
- TCP/IP headers are left encoded.

  #### Linux IPv4 Raw Sockets:
  - https://linux.die.net/man/7/raw
  - "Allow IPv4 Protocols to be implemented in user space."
  - Requires root privilidges: "Only processes with an effective user ID of 0 or the CAP_NET_RAW capability are allowed to open raw sockets."
    - Ports recide within the TCP/IP layers; as such you can accept packets inbound for a different user's application at this level, and possibly spoof them. Consequently, this poses a security risk.

#### Layers and Structures:

##### The Ethernet Frame:
##### IP Header:
##### ICMP:
##### TCP:
##### UDP:
  
## Future Improvements
- [ ] GUI, Similar to the likes of wireshark.

### Useful articles/ tutorials:
- https://www.opensourceforu.com/2015/03/a-guide-to-using-raw-sockets/

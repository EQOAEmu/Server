///////////////////////////////////////////////////////////////
//CRCsupport.c 
//
///////////////////////////////////////////////////////////////

#include "CRCsupport.h"

unsigned char*  gen_hex_string(void* packet, int len){
    uint8_t* bytes = (uint8_t*)packet;
    int i = 0;
    unsigned char hex[100];
    unsigned char *hexstring = malloc(4*1024);

    while (i < len){
	sprintf(hex, "%02X", bytes[i]);
	strcat(hexstring, hex);
	printf("%02X", bytes[i++]);
    }

    printf("\n");
    return hexstring;
}


int AppendCRC(unsigned char* hexstr, unsigned char* packet, int pSize) {
    char *start = packet+pSize;
    unsigned int u;
    int i = 0;
    while (i < 4 && sscanf(hexstr,"%2x",&u) == 1) {
	*start++ = u;
	hexstr += 2;
	i++;
	printf("copied: %x\n", u);
    }
    return 0;
}

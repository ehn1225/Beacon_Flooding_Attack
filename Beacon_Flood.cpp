//20230112 Best of the Best 11th 이예찬
#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "mac.h"
#include <vector>

using namespace std;

struct ieee80211_radiotap_header {
    u_char it_version;
    u_char it_pad;
    u_int16_t it_len;
	u_int32_t it_present_flags;
};

struct ieee80211_radiotap_channel {
	u_int16_t ch_frequency;
	u_int16_t ch_flags;
	int8_t antenna_signal;	
};

struct ieee80211_beacon_frame{
	u_int16_t frameCtl;
	u_int16_t duration;
	Mac dstAddr;
	Mac srcArrr;
	Mac bssId;
	u_int16_t Fraq_Squ_Number;

};

struct ieee80211_wireless_management{
	u_int64_t timestamp;
	u_int16_t beaconInterval;
	u_int16_t capablityInfo;
};

struct ieee80211_tag_ssid{
	u_int8_t tagNumber;
	u_int8_t tagLength;
	char ssid[32];
};

void usage() {
	printf("syntax: beacon-flood [interface] [ssid-list-if_stream]\n");
	printf("sample: beacon-flood mon0 ssid-list.txt\n");
}

int main(int argc, char* argv[]) {
	vector<string> ssids;

	if(argc == 2){
		ssids.push_back("1. LYC_Hello World");
		ssids.push_back("2. LYC_BEST OF THE BEST");
		ssids.push_back("3. LYC_2023_01_12");
		ssids.push_back("4. LYC_No More HomeWork");
		ssids.push_back("5. LYC_YES_I_CAN_DO");
	}
	else if(argc == 3){
		ifstream if_stream;
		if_stream.open(argv[2]);
		if(if_stream.is_open()){
			while (!if_stream.eof()){
				string tmp;
				getline(if_stream, tmp);
				ssids.push_back(tmp);
			}
		}
		if_stream.close();
	}
	else{
		usage();
		return 1;
	}

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf);
	if (pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", argv[1], errbuf);
		return -1;
	}

	unsigned char packet[1024];

	//Radiotap Header 생성 및 초기화
	struct ieee80211_radiotap_header radiotap;
	memset(&radiotap, 0, 8);
	radiotap.it_len = htons(0x0800);

	//Wireless_Management 구조체 생성 및 설정
	struct ieee80211_wireless_management wireless_mgr;
	wireless_mgr.timestamp = 0x00000330289f01a3;
	wireless_mgr.beaconInterval = htons(0x6400);
	wireless_mgr.capablityInfo = htons(0x1431);
	
	//unsigned char arr[] = {0x01, 0x08, 0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0x03, 0x01, 0x01, 0x05, 0x04, 0x00, 0x01, 0x00, 0x00, 0x07, 0x06, 0x4b, 0x52, 0x04, 0x01, 0x0d, 0x17, 0x2a, 0x01, 0x02, 0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x0c, 0x00, 0x3b, 0x06, 0x51, 0x53, 0x54, 0x7d, 0x80, 0x81, 0x2d, 0x1a, 0xad, 0x09, 0x17, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x3d, 0x16, 0x01, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xbf, 0x0c, 0x92, 0x79, 0x81, 0x33, 0xfa, 0xff, 0x0c, 0x03, 0xfa, 0xff, 0x0c, 0x03, 0xc0, 0x05, 0x00, 0x00, 0x00, 0xfa, 0xff, 0xc3, 0x02, 0x00, 0x17, 0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0x81, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32, 0x2f, 0x00, 0xdd, 0x0b, 0x8c, 0xfd, 0xf0, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01, 0x01, 0xdd, 0x35, 0x00, 0x50, 0xf2, 0x04, 0x10, 0x4a, 0x00, 0x01, 0x10, 0x10, 0x44, 0x00, 0x01, 0x02, 0x10, 0x49, 0x00, 0x06, 0x00, 0x37, 0x2a, 0x00, 0x01, 0x20, 0x10, 0x11, 0x00, 0x0d, 0xec, 0xa4, 0x80, 0xec, 0x84, 0x9c, 0xec, 0x9d, 0x98, 0x20, 0x53, 0x32, 0x32, 0x10, 0x54, 0x00, 0x08, 0x00, 0x0a, 0x00, 0x50, 0xf2, 0x04, 0x00, 0x05, 0xdd, 0x08, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x02, 0x01, 0x00, 0xdd, 0x12, 0x50, 0x6f, 0x9a, 0x09, 0x02, 0x02, 0x00, 0x25, 0x89, 0x03, 0x06, 0x00, 0xf2, 0xcd, 0x31, 0x22, 0xf8, 0x87, 0x51, 0xb6, 0x5e, 0xb8};
	unsigned int index = 0;
	while(true){
		unsigned int pos = 0;
		//Beacon Frame 생성 및 설정
		struct ieee80211_beacon_frame beaconframe;
		beaconframe.frameCtl = htons(0x8000);
		beaconframe.duration = 0;
		beaconframe.dstAddr = Mac::broadcastMac();
		beaconframe.Fraq_Squ_Number = htons(0x904e);
		Mac rand_mac = Mac::randomMac();
		beaconframe.srcArrr = rand_mac;
		beaconframe.bssId = rand_mac;

		struct ieee80211_tag_ssid tag_ssid;
		tag_ssid.tagNumber = 0;
		string ssid = ssids.at(index++ % ssids.size());
		memcpy(&tag_ssid.ssid, ssid.c_str(), ssid.length());
		tag_ssid.tagLength = ssid.length();

		//radiotap 복사
		int size = sizeof(radiotap);
		memcpy((char*)&packet, (char*)&radiotap, size);
		pos += size;
		printf("size : %d, pos : %d\n", size, pos);

		//beacon frame 복사
		size = sizeof(beaconframe);
		memcpy((char*)&packet + pos, (char*)&beaconframe, size);
		pos += size;
		printf("size : %d, pos : %d\n", size, pos);

		//Wireless_Management 복사
		size = 12;
		memcpy((char*)&packet + pos, (char*)&wireless_mgr, size);
		pos += size;
		printf("size : %d, pos : %d\n", size, pos);

		size = 2 + tag_ssid.tagLength;
		memcpy((char*)&packet + pos, (char*)&tag_ssid, size);
		pos += size;
		printf("size : %d, pos : %d\n", size, pos);

		// size = sizeof(arr);
		// memcpy((char*)&packet + pos, (char*)&arr, size);
		// pos += size;
		// printf("size : %d, pos : %d\n", size, pos);

		for(int i = 0; i < pos;i++){
			printf("%02x ", packet[i]);
		}

		//화면 출력
		printf("BSSID : %s, SSID : %s\n", string(rand_mac).c_str(), ssid.c_str());
		int res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(&packet), pos);
		if (res != 0) {
			fprintf(stderr, "Beacon_Flood::pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
		}
	}

	pcap_close(pcap);
}


		// unsigned char tmp[52];
		// memcpy(tmp, (char *)&tag_ssid, sizeof(tag_ssid));

		// for(int i = 0; i < sizeof(tag_ssid);i++){
		// 	printf("%02x ", tmp[i]);
		// }

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

	unsigned char packet[256];

	//Radiotap Header 생성 및 초기화
	struct ieee80211_radiotap_header radiotap;
	memset(&radiotap, 0, 8);

	//Wireless_Management 구조체 생성 및 설정
	struct ieee80211_wireless_management wireless_mgr;
	wireless_mgr.timestamp = 0x00000330289f01a3;
	wireless_mgr.beaconInterval = htons(0x6400);
	wireless_mgr.capablityInfo = htons(0x1104);
	
	unsigned int index = 0;
	unsigned int pos = 0;
	while(true){
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

		for(int i = 0; i < pos;i++){
			printf("%02x ", packet[i]);
		}

		//화면 출력
		printf("BSSID : %s, SSID : %s\n", string(rand_mac).c_str(), ssid.c_str());
		// int res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(&packet), sizeof(EthArpPacket));
		// if (res != 0) {
		// 	fprintf(stderr, "Beacon_Flood::pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
		// }
		break;

	}

	pcap_close(pcap);
}


		// unsigned char tmp[52];
		// memcpy(tmp, (char *)&tag_ssid, sizeof(tag_ssid));

		// for(int i = 0; i < sizeof(tag_ssid);i++){
		// 	printf("%02x ", tmp[i]);
		// }

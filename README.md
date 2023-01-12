<h1>Beacon_Flood_Attack</h1>
SSID가 기록된 파일을 읽어 Beacon Frame을 발생하는 프로그램 입니다.<br>

syntax : beacon-flood [interface] [ssid-list-file]<br>
sample : beacon-flood mon0 ssid-list.txt<br>

<h2>WireShark Beacon Frame 필터</h2>
wlan.fc.type_subtype == 0x0008<br>

<p>https://gitlab.com/gilgil/sns/-/wikis/dot11-frame/report-beacon-flood</p>

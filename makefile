LDLIBS += -lpcap

all: beacon-flood

mac.o : mac.h mac.cpp

beacon-flood : Beacon_Flood.o mac.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f beacon-flood *.o

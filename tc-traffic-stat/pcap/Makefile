APP=pcap-stat

build:
	g++ -Wall -std=c++17 -o $(APP) main.cpp -lpcap -lpthread

run:
	sudo ./$(APP)

clean:
	rm -f $(APP)

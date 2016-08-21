all:
	$(MEDIA_CXX) -I $(MEDIA_UDP_MULTICAST_DIR)/include -o  udp_multicast_send.bin  udp_multicast_sendtest.c -Wl,--start-group -L $(MEDIA_UDP_MULTICAST_DIR)/lib -ludp_multicast -lrt -Wl,--end-group -Wall
	$(MEDIA_CXX) -I $(MEDIA_UDP_MULTICAST_DIR)/include -o  udp_multicast_recv.bin  udp_multicast_recvtest.c -Wl,--start-group -L $(MEDIA_UDP_MULTICAST_DIR)/lib -ludp_multicast -lrt -Wl,--end-group -Wall
clean:
	rm -rf udp_multicast_recv.bin udp_multicast_send.bin

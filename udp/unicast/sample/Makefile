all:
	$(MEDIA_CXX) -I $(MEDIA_UDP_UNICAST_DIR)/include  -o  udp_unicast_send.bin  udp_unicast_sendtest.c -Wl,--start-group -L $(MEDIA_UDP_UNICAST_DIR)/lib -ludp_unicast -lrt -Wl,--end-group -Wall
	$(MEDIA_CXX) -I $(MEDIA_UDP_UNICAST_DIR)/include  -o  udp_unicast_recv.bin  udp_unicast_recvtest.c -Wl,--start-group -L $(MEDIA_UDP_UNICAST_DIR)/lib -ludp_unicast -lrt -Wl,--end-group -Wall
clean:
	rm -rf udp_unicast_send.bin udp_unicast_recv.bin
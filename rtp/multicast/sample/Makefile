all:
	$(MEDIA_CXX) -I $(MEDIA_RTP_MULTICAST_DIR)/include  -o  rtp_multicast_send.bin  rtp_multicast_sendtest.c -Wl,--start-group -L $(MEDIA_RTP_MULTICAST_DIR)/src/lib -ljthread -ljrtp -L $(MEDIA_RTP_MULTICAST_DIR)/lib -lrtp_multicast -lrt -Wl,--end-group -Wall
	$(MEDIA_CXX) -I $(MEDIA_RTP_MULTICAST_DIR)/include  -o  rtp_multicast_recv.bin  rtp_multicast_recvtest.c -Wl,--start-group -L $(MEDIA_RTP_MULTICAST_DIR)/src/lib -ljthread -ljrtp -L $(MEDIA_RTP_MULTICAST_DIR)/lib -lrtp_multicast -lrt -Wl,--end-group -Wall
clean:
	rm -rf rtp_multicast_recv.bin rtp_multicast_send.bin
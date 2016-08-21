all:
	$(MEDIA_CXX) -I $(MEDIA_RTP_UNICAST_DIR)/include -o  rtp_unicast_send.bin  rtp_unicast_sendtest.c -Wl,--start-group -L $(MEDIA_RTP_UNICAST_DIR)/src/lib -ljthread -ljrtp -L $(MEDIA_RTP_UNICAST_DIR)/lib -lrtp_unicast -lrt -Wl,--end-group -Wall
	$(MEDIA_CXX) -I $(MEDIA_RTP_UNICAST_DIR)/include -o  rtp_unicast_recv.bin  rtp_unicast_recvtest.c -Wl,--start-group -L $(MEDIA_RTP_UNICAST_DIR)/src/lib -ljthread -ljrtp -L $(MEDIA_RTP_UNICAST_DIR)/lib -lrtp_unicast -lrt -Wl,--end-group -Wall
clean:
	rm -rf rtp_unicast_send.bin rtp_unicast_recv.bin
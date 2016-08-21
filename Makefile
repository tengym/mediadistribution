.EXPORT_ALL_VARIABLES:

MEDIA_ROOT_DIR=${PWD}

# udp and rtp common source code path
MEDIA_COMMON_DIR=$(MEDIA_ROOT_DIR)/common

# udp path
MEDIA_UDP_DIR=$(MEDIA_ROOT_DIR)/udp
MEDIA_UDP_UNICAST_DIR=$(MEDIA_UDP_DIR)/unicast
MEDIA_UDP_MULTICAST_DIR=$(MEDIA_UDP_DIR)/multicast

# rtp path
MEDIA_RTP_DIR=$(MEDIA_ROOT_DIR)/rtp
MEDIA_RTP_UNICAST_DIR=$(MEDIA_RTP_DIR)/unicast
MEDIA_RTP_MULTICAST_DIR=$(MEDIA_RTP_DIR)/multicast

#complile tools
MEDIA_CC	  = arm-hisiv200-linux-gcc 
MEDIA_AR	  = arm-hisiv200-linux-ar   
MEDIA_LD	  = arm-hisiv200-linux-ld
MEDIA_RUN	  = arm-hisiv200-linux-run  
MEDIA_DB	  = arm-hisiv200-linux-gdb  
MEDIA_LINK    = arm-hisiv200-linux-gcc  
MEDIA_CPP     = arm-hisiv200-linux-cpp
MEDIA_CXX     = arm-hisiv200-linux-g++
MEDIA_RANLIB  = arm-hisiv200-linux-ranlib
MEDIA_AS      = arm-hisiv200-linux-as
MEDIA_STRIP   = arm-hisiv200-linux-strip

CFG_CFLAGS	= -Werror
CFG_CFLAGS += -Wall -O3 -g

#out
MEDIA_OUT_DIR=$(MEDIA_ROOT_DIR)/out
MEDIA_OUT_INC_DIR=$(MEDIA_OUT_DIR)/include
MEDIA_OUT_LIB_DIR=$(MEDIA_OUT_DIR)/lib
MEDIA_OUT_TEST_DIR=$(MEDIA_OUT_DIR)/test 

#====================================================================================
#			   all/clean
#====================================================================================
.PHONY: all clean
all: common_install udp_install rtp_install sample_install out_install
clean: common_uninstall udp_uninstall rtp_uninstall sample_uninstall out_uninstall

#====================================================================================
#			   common_install/common_uninstall
#====================================================================================
.PHONY: common_install common_uninstall
common_install:
	echo "make common_install"
	make -C $(MEDIA_COMMON_DIR) install
common_uninstall:
	echo "make common_uninstall"
	make -C $(MEDIA_COMMON_DIR) uninstall

#====================================================================================
#				udp_install/udp_uninstall 
#				udp_unicast_install/udp_unicast_uninstall 
#				udp_multicast_install/udp_multicast_uninstall
#====================================================================================
.PHONY: udp_install udp_uninstall
.PHONY: udp_unicast_install udp_unicast_uninstall
.PHONY: udp_multicast_install udp_multicast_uninstall
udp_install: udp_unicast_install udp_multicast_install
udp_uninstall: udp_unicast_uninstall udp_multicast_uninstall
udp_unicast_install:
	echo "make udp_unicast_install"
	make -C $(MEDIA_UDP_UNICAST_DIR) install
udp_unicast_uninstall:
	echo "make udp_unicast_uninstall"
	make -C $(MEDIA_UDP_UNICAST_DIR) uninstall
udp_multicast_install:
	echo "make udp_multicast_install"
	make -C $(MEDIA_UDP_MULTICAST_DIR) install	
udp_multicast_uninstall:
	echo "make udp_multicast_uninstall"
	make -C $(MEDIA_UDP_MULTICAST_DIR) uninstall

#====================================================================================
#				rtp_install/rtp_uninstall 
#				rtp_unicast_install/rtp_unicast_uninstall 
#				rtp_multicast_install/rtp_multicast_uninstall
#====================================================================================
.PHONY: rtp_install rtp_uninstall
.PHONY: rtp_unicast_install rtp_unicast_uninstall
.PHONY: rtp_multicast_install rtp_multicast_uninstall
rtp_install: rtp_unicast_install rtp_multicast_install
rtp_uninstall: rtp_unicast_uninstall rtp_multicast_uninstall
rtp_unicast_install:
	echo "make rtp_unicast_install"
	make -C $(MEDIA_RTP_UNICAST_DIR) install
rtp_unicast_uninstall:
	echo "make rtp_unicast_uninstall"
	make -C $(MEDIA_RTP_UNICAST_DIR) uninstall
rtp_multicast_install:
	echo "make rtp_multicast_install"
	make -C $(MEDIA_RTP_MULTICAST_DIR) install	
rtp_multicast_uninstall:
	echo "make rtp_multicast_uninstall"
	make -C $(MEDIA_RTP_MULTICAST_DIR) uninstall

#====================================================================================
#			   sample_install/sample_uninstall
#====================================================================================
.PHONY: sample_install sample_uninstall
sample_install:
	make -C $(MEDIA_RTP_UNICAST_DIR)/sample all
	make -C $(MEDIA_RTP_MULTICAST_DIR)/sample all
	make -C $(MEDIA_UDP_UNICAST_DIR)/sample all
	make -C $(MEDIA_UDP_MULTICAST_DIR)/sample all
sample_uninstall:
	make -C $(MEDIA_RTP_UNICAST_DIR)/sample clean
	make -C $(MEDIA_RTP_MULTICAST_DIR)/sample clean
	make -C $(MEDIA_UDP_UNICAST_DIR)/sample clean
	make -C $(MEDIA_UDP_MULTICAST_DIR)/sample clean

#====================================================================================
#			   out_install/out_uninstall
#====================================================================================
.PHONY: out_install out_uninstall
out_install:
	-@rm $(MEDIA_OUT_DIR) -Rf
	-@mkdir $(MEDIA_OUT_DIR) -p
	-@mkdir $(MEDIA_OUT_INC_DIR) -p
	-@mkdir $(MEDIA_OUT_LIB_DIR) -p
	-@mkdir $(MEDIA_OUT_TEST_DIR) -p
	
	-@cp $(MEDIA_UDP_UNICAST_DIR)/include/* $(MEDIA_OUT_INC_DIR) -Rf
	-@cp $(MEDIA_UDP_UNICAST_DIR)/lib/* $(MEDIA_OUT_LIB_DIR) -Rf
	-@cp $(MEDIA_UDP_MULTICAST_DIR)/include/* $(MEDIA_OUT_INC_DIR) -Rf
	-@cp $(MEDIA_UDP_MULTICAST_DIR)/lib/* $(MEDIA_OUT_LIB_DIR) -Rf
	-@cp $(MEDIA_RTP_UNICAST_DIR)/include/* $(MEDIA_OUT_INC_DIR) -Rf
	-@cp $(MEDIA_RTP_UNICAST_DIR)/lib/* $(MEDIA_OUT_LIB_DIR) -Rf
	-@cp $(MEDIA_RTP_MULTICAST_DIR)/include/* $(MEDIA_OUT_INC_DIR) -Rf
	-@cp $(MEDIA_RTP_MULTICAST_DIR)/lib/* $(MEDIA_OUT_LIB_DIR) -Rf
	-@cp $(MEDIA_COMMON_DIR)/lib/libjrtp.* $(MEDIA_OUT_LIB_DIR) -Rf
	-@cp $(MEDIA_COMMON_DIR)/lib/libjthread.* $(MEDIA_OUT_LIB_DIR) -Rf
	
	-@cp $(MEDIA_UDP_UNICAST_DIR)/sample/*.bin $(MEDIA_OUT_TEST_DIR) -Rf
	-@cp $(MEDIA_UDP_MULTICAST_DIR)/sample/*.bin $(MEDIA_OUT_TEST_DIR) -Rf
	-@cp $(MEDIA_RTP_UNICAST_DIR)/sample/*.bin $(MEDIA_OUT_TEST_DIR) -Rf
	-@cp $(MEDIA_RTP_MULTICAST_DIR)/sample/*.bin $(MEDIA_OUT_TEST_DIR) -Rf

out_uninstall:
	-@rm $(MEDIA_OUT_DIR) -Rf
	
#====================================================================================
#		help
#====================================================================================
.PHONY:  help
GREEN="\e[32;1m"
NORMAL="\e[39m"
RED="\e[31m"
AT := @

help:
	$(AT)echo -e ${GREEN}
	$(AT)cat Readme|less
	$(AT)echo -e ${NORMAL}
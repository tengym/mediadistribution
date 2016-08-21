MEDIA_COMMON_JRTP_DIR=$(MEDIA_COMMON_DIR)/jrtplib-3.9.1
MEDIA_COMMON_JTHREAD_DIR=$(MEDIA_COMMON_DIR)/jthread-1.3.1
MEDIA_CMAKE_FILE=CMakeLists.txt
MEDIA_CMAKE_CACHE_FILE=CMakeCache.txt

#====================================================================================
#			   install/uninstall
#====================================================================================
.PHONY: install uninstall
install:
	cd $(MEDIA_COMMON_JTHREAD_DIR)
	echo $(MEDIA_COMMON_JTHREAD_DIR)
	-rm $(MEDIA_COMMON_JTHREAD_DIR)/$(MEDIA_CMAKE_CACHE_FILE)
	cmake $(MEDIA_COMMON_JTHREAD_DIR)/$(MEDIA_CMAKE_FILE)
	make -C  $(MEDIA_COMMON_JTHREAD_DIR)
	make -C  $(MEDIA_COMMON_JTHREAD_DIR) install
	
	cd ..
	
	cd $(MEDIA_COMMON_JRTP_DIR)
	echo $(MEDIA_COMMON_JRTP_DIR)
	-rm $(MEDIA_COMMON_JRTP_DIR)/$(MEDIA_CMAKE_CACHE_FILE)
	cmake $(MEDIA_COMMON_JRTP_DIR)/$(MEDIA_CMAKE_FILE)
	make -C  $(MEDIA_COMMON_JRTP_DIR)
	make -C  $(MEDIA_COMMON_JRTP_DIR) install

uninstall:
	cd $(MEDIA_COMMON_JTHREAD_DIR)
	echo $(MEDIA_COMMON_JTHREAD_DIR)
	make -C  $(MEDIA_COMMON_JTHREAD_DIR) clean
	-rm $(MEDIA_COMMON_JTHREAD_DIR)/$(MEDIA_CMAKE_CACHE_FILE)
	
	cd ..
	
	cd $(MEDIA_COMMON_JRTP_DIR)
	echo $(MEDIA_COMMON_JRTP_DIR)
	make -C  $(MEDIA_COMMON_JRTP_DIR) clean
	-rm $(MEDIA_COMMON_JRTP_DIR)/$(MEDIA_CMAKE_CACHE_FILE)
	
	cd ..
	
	rm -f include -R
	rm -f lib -R
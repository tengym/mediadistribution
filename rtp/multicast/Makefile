#target
TARGET:=rtp_multicast
LIBS := lib$(TARGET)

#source
OBJ_DIR  	 := ./src/obj
LIB_DIR  	 := ./lib
INC_DIR		 := ./include
SPATH 	 	 := ./src
INCFLAGS 	 := -I $(SPATH)/include -I $(SPATH)/include/jthread -I $(SPATH)/include/jrtplib3 -I $(SPATH)
LIBFLAGS 	 := -L $(SPATH)/lib
EXTEA_LIB 	 := -Wl,--start-group -ljthread -ljrtp -Wl,--end-group
EXTRA_CFLAGS := $(INCFLAGS) $(LIBFLAGS) $(EXTEA_LIB)
SOURCES  	 := $(notdir $(shell find $(SPATH) -maxdepth 1 -name "*.cpp"))
OBJECTS  	 := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

#====================================================================================
#			   install/uninstall
#====================================================================================
.PHONY: install uninstall multicast_prepare multicast_build
install: multicast_prepare multicast_build

multicast_prepare:
	-@rm $(SPATH)/include -Rf
	-@rm $(SPATH)/lib -Rf
	-@cp $(MEDIA_COMMON_DIR)/include $(SPATH) -Rf
	-@cp $(MEDIA_COMMON_DIR)/lib $(SPATH) -Rf
	-@mkdir $(OBJ_DIR) -p
	-@mkdir $(LIB_DIR) -p
	-@mkdir $(INC_DIR) -p

multicast_build:$(LIBS).a $(LIBS).so

.c.o:
	@echo  CC   $< ...
	@$(MEDIA_CC) $(EXTRA_CFLAGS) $< -o $@

.cc.o: 
	@echo  CC  $< ...
	@$(MEDIA_CC) $(EXTRA_CFLAGS) $< -o $@

.cpp.o:
	@echo  CPP  $< ...
	@$(MEDIA_CXX) $(EXTRA_CFLAGS) $< -o $@

.cxx.o: 
	@echo  CXX  $< ...
	@$(MEDIA_CXX) $(EXTRA_CFLAGS) $< -o $@

$(OBJ_DIR)/%.o:$(SPATH)/%.cpp
	@echo CXX $< ...
	-@$(MEDIA_CXX) $(EXTRA_CFLAGS) $< -c -o $@
	
$(LIBS).a: $(OBJECTS)
	$(MEDIA_AR) -rcs -o $@ $^	
	-@mv $(LIBS).a $(LIB_DIR)

$(LIBS).so:$(OBJECTS)
	-@$(MEDIA_CXX) -shared -fPCI -o $@ $^
	-@mv $(LIBS).so $(LIB_DIR)
	-@cp $(SPATH)/rtp_multicast_api.h $(INC_DIR)
	-@rm $(OBJ_DIR) -Rf
	
uninstall:
	-@rm include -Rf
	-@rm lib -Rf
	-@rm $(SPATH)/include -Rf
	-@rm $(SPATH)/lib -Rf
	-@rm $(OBJ_DIR) -Rf

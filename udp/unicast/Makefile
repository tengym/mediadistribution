#target
TARGET:=udp_unicast
LIBS := lib$(TARGET)

#source
OBJ_DIR  	 := ./src/obj
LIB_DIR  	 := ./lib
INC_DIR		 := ./include
SPATH 	 	 := ./src
INCFLAGS 	 := -I $(SPATH)
EXTRA_CFLAGS := $(INCFLAGS)
SOURCES  	 := $(notdir $(shell find $(SPATH) -maxdepth 1 -name "*.cpp"))
OBJECTS  	 := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

#====================================================================================
#			   install/uninstall
#====================================================================================
.PHONY: install uninstall unicast_prepare unicast_build
install: unicast_prepare unicast_build

unicast_prepare:
	-@mkdir $(OBJ_DIR) -p
	-@mkdir $(LIB_DIR) -p
	-@mkdir $(INC_DIR) -p

unicast_build:$(LIBS).a $(LIBS).so

.c.o:
	@echo  CC   $< ...
	@$(MEDIA_CXX) $(EXTRA_CFLAGS) $< -o $@

.cc.o: 
	@echo  CC  $< ...
	@$(MEDIA_CXX) $(EXTRA_CFLAGS) $< -o $@

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
	$(MEDIA_AR) -r -o $@ $^	
	-@mv $(LIBS).a $(LIB_DIR)

$(LIBS).so:$(OBJECTS)
	-@$(MEDIA_CXX) -shared -fPCI -o $@ $^
	-@mv $(LIBS).so $(LIB_DIR)
	-@cp $(SPATH)/udp_unicast_api.h $(INC_DIR)
	-@rm $(OBJ_DIR) -Rf
	
uninstall:
	-@rm include -Rf
	-@rm lib -Rf
	-@rm $(OBJ_DIR) -Rf

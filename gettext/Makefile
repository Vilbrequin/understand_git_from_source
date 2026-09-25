# Compiler Configuration
SHELL			:=/bin/sh
CC			:=gcc

# Output and Build Directories
TARGET			:=multi_lang_utility
SRC			:=$(TARGET).c

# msgfmt configuration
BIND_TEXT_DOMAINE_PATH	:=/usr/share/locale
FR			:=fr
ES			:=es
DOMAINE			:=my-domaine.mo
PO_FILE			:=msg.po


$(TARGET): $(SRC) 
	$(CC) $< -o $@

create_machine_obj:
	@echo "COMPILING TO MACHINE OBJECTS"
	msgfmt $(FR)-$(PO_FILE) -o $(BIND_TEXT_DOMAINE_PATH)/$(FR)/LC_MESSAGES/$(DOMAINE) 
	msgfmt $(ES)-$(PO_FILE) -o $(BIND_TEXT_DOMAINE_PATH)/$(ES)/LC_MESSAGES/$(DOMAINE)

clean:
	rm -f $(TARGET)
	rm -f $(BIND_TEXT_DOMAINE_PATH)/$(FR)/LC_MESSAGES/$(DOMAINE)
	rm -f $(BIND_TEXT_DOMAINE_PATH)/$(ES)/LC_MESSAGES/$(DOMAINE)
all: $(TARGET)

.PHONY: all clean

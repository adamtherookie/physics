LD = g++
CC = g++

CFLAGS := \
	-Isrc/include \
	-Wall \
	-Wextra \

BLUE=\033[0;34m
GREEN=\033[;32m
YELLOW=\033[;33m
NC=\033[0m

CFILES := $(shell find src/ -name '*.cpp')
OFILES := $(CFILES:.cpp=.o)

TARGET = fiziks

all: clean compile

compile: ld
	@ echo -e "Done!"
	
ld: $(OFILES)
	@ echo -e "${GREEN}[ LD ]${NC} + $^"
	@ $(LD) $^ -o $(TARGET)

%.o: %.c
	@ echo -e "${BLUE}[ CC ]${NC} + $<"
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@ echo -e "${YELLOW}[ CLEAN ]${NC}"
	@ rm -rf $(OFILES) $(TARGET)

run:
	@ ./fiziks

install:
	@ cp ./fiziks /usr/bin
	@ echo "Installed!"


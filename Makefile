CC := gcc
CFLAGS := -Wall -Wextra -O2

TARGET_READER := keyboard_reader
TARGET_FAKE   := fake_input

SRC_READER := main.c
SRC_FAKE   := fake_input.c

.PHONY: all clean run_fake run_reader

all: $(TARGET_READER) $(TARGET_FAKE)

$(TARGET_READER): $(SRC_READER)
	$(CC) $(CFLAGS) -o $(TARGET_READER) $(SRC_READER)

$(TARGET_FAKE): $(SRC_FAKE)
	$(CC) $(CFLAGS) -o $(TARGET_FAKE) $(SRC_FAKE)

run_fake: $(TARGET_FAKE)
	./$(TARGET_FAKE)

run_reader: $(TARGET_READER)
	./$(TARGET_READER) /tmp/fake_event

clean:
	rm -f $(TARGET_READER) $(TARGET_FAKE)

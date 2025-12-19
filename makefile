# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -std=c99

# Target Executable
TARGET = bigchallenge

# Source Files
SRCS = main.c parser.c storage.c

# Rule 'all' adalah default
all: $(TARGET)

# Rule untuk membuat executable langsung dari semua file .c
# Ini menghindari pembuatan file .o terpisah secara permanen
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Rule untuk membersihkan file hasil kompilasi (untuk maintenance)
clean:
	rm -f $(TARGET) output.bin

# Jalankan test (opsional)
run: $(TARGET)
	./$(TARGET) Data-50K.txt

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
LDLIBS = -lm

TARGET = artemis
SOURCES = main.c simulador.c validaciones.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

rebuild: clean all
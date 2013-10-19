
CC = i686-pc-mingw32-gcc

INCLUDE = -I/usr/local/include -I/usr/local/include/cairo
CFLAGS = -mwindows -Wall -g -O2 $(INCLUDE)

LDFLAGS = -static -L/usr/local/lib

WIN32LIBS = -lgdi32
LIBS = -lcairo -lpng -lz $(WIN32LIBS)

TARGET = emu.exe

emu-objs = emulator.o

$(TARGET) : $(emu-objs)
	$(CC) -o $(TARGET) $(emu-objs) $(LDFLAGS) $(LIBS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm -f $(TARGET) $(emu-objs)

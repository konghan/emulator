
CC = i686-pc-mingw32-gcc

INCLUDE = -I/usr/local/include				\
		  -I/usr/local/include/cairo		\
		  -I../../swapi/win32				\
		  -I../../swapi/include				\
		  -I..

CFLAGS = -mwindows -Wall -g -O2 $(INCLUDE)

LDFLAGS = -static -L/usr/local/lib -L../libs

WIN32LIBS = -lgdi32
LIBS = -lswapi -lcairo -lpng -lz $(WIN32LIBS)

TARGET = emu.exe

emu-objs = emu-shell.o native_graphic.o native_logger.o emulator.o
win-objs = win32_native.o

vpath %.c ./..

$(TARGET) : $(emu-objs) $(win-objs)
	$(CC) -o $(TARGET) $(emu-objs) $(win-objs) $(LDFLAGS) $(LIBS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm -f $(TARGET) $(emu-objs) $(win-objs)

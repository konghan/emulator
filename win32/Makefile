
CC = i686-pc-mingw32-gcc

INCLUDE = -I../../swapi/win32				\
		  -I../../swapi/include				\
		  -I..

CFLAGS = -mwindows -Wall -g -O2 $(INCLUDE)

LDFLAGS = -static -L../libs

WIN32LIBS = -lgdi32 -lmsimg32
LIBS = -lswapi -lcairo -lpixman -lfreetype -lpng -lz $(WIN32LIBS)

TARGET = emu.exe

emu-objs = emu-keyboard.o emu-shell.o						\
		   natv_surface.o natv_logger.o					\
		   emulator.o

win-objs = win32_native.o

vpath %.c ./..

$(TARGET) : $(emu-objs) $(win-objs)
	$(CC) -o $(TARGET) $(emu-objs) $(win-objs) $(LDFLAGS) $(LIBS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm -f $(TARGET) $(emu-objs) $(win-objs)

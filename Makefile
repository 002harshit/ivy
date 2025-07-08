# PLATFORM_MINGW
# PLATFORM_LINUX

PLATFORM ?= PLATFORM_LINUX

SOURCES = ivy_stl.c ivy_wnd.c ivy_gfx.c
OBJECTS = ivy_stl.o ivy_wnd.o ivy_gfx.o

ifeq ($(PLATFORM), PLATFORM_MINGW)
	CC = x86_64-w64-mingw32-gcc
	AR = x86_64-w64-mingw32-ar
else 
	CC ?= clang
	AR ?= ar
	SOURCES+= backends/ivy_x11.c backends/ivy_alsa.c
	OBJECTS+= backends/ivy_x11.o backends/ivy_alsa.o
endif

libivy.a: $(OBJECTS)
	$(AR) r libivy.a $^

$(OBJECTS): %.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm -fv $(OBJECTS)

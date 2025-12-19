# BASEDIR= (set by gnubg3D.bat)
# GNUDIR= (set by gnubg3D.bat)
LIBFLAGS=-mwindows
CPPFLAGS=-mms-bitfields -Wall -O -DHAVE_FTGL=1 $(INCLUDE)
AR=ar cr
RANLIB=ranlib
CC=gcc

INCLUDE= \
	-I$(BASEDIR)/include/gtk \
	-I$(BASEDIR)/lib/gtk+/include \
	-I$(BASEDIR)/include/glib-2.0 \
	-I$(BASEDIR)/lib/glib-2.0/include \
	-I$(BASEDIR)/include/freetype2 \
	-I$(BASEDIR)/include/FTGL \
	-I$(BASEDIR)/include/gtkgl \
	-I$(GNUDIR) \
	-I$(GNUDIR)/lib \
	-I$(GNUDIR)/board3d

LIBS= -lftgl -lglib-2.0.dll -lgtkgl -lgobject-2.0.dll \
	-lgdkglext-win32-1.0.dll -lgtkglext-win32-1.0.dll -lfreetype \
	-lopengl32 -lglu32 -lglut32 -lstdc++

TARGET=libboard.a

SOURCE=	drawboard3d misc3d graph texture3d font3d widget3d \
	matrix model shadow mylist gtkcolour3d
OBJECTS=$(patsubst %,%.o,$(SOURCE)) $(GNUDIR)/luxi.o
HEADERS=../gnubg/gtkboard.h inc3d.h test/main.h

all: $(TARGET)
	@echo done

$(TARGET): $(OBJECTS)
	@echo creating lib
	$(AR) $(TARGET) $(OBJECTS)
	$(RANLIB) $(TARGET)

CC      = gcc
CFLAGS  = -mms-bitfields -Os -Wall $(DEFS) $(INCLUDE)
DEFS    = -DHAVE_CONFIG_H
INCLUDE = -I. -I.. 
AR      = ar cr
RANLIB  = ranlib
TARGET  = libevent.a

SRCS = dynarray.c hash.c heap.c list.c \
	isaac.c md5.c mt19937int.c neuralnet.c

OBJS = dynarray.o hash.o heap.o list.o \
	isaac.o md5.o mt19937int.o neuralnet.o

$(TARGET): $(OBJS)
	$(AR) $(TARGET) $(OBJS)
	$(RANLIB) $(TARGET)

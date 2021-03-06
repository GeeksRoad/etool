###
 # Copyright 2017, PaPa.  All rights reserved.
 # Config
###

#CROSS : Makefile define

CPP	= @echo g++	$@; $(CROSS)g++
CC	= @echo gcc	$@; $(CROSS)gcc
LD	= @echo ld 	$@; $(CROSS)ld
AR	= @echo ar 	$@; $(CROSS)ar
RM	= @echo rm 	$@; rm -rf
STRIP	= @echo strip 	$@; $(CROSS)strip

CFLAGS = -D_$(SYSTEM)
CFLAGS += -Wall -O3 -Os -fPIC
CFLAGS += -D_REENTRANT
#CFLAGS += -frtti -fexceptions
CFLAGS_DBG := -g -ggdb

AFLAGS += -r
#LDFLAGS := -lc
ifeq ($(SYSTEM), linux)
LDFLAGS += -lpthread -ldl -lrt
else ifeq ($(SYSTEM), mac)
LDFLAGS += -lpthread -ldl
else ifeq ($(SYSTEM), android)
LDFLAGS += -lpthread -ldl -lrt
else ifeq ($(SYSTEM), ios)
LDFLAGS += -lpthread -ldl
else ifeq ($(SYSTEM), windows)
LDFLAGS += -lws2_32
endif

LIBDIR  := ./lib/$(CROSS)
BINDIR  := ./bin/$(CROSS)

TARGET_A = $(LIBDIR)/libetool.a
TARGET_SO = $(LIBDIR)/libetool.so
TARGET_DEBUG_A = $(LIBDIR)/libetool.debug.a
TARGET_DEBUG_SO = $(LIBDIR)/libetool.debug.so
TARGET_TEST = $(BINDIR)/etool

LIB_SRCS_C += $(foreach file, $(wildcard $(SRCS_PATH)/*), $(wildcard $(file)/*.c))
LIB_SRCS_CPP += $(foreach file, $(wildcard $(SRCS_PATH)/*), $(wildcard $(file)/*.cpp))
TEST_SRCS += $(foreach file, $(TEST_SRCS_PATH), $(wildcard $(file)/*.cpp))


COMPILE_PATH = $(LIBDIR)/.objs
LIB_OBJS += $(patsubst %.c, $(COMPILE_PATH)/%.o, $(LIB_SRCS_C)) 
LIB_OBJS += $(patsubst %.cpp, $(COMPILE_PATH)/%.o, $(LIB_SRCS_CPP))
LIB_DEBUG_OBJS += $(patsubst %.c, $(COMPILE_PATH)/%.debug.o, $(LIB_SRCS_C)) 
LIB_DEBUG_OBJS += $(patsubst %.cpp, $(COMPILE_PATH)/%.debug.o, $(LIB_SRCS_CPP))
TEST_OBJS += $(patsubst %.cpp, $(COMPILE_PATH)/%.o, $(TEST_SRCS))

$(shell mkdir -p $(LIBDIR))
$(shell mkdir -p $(BINDIR))
$(foreach dir, $(wildcard $(SRCS_PATH)/*), $(shell mkdir -p $(COMPILE_PATH)/$(dir)))
$(shell mkdir -p $(COMPILE_PATH)/$(TEST_SRCS_PATH))

#compile
all : $(TARGET_A) $(TARGET_SO) $(TARGET_TEST)
debug :  $(TARGET_DEBUG_A) $(TARGET_DEBUG_SO)
$(VERSION) :
	echo The gitversion :  && git reflog

$(TARGET_A) : $(VERSION) $(LIB_OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $(LIB_OBJS)

$(TARGET_SO) : $(VERSION) $(LIB_OBJS)
	$(RM) $@;
	$(CC) -o $@ $(LIB_OBJS) $(LDFLAGS) -shared

$(TARGET_DEBUG_A) : $(VERSION) $(LIB_DEBUG_OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $(LIB_DEBUG_OBJS)

$(TARGET_DEBUG_SO) : $(VERSION) $(LIB_DEBUG_OBJS)
	$(RM) $@;
	$(CC) -o $@ $(LIB_DEBUG_OBJS) $(LDFLAGS) -shared

$(TARGET_TEST) : $(TEST_OBJS) $(LIB_DEBUG_OBJS)
	$(RM) $@;
	$(CPP) -o $@ $^ $(LDFLAGS)

$(COMPILE_PATH)/%.o : %.c
	$(CC) -c $(CFLAGS) -I $(INCLUDES) $< -o $@

$(COMPILE_PATH)/%.o : %.cpp
	$(CPP) -c $(CFLAGS) -I $(INCLUDES) $< -o $@

$(COMPILE_PATH)/%.debug.o : %.c
	$(CC) -c $(CFLAGS) $(CFLAGS_DBG) -I $(INCLUDES) $< -o $@

$(COMPILE_PATH)/%.debug.o : %.cpp
	$(CPP) -c $(CFLAGS) $(CFLAGS_DBG) -I $(INCLUDES) $< -o $@

clean :
	$(RM) $(TARGET_A) $(TARGET_SO) $(COMPILE_PATH)

#flint :
#	echo $(LIB_SRCS_C) $(LIB_SRCS_CPP)
#	@flexelint -i $(FLINTDIR) config.lnt $(LIB_SRCS_C) $(LIB_SRCS_CPP) > $(FLINTDIR)/lint.rs

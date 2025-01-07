#
# Makefile for 'sdckeyboard'
#

# overide this with something like `HIDAPI_DIR=../hidapi-libusb make`
HIDAPI_DIR ?= ../hidapi

# try to do some autodetecting
UNAME := $(shell uname -s)
ARCH := $(shell uname -m)

ifeq "$(UNAME)" "Linux"
	OS=linux
endif


# construct version string from git tag
# allow overriding of GIT_TAG for automated builds
# If we have a file .git-tag (from source archive), read it
ifneq ($(wildcard .git-tag),)
	GIT_TAG_RAW=$(file <.git-tag)
endif
GIT_TAG_RAW?=$(strip $(shell git tag 2>&1 | tail -1 | cut -f1 -d' '))
# deal with case of no git or no git tags, check for presence of "v" (i.e. "v1.93")
ifneq ($(findstring v,$(GIT_TAG_RAW)), v)
	GIT_TAG_RAW:="v$(strip $(shell date -r . +'%Y%m%d' ))"
endif
GIT_TAG?="$(GIT_TAG_RAW)"
SDCKEYBOARD_VERSION?="$(GIT_TAG)"

############ Linux (hidraw)
ifeq "$(OS)" "linux"

PKGS = libudev

ifneq ($(wildcard $(HIDAPI_DIR)),)
OBJS = $(HIDAPI_DIR)/linux/hid.o
else
PKGS += hidapi-hidraw hidapi-libusb
endif

CFLAGS += $(shell pkg-config --cflags $(PKGS))
LIBS = $(shell pkg-config --libs $(PKGS))
EXE=

endif

############# common

CFLAGS += -I $(HIDAPI_DIR)/hidapi
CFLAGS += -DSDCKEYBOARD_VERSION=\"$(SDCKEYBOARD_VERSION)\"
OBJS += sdckeyboard.o

all: sdckeyboard

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


sdckeyboard: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o sdckeyboard$(EXE) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f sdckeyboard$(EXE)

package: sdckeyboard$(EXE)
	@echo "Packaging up sdckeyboard for '$(OS)-$(ARCH)'"
	zip sdckeyboard.zip sdckeyboard$(EXE)

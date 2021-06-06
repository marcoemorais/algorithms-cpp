PWD=$(shell pwd)
NAME=$(shell basename $(PWD))
IMAGE=$(NAME)
TAG=local
ACTION?=test

DIRS :=

.PHONY: $(DIRS) all-clean

all:: $(DIRS)

$(DIRS):
	$(MAKE) -C $@ $(ACTION)

all-clean:
	$(MAKE) ACTION=clean

include Makefile.defs

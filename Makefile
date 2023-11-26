PWD=$(shell pwd)
NAME=$(shell basename $(PWD))
IMAGE=$(NAME)
TAG=local
ACTION?=test

DIRS := \
	01-partition \
	02-quicksort \
	03-rselect \
	04-toposort \
	05-dijkstra-shortest-path \
	06-bsearch \
	07-perms \
	08-powerset \
	09-bst-ops \
	10-tree-ops \
	11-prefix-sum \
	12-monte-carlo-pi \
	13-coin-change \
	14-bisection \
	15-horners-rule \
	16-remove \
	17-unique \
	18-rotate \
	19-combos \
	20-nqueens \
	21-fastpow \
	22-rolling-hash

.PHONY: $(DIRS) all-clean

all:: $(DIRS)

$(DIRS):
	$(MAKE) -C $@ $(ACTION)

all-clean:
	$(MAKE) ACTION=clean

include Makefile.defs

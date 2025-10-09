CC = gcc
CFLAGS = -g -O0

all: range

range: range.c
	$(CC) $(CFLAGS) -o range range.c

test: range gdb_script1.gdb gdb_script2.gdb
	@echo "===== Running Test 1: ./range 1 12 (output when current is divisible by 5) ====="
	@gdb --batch --quiet -x gdb_script1.gdb range
	@echo ""
	@echo "===== Running Test 2: ./range -100 100 3 (output for elements 28-35) ====="
	@gdb --batch --quiet -x gdb_script2.gdb range

clean:
	rm -f range

.PHONY: all test clean

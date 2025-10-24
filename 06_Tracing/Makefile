CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = move
LIBRARY = protect.so

all: $(TARGET) $(LIBRARY)

$(TARGET): move.c
        $(CC) $(CFLAGS) -o $(TARGET) move.c

$(LIBRARY): protect.c
        $(CC) -shared -fPIC $(CFLAGS) -o $(LIBRARY) protect.c -ldl

test: $(TARGET)
        @echo "=== Test 1: Normal move ==="
        @echo "test content" > test_in.txt
        @./$(TARGET) test_in.txt test_out.txt && echo "PASS: moved" || echo "✗ FAIL"
        @test -f test_out.txt && echo "PASS: output exists" || echo "✗ FAIL"
        @test ! -f test_in.txt && echo "PASS: input deleted" || echo "✗ FAIL"
        @rm -f test_out.txt
        @echo ""
        @echo "=== Test 2: Destination exists ==="
        @echo "source" > test_in.txt
        @echo "dest" > test_out.txt
        @./$(TARGET) test_in.txt test_out.txt 2>/dev/null && echo "✗ FAIL" || echo "PASS: error as expected"
        @rm -f test_in.txt test_out.txt
        @echo ""
        @echo "=== Test 3: Source doesn't exist ==="
        @./$(TARGET) nonexistent.txt test_out.txt 2>/dev/null && echo "✗ FAIL" || echo "PASS: error as expected"
        @echo ""
        @echo "=== Test 4: LD_PRELOAD - PROTECT in filename ==="
        @echo "content" > file_PROTECT_name.txt
        @echo "normal" > normal_file.txt
        @LD_PRELOAD=./$(LIBRARY) ./$(TARGET) file_PROTECT_name.txt moved1.txt 2>&1 | grep -q "PROTECT" && echo "PASS: PROTECT filename blocked" || echo "✗ FAIL"
        @test -f file_PROTECT_name.txt && echo "PASS: PROTECT file still exists" || echo "✗ FAIL"
        @LD_PRELOAD=./$(LIBRARY) ./$(TARGET) normal_file.txt moved2.txt && echo "PASS: normal file moved" || echo "✗ FAIL"
        @test ! -f normal_file.txt && echo "PASS: normal file deleted" || echo "✗ FAIL"
        @test -f moved2.txt && echo "PASS: normal file created" || echo "✗ FAIL"
        @rm -f file_PROTECT_name.txt moved1.txt moved2.txt

clean:
        rm -f $(TARGET) $(LIBRARY) *.o test_*.txt file_PROTECT*.txt normal_file.txt moved*.txt

.PHONY: all test clean

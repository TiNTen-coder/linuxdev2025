TRASH = *.o *~ o.* 
TARGET = main main1 main2 main3
GENERATES = $(TARGET) main1.c main2.c main3.c
CFLAGS= -Wall -Wextra -O0 -g
OFILES= main.o main1.o main2.o main3.o


all: $(TARGET)

run: $(TARGET)
	./main
	./main1 8
	./main2 " @" 5
	./main3 42 ".#" 6 

%.o: %.c
	cc $(CFLAGS) $< -c -o $@ 

$(TARGET): $(OFILES)
	cc $(CFLAGS) $@.o -o $@

main1.c: main.c
	patch main.c 0001-Add-first-argument.patch -o main1.c

main2.c: main1.c
	patch main1.c -p2 0002-Add-custom-walls-and-passways.patch -o main2.c

main3.c: main2.c
	patch main2.c 0003-Add-seed-as-argument.patch -o main3.c

clean:
	rm -f $(TRASH) $(GENERATES)

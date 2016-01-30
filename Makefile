
VPATH=:./inc/:./src/:./lib

TARGET=LibExtractor

SRC=libextract.o do_extract.o do_list.o do_help.o

HEADER= -I ./inc

SLIB=./lib/libz.a

FLAGS=-g -Wall

$(TARGET) : CFLAGS=-g -Wall

$(TARGET) : $(SRC)
	gcc -static $(CFLAGS) -o $@ $^ $(HEADER) $(SLIB)

%.o : %.c
	gcc -c $(CFLAGS) $< $(HEADER)
clean :
	rm -f $(TARGET)
	rm -f *.o

rmbackup : 
	rm -f *~
	rm -f ./inc/*~
	rm -f ./src/*~


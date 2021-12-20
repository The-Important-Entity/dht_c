TARGET   = tests
CXXFLAGS = -O3 -Wall -Werror -pedantic-errors -fmessage-length=0
OBJPATH = bin
LIBPATH = lib

all: init linkedlist
	gcc $(CXXFLAGS) $(TARGET).c -o $(TARGET) $(OBJPATH)/linkedlist.o

linkedlist:
	gcc $(CXXFLAGS) -c -o $(OBJPATH)/linkedlist.o $(LIBPATH)/linkedlist.c 

init:
	@mkdir -p $(OBJPATH)
	
clean:
	@rm -r $(OBJPATH) $(TARGET)
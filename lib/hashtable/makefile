TARGET   = tests
CXXFLAGS = -O3 -Wall -Werror -pedantic-errors -fmessage-length=0
OBJPATH = bin
LIBPATH = lib

all: init table
	gcc $(CXXFLAGS) $(TARGET).c -o $(TARGET) $(OBJPATH)/linkedlist.o $(OBJPATH)/table.o


table: linkedlist
	gcc $(CXXFLAGS) -c -o $(OBJPATH)/table.o $(LIBPATH)/table.c
	
linkedlist:
	@make -C $(LIBPATH)/linkedlist
	@mv $(LIBPATH)/linkedlist/bin/*.o ./bin/
	@make clean -C $(LIBPATH)/linkedlist

init:
	@mkdir -p $(OBJPATH)

clean:
	@rm -r bin $(TARGET)
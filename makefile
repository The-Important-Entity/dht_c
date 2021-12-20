TARGET   = main
CXXFLAGS = -O3 -Wall -Werror -pedantic-errors -fmessage-length=0 -pthread
OBJPATH = bin
LIBPATH = lib

all: init hashtable sem workers underlay overlay dht
	gcc $(CXXFLAGS) $(TARGET).c -o $(TARGET) $(OBJPATH)/linkedlist.o $(OBJPATH)/table.o $(OBJPATH)/underlay.o $(OBJPATH)/overlay.o $(OBJPATH)/dht.o $(OBJPATH)/sem.o $(OBJPATH)/workers.o
	
hashtable:
	@make -C $(LIBPATH)/hashtable
	@mv $(LIBPATH)/hashtable/bin/*.o ./bin/
	@make clean -C $(LIBPATH)/hashtable

underlay:
	gcc $(CXXFLAGS) -c -o $(OBJPATH)/underlay.o $(LIBPATH)/underlay.c

overlay:
	gcc $(CXXFLAGS) -c -o $(OBJPATH)/overlay.o $(LIBPATH)/overlay.c 

dht:
	gcc $(CXXFLAGS) -c -o $(OBJPATH)/dht.o $(LIBPATH)/dht.c

sem:
	gcc $(CXXFLAGS) -c -o $(OBJPATH)/sem.o $(LIBPATH)/sem.c

workers:
	gcc $(CXXFLAGS) -c -o $(OBJPATH)/workers.o $(LIBPATH)/workers.c

init:
	@mkdir -p $(OBJPATH)

clean:
	@rm -r bin $(TARGET)
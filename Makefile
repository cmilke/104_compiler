OBJECTS  = auxlib.o stringset.o
COMPILE  = g++ -g -O0 -Wall -Wextra -std=gnu++14 -o
PRODUCTS = *.str 


all: oc.cpp deps
	$(COMPILE) oc oc.cpp $(OBJECTS) 

clean:
	-rm $(OBJECTS) $(PRODUCTS) 2> /dev/null

spotless: clean
	-rm oc 2> /dev/null

deps: $(OBJECTS)

auxlib.o: auxlib.cpp auxlib.h
	$(COMPILE) auxlib.o -c auxlib.cpp
	
stringset.o: stringset.cpp stringset.h
	$(COMPILE) stringset.o -c stringset.cpp

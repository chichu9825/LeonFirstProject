CC := gcc
target := log_parser

OBJECTS := parser2.o

$(target): $(OBJECTS) 
	$(CC) -o $(target) $(OBJECTS) 

.PHONY: clean
clean:
	-rm -rf $(target) $(OBJECTS)

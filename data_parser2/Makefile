CC := gcc
target := log_parser.run

OBJECTS := parser.o

$(target): $(OBJECTS) 
	$(CC) -o $(target) $(OBJECTS) 

.PHONY: clean
clean:
	-rm -rf $(target) $(OBJECTS)

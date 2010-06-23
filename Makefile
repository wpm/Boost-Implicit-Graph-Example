TARGET = implicit
OBJECTS = main.o
CXXFLAGS = -g -I/opt/local/include

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) -o $@

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS)

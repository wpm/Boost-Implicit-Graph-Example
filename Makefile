BOOST_PATH=/opt/local/include

TARGET = implicit
OBJECTS = main.o implicit.o
CXXFLAGS = -g -I$(BOOST_PATH) -Wall -Werror -O3

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) -o $@

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS)

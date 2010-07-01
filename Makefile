PACKAGE=implicit
VERSION=1.0.0
ARCHIVE_NAME=$(PACKAGE)-$(VERSION)

BOOST_PATH=/opt/local/include

FILES=LICENSE_1_0.txt Makefile README.md implicit.hpp main.cpp

TARGET = implicit
OBJECTS = main.o
CXXFLAGS = -g -I$(BOOST_PATH) -Wall -Werror -O3

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) -o $@

.PHONY : dist
dist: $(FILES)
	mkdir $(ARCHIVE_NAME)
	ln $(FILES) $(ARCHIVE_NAME)
	tar -czv -f $(PACKAGE)-$(VERSION).tar.gz $(ARCHIVE_NAME)
	-rm -rf $(ARCHIVE_NAME)

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS)

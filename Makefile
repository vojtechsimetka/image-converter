
# Makefile
# @author: xsimet00
# @author: xskota05
# @author: xsirok07

PROGRAM=ImageConvertor
ARCHIVE=xsimet00_xsirok07_xskota05.zip

CC=g++
CFLAGS=-Wall -O3 -v `pkg-config --cflags opencv` -I/usr/local/include -lopencv_core -lopencv_imgproc -lopencv_highgui
CFILES=$(shell find . -name '*.cpp')
OBJFILES=$(CFILES:.cpp=.o)
DOXOUT=doc
DOXCONF=doxygen.conf

# Initial rule
all: $(PROGRAM)

# Universal rules for generating object files
%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

# Dependency generator
dep: 
	$(CC) -MM *.cpp > depependency.list

# Includes dependencies
-include dependency.list

# Linking object files into executable binary file
$(PROGRAM): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@
	
# Run
run: $(PROGRAM)
	./$(PROGRAM)

# Pack
pack: clean
	@rm -f $(ARCHIVE)
	zip -r $(ARCHIVE) *.h *.cpp Makefile README dokumentace.pdf $(DOXCONF)

# Clean
clean:
	@rm -rf $(ARCHIVE) *.o $(TEST_FOLDER) $(DOXOUT) $(PROGRAM) test

test:
	./test.sh

# Doc
doc: *.cpp *.h $(DOXCONF)
	@doxygen $(DOXCONF)

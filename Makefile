CXXFLAGS=`pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`

# Makefile
# @author: xsimet00

PROGRAM=filter
ARCHIVE=xsimet00.zip

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

pack: clean
	@rm -f $(ARCHIVE)
	zip -r $(ARCHIVE) *.h *.cpp Makefile README documentation.pdf $(DOXCONF)

clean:
	@rm -rf $(ARCHIVE) *.o $(TEST_FOLDER) $(DOXOUT) $(PROGRAM)

doc: *.cpp *.h $(DOXCONF)
	@doxygen $(DOXCONF)

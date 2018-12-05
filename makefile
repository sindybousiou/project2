CC = g++
CC_FLAGS = -std=c++11
LIBS = -lm

# File names
EXEC = cluster
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@ $(LIBS)

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
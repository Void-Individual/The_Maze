# OBJS specifies the files to be compiled
SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)

# CC specifies the compiler
CC = gcc

# FLAGS specifies the additional compilation flag options to be used
FLAGS = -g -Wall -Werror -w -Wextra -pedantic

# LINKER specifies the libraries that we're linking to
LINKER = -lSDL2 -lm -lSDL2_image

#OBJ is the executable object file
OBJ = maze

# Target to compile the executable
all: $(OBJ)

# Rule to link object files to make the executable
$(OBJ): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) $(LINKER) -o $(OBJ)

# Rule to compile each source file into an object file
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJS)

clean_all:
	rm -f $(OBJS) $(OBJ)
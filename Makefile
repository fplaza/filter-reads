EXEC = filter-reads

CC = g++
INCLUDE_DIRS = third_party/
CFLAGS = -I $(INCLUDE_DIRS) -Wall -Wextra -O3 -march=native -fopenmp
BOOST_LIBS = -lboost_program_options -lboost_iostreams
ZLIB = -lz
LDFLAGS = $(BOOST_LIBS) $(ZLIB) -fopenmp

SRC = $(wildcard *.cc)
OBJECTS = $(patsubst %.cc, %.o, $(SRC))

.PHONY : clean

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) *.o

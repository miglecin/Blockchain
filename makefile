CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

# Programos pavadinimas
TARGET = hash_program

# Šaltinio failai
SRCS = main.cpp hash.cpp
OBJS = $(SRCS:.cpp=.o)

# Numatytoji taisyklė
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Išvalyti objektinius failus ir programą
clean:
	rm -f $(OBJS) $(TARGET)

# Perkompiliuoti viską nuo nulio
rebuild: clean all

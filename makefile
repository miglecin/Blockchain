CXX = g++
CXXFLAGS = -Wall -Iinclude -Wno-deprecated-declarations -I/usr/local/opt/openssl@3/include
LDFLAGS = -L/usr/local/opt/openssl@3/lib -lcrypto
TARGET = hash_program

SRCS = src/main.cpp src/hash.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) 

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

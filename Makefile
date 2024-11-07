CXX = g++
CXXFLAGS = -I./lib
LDFLAGS = -shared
LIBS = -luser32 -lgdi32 -lkernel32

SRCS = entry.cpp lib/MainWindow.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = entry.dll

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

CXX = g++
CXXFLAGS = -I./lib
LDFLAGS = -shared
LIBS = -luser32 -lgdi32 -lkernel32
EXEFLAGS = -mwindows -municode
WINDRES = windres

SRCS = entry.cpp lib/MainWindow.cpp
OBJS = $(SRCS:.cpp=.o)
RC_OBJS = resources/DialogResource.res
TARGET_DLL = entry.dll
TARGET_EXE = entry.exe

all: $(TARGET_DLL) $(TARGET_EXE) $(RC_OBJS)

$(RC_OBJS): resources/DialogResource.rc
	windres -i $< -o $@ --input-format=rc --output-format=coff

$(TARGET_DLL): $(OBJS) $(RC_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(TARGET_EXE): $(OBJS) $(RC_OBJS)
	$(CXX) $(EXEFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /S *.o

.PHONY: all clean

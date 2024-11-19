CXX = g++
CXXFLAGS = -I./lib
LDFLAGS = -shared
LIBS = -luser32 -lgdi32 -lcomdlg32 -lkernel32
EXEFLAGS = -mwindows -municode -static-libstdc++ -static-libgcc
WINDRES = windres

SRCS = entry.cpp lib/MainWindow.cpp lib/Dialog.cpp lib/CryptoWindow.cpp 
OBJS = $(SRCS:.cpp=.o)
RC_OBJS = resources/DialogResource.res
TARGET_DLL = c_entry.dll
TARGET_EXE = entry.exe

LOGIC_DLL = logic/build/logic.dll

all: $(TARGET_DLL) $(TARGET_EXE) $(RC_OBJS) $(LOGIC_DLL)

$(LOGIC_DLL):
	$(MAKE) -C logic

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
	del /S *.dll
	del /S *.exe
	del /S *.res

.PHONY: all clean

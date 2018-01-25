CXX = clang++
CXXFLAGS = -flto -pedantic -std=c++14 -m32 -Wall -Wextra -Weverything \
		   -Wno-c++98-compat -Wno-old-style-cast -Wno-missing-prototypes \
		   -Wno-cast-align -Wno-c++98-compat-pedantic -Wno-c99-extensions \
		   -Wno-padded -Ofast -march=native -mtune=native
HEADERS = -I./mach_override
LIBS = -L./mach_override/build/ -lmach_override_32
OBJS = main.o hook.o libbase.o libhl.o libhw.o libclient.o hud.o cvar.o
TARGET = libmachook.dylib

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) -shared -o $@ $^

%.o: %.cpp
	$(CXX) $(HEADERS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

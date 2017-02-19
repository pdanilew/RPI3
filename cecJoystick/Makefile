CC=gcc
CXX=g++

TARGET := cecJoystick

LDFLAGS = -L /opt/vc/lib -lcec -ldl -lbcm_host -levdev
INCFLAGS += -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux/ -I/opt/vc/include/interface/vcos/pthreads -I/usr/local/include -I/usr/include/libevdev-1.0/
CPUFLAGS += -march=armv8-a+crc -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard

CPPFLAGS += $(CPUFLAGS) $(INCFLAGS) -fPIC
CXXFLAGS += -std=c++11

OBJECTS := cecServer.o

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


clean:
	find -name "*.o" -type f -delete
	rm -f $(TARGET)

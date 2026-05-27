CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I include
SRCS = src/main.cpp src/Paquete.cpp src/Compresor.cpp src/Integridad.cpp src/GestorPaquetes.cpp
TARGET = programa

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

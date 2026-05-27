CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I include
SRCS = src/main.cpp src/Pokemon.cpp src/Entrenador.cpp src/Mochila.cpp src/Batalla.cpp
TARGET = programa.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>nul || rm -f $(TARGET)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run

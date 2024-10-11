CXX = g++ # compilador a utilizar
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread # esto es para que compile con c++11 y muestre todos los warnings.
TARGET = carrera # nombre del ejecutable

$(TARGET): carrera.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) carrera.cpp

clean:
	rm -f $(TARGET)
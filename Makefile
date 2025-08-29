CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# final executable
sfml-app: main.o C_numbers.o
	$(CXX) $(CXXFLAGS) -o sfml-app main.o C_numbers.o $(LDFLAGS)

# object file
main.o: main.cpp C_numbers.h
	$(CXX) $(CXXFLAGS) -c main.cpp

C_numbers.o: C_numbers.cpp C_numbers.h
	$(CXX) $(CXXFLAGS) -c C_numbers.cpp

# clean up build artifacts
clean:
	rm -f *.o sfml-app

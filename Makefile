#First time making a makefile!
CXX = g++
CXXFLAGS = -std=c++17

SRC = main.cpp \
      classes/parser.cpp \
      classes/SymbolTable.cpp \
      classes/CodeGen.cpp \
      classes/Lexer.cpp
TARGET = parser

all: $(TARGET)

build: all

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET) test-input-files/testCodeHere.txt test-input-files/testCodeOut.txt

clean:
	rm -f $(TARGET)

small: all
	./$(TARGET) test-input-files/smlrat25s.txt test-input-files/smlrat25s.txt.out

med: all
	./$(TARGET) test-input-files/medrat25s.txt test-input-files/medrat25s.txt.out

large: all
	./$(TARGET) test-input-files/largerat25s.txt test-input-files/largerat25s.txt.out

#Never made one of these before!
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
	./parser test-input-files/medrat25s.txt test-input-files/medrat25s.txt.out

clean:
	rm -f $(TARGET)
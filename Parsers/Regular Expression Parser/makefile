CXX = g++
CXXFLAGS = -std=c++14 -Wall -g

TARGET = main

# Base directory for sources and headers
SRC_DIR = DsPatternRecognition/src

# Source files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/program_runner.cpp \
       $(SRC_DIR)/parsing/lexer.cpp \
       $(SRC_DIR)/parsing/parser.cpp \
       $(SRC_DIR)/services/dot_exporter_service.cpp \
       $(SRC_DIR)/services/matcher_service.cpp \
       $(SRC_DIR)/utils/string_trim.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = $(SRC_DIR)/program_runner.h \
          $(SRC_DIR)/parsing/lexer.h \
          $(SRC_DIR)/parsing/parser.h \
          $(SRC_DIR)/models/automaton.h \
          $(SRC_DIR)/services/dot_exporter_service.h \
          $(SRC_DIR)/services/matcher_service.h \
          $(SRC_DIR)/utils/string_trim.h \
          $(SRC_DIR)/utils/sort_by_member.h \
          $(SRC_DIR)/utils/find_by_member.h

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp files to .o object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# To avoid errors. Make sees everything as files so this was added to ensure it's not seen as files but as commands
.PHONY: all clean

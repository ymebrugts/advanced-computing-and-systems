CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

TARGET = main

SRC_DIR = DsExpressions/src

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/program_runner.cpp \
       $(SRC_DIR)/parser/parser.cpp \
       $(SRC_DIR)/parser/tokenizer.cpp \
       $(SRC_DIR)/nodes/constant_node.cpp \
       $(SRC_DIR)/nodes/operator_node.cpp \
       $(SRC_DIR)/nodes/variable_node.cpp \
       $(SRC_DIR)/visitors/simplify_visitor.cpp \
       $(SRC_DIR)/visitors/eval_visitor.cpp \
       $(SRC_DIR)/visitors/diff_visitor.cpp \
       $(SRC_DIR)/utils/string_utils.cpp \
       $(SRC_DIR)/utils/double_utils.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

HEADERS = $(SRC_DIR)/program_runner.h \
          $(SRC_DIR)/parser/parser.h \
          $(SRC_DIR)/parser/tokenizer.h \
          $(SRC_DIR)/nodes/expression_node.h \
          $(SRC_DIR)/nodes/constant_node.h \
          $(SRC_DIR)/nodes/operator_node.h \
          $(SRC_DIR)/nodes/variable_node.h \
          $(SRC_DIR)/visitors/simplify_visitor.h \
          $(SRC_DIR)/visitors/visitor.h \
          $(SRC_DIR)/visitors/eval_visitor.h \
          $(SRC_DIR)/visitors/diff_visitor.h \
          $(SRC_DIR)/utils/string_utils.h \
          $(SRC_DIR)/utils/double_utils.h

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

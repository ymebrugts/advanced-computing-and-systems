CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra \
    -IMovieParser/src \
    -IMovieSearch/src \
    -IMovieParser/src/models \
    -IMovieParser/src/parsers \
    -IShared/src \
    -IShared/src/utils


# Project directories
MOVIEPARSER_SRC := MovieParser/src
MOVIESEARCH_SRC := MovieSearch/src
SHARED_SRC := Shared/src

# Find all cpp files
MOVIEPARSER_SRCS := $(wildcard $(MOVIEPARSER_SRC)/**/*.cpp) $(wildcard $(MOVIEPARSER_SRC)/*.cpp)
MOVIESEARCH_SRCS := $(wildcard $(MOVIESEARCH_SRC)/**/*.cpp) $(wildcard $(MOVIESEARCH_SRC)/*.cpp)
SHARED_SRCS := $(wildcard $(SHARED_SRC)/**/*.cpp) $(wildcard $(SHARED_SRC)/*.cpp)

# Object files
MOVIEPARSER_OBJS := $(MOVIEPARSER_SRCS:.cpp=.o)
MOVIESEARCH_OBJS := $(MOVIESEARCH_SRCS:.cpp=.o)
SHARED_OBJS := $(SHARED_SRCS:.cpp=.o)

# Final executable
TARGET := moviesearch_app

all: $(TARGET)

$(TARGET): $(MOVIEPARSER_OBJS) $(SHARED_OBJS) $(MOVIESEARCH_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(MOVIEPARSER_OBJS) $(MOVIESEARCH_OBJS) $(SHARED_OBJS) $(TARGET)

.PHONY: all clean

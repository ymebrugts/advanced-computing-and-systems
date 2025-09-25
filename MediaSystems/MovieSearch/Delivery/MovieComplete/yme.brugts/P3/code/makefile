CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra \
    -IMovieParser/src \
    -IMovieSearch/src \
    -IRatingDistance/src \
    -IRatingDistance/services \
    -IMovieRate/src \
    -IMovieParser/src/models \
    -IMovieParser/src/parsers \
    -IMovieParser/src/services \
    -IMovieParser/src/utils \
    -IMovieParser/src/algorithms \
    -IShared/src \
    -IShared/src/utils

# Project directories
MOVIEPARSER_DIR     := MovieParser/src
MOVIESEARCH_DIR     := MovieSearch/src
RATINGDISTANCE_DIR  := RatingDistance/src
MOVIERATE_DIR       := MovieRate/src
SHARED_DIR          := Shared/src

# Source files
MOVIEPARSER_SRCS     := $(wildcard $(MOVIEPARSER_DIR)/**/*.cpp) $(wildcard $(MOVIEPARSER_DIR)/*.cpp)
MOVIESEARCH_SRCS     := $(wildcard $(MOVIESEARCH_DIR)/**/*.cpp) $(wildcard $(MOVIESEARCH_DIR)/*.cpp)
RATINGDISTANCE_SRCS  := $(wildcard $(RATINGDISTANCE_DIR)/**/*.cpp) $(wildcard $(RATINGDISTANCE_DIR)/*.cpp)
MOVIERATE_SRCS       := $(wildcard $(MOVIERATE_DIR)/**/*.cpp) $(wildcard $(MOVIERATE_DIR)/*.cpp)
SHARED_SRCS          := $(wildcard $(SHARED_DIR)/**/*.cpp) $(wildcard $(SHARED_DIR)/*.cpp)

# Object files
MOVIEPARSER_OBJS     := $(MOVIEPARSER_SRCS:.cpp=.o)
MOVIESEARCH_OBJS     := $(MOVIESEARCH_SRCS:.cpp=.o)
RATINGDISTANCE_OBJS  := $(RATINGDISTANCE_SRCS:.cpp=.o)
MOVIERATE_OBJS       := $(MOVIERATE_SRCS:.cpp=.o)
SHARED_OBJS          := $(SHARED_SRCS:.cpp=.o)

# Executables
APPS := moviesearch_app ratingdistance_app movierate_app

all: $(APPS)

moviesearch_app: $(MOVIEPARSER_OBJS) $(SHARED_OBJS) $(MOVIESEARCH_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

ratingdistance_app: $(MOVIEPARSER_OBJS) $(SHARED_OBJS) $(RATINGDISTANCE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

movierate_app: $(MOVIEPARSER_OBJS) $(SHARED_OBJS) $(MOVIERATE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(MOVIEPARSER_OBJS) $(MOVIESEARCH_OBJS) $(RATINGDISTANCE_OBJS) $(MOVIERATE_OBJS) $(SHARED_OBJS) $(APPS)

.PHONY: all clean


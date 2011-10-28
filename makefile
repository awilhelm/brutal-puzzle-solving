all: $(basename $(wildcard *.cc))
CPPFLAGS += -g0 -O3 -march=native -Wall -Wextra -Werror -Wconversion

all: ~/.local/bin/organize_directory_contents

~/.local/bin/organize_directory_contents: main.cc
	clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics -o $@ $<

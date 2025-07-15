bin := ~/.local/bin/organize_directory_contents

' ':

cc: main.cc
	clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics -o ${bin} $<

py: main.py
	install -v -l as $< ${bin}

cfg: paths.cfg
	install -v -l as $< ~/.config/organize_directory_contents.cfg
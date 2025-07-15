bin := ~/.local/bin/organize_directory_contents

CXX := clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics

' ':

cc: main.cc
	${CXX} -o ${bin} $<

test_cc: main.cc
	${CXX} -g -o $@ $<
	./$@ ./test ./paths.cfg
	rm ./$@

py: main.py
	install -v -l as $< ${bin}

cfg: paths.cfg
	install -v -l as $< ~/.config/organize_directory_contents.cfg
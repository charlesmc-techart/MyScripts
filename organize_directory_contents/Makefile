bin_path := ~/.local/bin/organize_directory_contents

CXX      := clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics

all: build-c++ install-config

build-c++: $(bin_path)
$(bin_path): c++/main.o c++/move_file.o c++/read_targets_file.o
	if [[ -L $(bin_path) ]]; then rm $(bin_path); fi
	$(CXX) -o $(bin_path) $^


c++/%.o: c++/%.cc
	$(CXX) -o $@ -c $<

test_dir  := test_d
test_file := test
test-c++: c++/main.o c++/move_file.o c++/read_targets_file.o
	$(CXX) -o c++/$(test_file) $^
	mkdir -p c++/$(test_dir)

	for file in blend blend1 ma mb abc fbx obj zip 7z tar py cpp h hpp s js gs jpg png orf dng xls txt mov mp4 mp3 xmp; do\
		touch c++/$(test_dir)/something.$$file;\
	done

	c++/$(test_file) c++/$(test_dir) /targets.cfg

clean-obj:
	rm -rf c++/*.o

clean-test:
	rm -rf c++/$(test_file) c++/$(test_dir)


link-py: main.py
	install -v -l as $< $(bin_path)

install-config: ~/.config/organize_directory_contents.cfg
~/.config/organize_directory_contents.cfg: targets.cfg
	if [[ ! -L  $@ ]]; then install -v -l as $<; fi

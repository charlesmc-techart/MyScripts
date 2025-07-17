bin_path := ~/.local/bin/organize_directory_contents

CXX      := clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics

all: build cfg

build: c++/main.o c++/move_file.o c++/read_targets_file.o
	if [[ -L $(bin_path) ]]; then\
  		rm $(bin_path);\
  	fi

	$(CXX) -o $(bin_path) $^

c++/%.o: c++/%.cc
	$(CXX) -o $@ -c $<

test_dir  := test_d
test_file := test_f
test: c++/main.o c++/move_file.o c++/read_targets_file.o
	$(CXX) -o $(test_file) $^
	mkdir -p ./$(test_dir)

	for file in blend blend1 ma mb abc fbx obj zip 7z tar py cpp h hpp s js gs jpg png orf dng xls txt mov mp4 mp3 xmp; do\
		touch ./$(test_dir)/something.$$file;\
	done

	./$(test_file) ./$(test_dir) ./targets.cfg

clean-obj:
	rm -rf ./**/*.o

clean-test:
	rm -rf ./$(test_file) ./$(test_dir)


py: main.py
	install -v -l as $< $(bin_path)

cfg: targets.cfg
	install -v -l as $< ~/.config/organize_directory_contents.cfg

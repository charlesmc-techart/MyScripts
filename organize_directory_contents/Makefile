bin_path := ~/.local/bin/organize_directory_contents

CXX      := clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics


cc: main.cc
	$(CXX) -o $(bin_path) ./$<

py: main.py
	install -v -l as $< $(bin_path)

cfg: paths.cfg
	install -v -l as $< ~/.config/organize_directory_contents.cfg


test_dir  := test_d
test_file := test_f

clean:
	rm -rf ./$(test_file) ./$(test_dir)

test: main.cc
	$(CXX) -o $(test_file) ./$<
	mkdir -p ./$(test_dir)

	for file in blend blend1 ma mb abc fbx obj zip 7z tar py cpp h hpp s js gs jpg png orf dng xls txt mov mp4 mp3; do\
		touch ./$(test_dir)/something.$$file;\
	done

	./$(test_file) ./$(test_dir) ./paths.cfg

lbin := ~/.local/bin
lconfig := ~/.config

all:
	make ${lbin}/organize_directory_contents
	make ${lbin}/process_screenshots

${lbin}/organize_directory_contents: ./organize_directory_contents/main.cc
	clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics -o $@ $<

${lbin}/process_screenshots: ./process_screenshots/main.sh
	install -v -l as $< $@
	install -v -l as ./process_screenshots/process_screenshots.workflow ~/Library/Workflows/Applications/Folder\ Actions

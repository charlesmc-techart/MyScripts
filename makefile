lbin := ~/.local/bin
lconfig := ~/.config

all:
	make ${lbin}/organize_directory_contents
	make ${lconfig}/process_screenshots/main

${lbin}/organize_directory_contents: ./organize_directory_contents/main.cc
	clang++ -std=c++26 -Wall -Wextra -Wpedantic -O3 -fcolor-diagnostics -o $@ $<

${lconfig}/process_screenshots/main: ./process_screenshots/main.sh ./process_screenshots/process_screenshots.workflow/Contents/document.wflow ./process_screenshots/process_screenshots.workflow/Contents/Info.plist
	install -v -l as ./process_screenshots/process_screenshots.workflow ~/Library/Workflows/Applications/Folder\ Actions
	install -v -l as $< $@
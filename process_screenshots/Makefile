all: main workflow

main: main.sh
	install -v -l as $< ~/.local/bin/process_screenshots

workflow: process_screenshots.workflow
	install -v -l as $< ~/Library/Workflows/Applications/Folder\ Actions/$<
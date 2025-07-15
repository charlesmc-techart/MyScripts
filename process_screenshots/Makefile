all: workflow main

workflow_dir = ~/Library/Workflows/Applications/Folder\ Actions
workflow: process_screenshots.workflow ${workflow_dir}/process_screenshots.workflow
	install -v -l as $< ${workflow_dir}

main: main.sh ~/.local/bin/process_screenshots
	install -v -l as $^
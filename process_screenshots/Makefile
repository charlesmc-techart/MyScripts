workflow_dir = ~/Library/Workflows/Applications/Folder\ Actions
all: ${workflow_dir}/process_screenshots.workflow ~/.local/bin/process_screenshots

${workflow_dir}/process_screenshots.workflow: process_screenshots.workflow
	install -v -l as $< ${workflow_dir}

~/.local/bin/process_screenshots: ./main.sh
	install -v -l as $< $@
SCRIPT_NAME := process_screenshots
BIN_PATH    := ~/.local/bin/$(SCRIPT_NAME)

all:  main workflow

main: main.zsh dir
	install -v -l as $< $(BIN_PATH)/$@
	chmod +x $(BIN_PATH)/$@

workflow: workflow.zsh $(SCRIPT_NAME).workflow dir
	install -v -l as $(SCRIPT_NAME).workflow ~/Library/Workflows/Applications/Folder\ Actions/$<

	install -v -l as $< $(BIN_PATH)/$<
	chmod +x $(BIN_PATH)/$<

dir:
	if [[ -d $(BIN_PATH) ]]; then exit 0;\
	elif [[ -e $(BIN_PATH) ]]; then rm $(BIN_PATH);\
	fi;\
	mkdir $(BIN_PATH)

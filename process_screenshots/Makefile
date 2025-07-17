SCRIPT_NAME := process_screenshots
BIN_PATH    := ~/.local/bin/$(SCRIPT_NAME)

all:  main workflow

main: main.zsh dir
	install -v -l as $< $(BIN_PATH)/$@
	chmod +x $(BIN_PATH)/$@

workflow: workflow.zsh $(SCRIPT_NAME).workflow dir
	install -v -l as $(SCRIPT_NAME).workflow ~/Library/Workflows/Applications/Folder\ Actions/$<

	install -v -l as $< $@
	chmod +x $(BIN_PATH)/$<

dir: $(BIN_PATH)
$(BIN_PATH):
	if [[ -e $(BIN_PATH) && ! -d $(BIN_PATH) ]]; then\
		rm $(BIN_PATH);\
	fi
	mkdir $(BIN_PATH)
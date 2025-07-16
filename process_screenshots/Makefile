SCRIPT_NAME := process_screenshots
BIN_PATH    := ~/.local/bin/$(SCRIPT_NAME)

all: main workflow

main: main.zsh
	[[ -d $(BIN_PATH) ]] || mkdir $(BIN_PATH)

	install -v -l as $< $(BIN_PATH)/$@
	chmod +x $(BIN_PATH)/$@

workflow: workflow.zsh $(SCRIPT_NAME).workflow
	install -v -l as $< ~/Library/Workflows/Applications/Folder\ Actions/$<

	[[ -d $(BIN_PATH) ]] || mkdir $(BIN_PATH)
	install -v -l as $< $(BIN_PATH)/$<
	chmod +x $(BIN_PATH)/$<
#!/usr/bin/env zsh -f
# A Folder Action script for preparing `main.sh`

readonly SCREENSHOTS_DIR=~/MyFiles/Pictures/Screenshots
readonly TAG_FILES_DIR=~/.config/exiftool

readonly EXECUTABLE_DIR=~/.local/bin
readonly EXECUTABLE_NAME=process_screenshots
readonly PIPE=~SCREENSHOTS_DIR/$EXECUTABLE_NAME

################################################################################

if [[ -p ~PIPE ]]; then
    echo "Pipe '${PIPE}' exists; Folder action already in progress" >&2
    exit 1
fi
# Taking multiple screenshots in succession causes the Folder Action to trigger
# the same amount of times. Checking for this temporary pipe in the `elif`
# statement above ensures that only the first instance of the Folder Action
# executes the rest of the script body
mkfifo ~PIPE && trap 'rm ~PIPE' EXIT

~EXECUTABLE_DIR/$EXECUTABLE_NAME\
    -i ~SCREENSHOTS_DIR/.tmp             -o ~SCREENSHOTS_DIR\
    ~TAG_FILES_DIR/charlesmc.args        ~TAG_FILES_DIR/screenshot.args

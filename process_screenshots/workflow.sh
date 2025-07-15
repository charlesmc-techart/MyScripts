#!/usr/bin/env zsh -f
# A Folder Action script for preparing `main.sh`

readonly SCREENSHOTS_DIR=~/MyFiles/Pictures/Screenshots/.tmp
readonly PIPE=${~SCREENSHOTS_DIR:h}/ps
readonly TAG_FILES_DIR=~/.config/exiftool

################################################################################

if [[ -p $PIPE ]]; then
    echo "Pipe '$PIPE' exists, so the folder action is already in progress" >&2
    exit 1
fi
# Taking multiple screenshots in succession causes the Folder Action to trigger
# the same amount of times. Checking for this temporary pipe in the `elif`
# statement above ensures that only the first instance of the Folder Action
# executes the rest of the script body
mkfifo $PIPE && trap 'rm $PIPE' EXIT

~/.local/bin/process_screenshots ~SCREENSHOTS_DIR ~TAG_FILES_DIR/charlesmc.args ~TAG_FILES_DIR/screenshot.args

if (( $? == 0 )); then
    mv ~SCREENSHOTS_DIR/*.*(.) ${~SCREENSHOTS_DIR:h}
fi
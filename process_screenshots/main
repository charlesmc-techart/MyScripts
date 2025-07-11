#!/usr/bin/env zsh -f
# A Folder Action script for renaming screenshots and adding certain metadata

readonly SCREENSHOTS_DIR=~/MyFiles/Pictures/Screenshots/.tmp
cd ~SCREENSHOTS_DIR || exit $?

setopt EXTENDED_GLOB
readonly FILENAME_PATTERN='*2<-1><-9><-9>-<-1><-9>-<-3><-9>*<-2><-9>.<-5><-9>.<-5><-9>*.*(.)'
readonly DIRNAME=${0:A:h:t}
readonly PIPE=../$DIRNAME
readonly TAG_FILES_DIR=~/.config/$DIRNAME

################################################################################

if [[ ! ./~FILENAME_PATTERN ]] 2>/dev/null; then
    echo "No screenshots to process" >&2
    exit 2
elif [[ -p $PIPE ]]; then
    echo "Pipe '$PIPE' exists, so the folder action is already in progress" >&2
    exit 1
fi
# Taking multiple screenshots in succession causes the Folder Action to trigger
# the same amount of times. Checking for this temporary pipe in the `elif`
# statement above ensures that only the first instance of the Folder Action
# executes the rest of the script body
mkfifo $PIPE && trap 'rm $PIPE' EXIT

readonly search_str='Model Name:'
readonly hardware=$(system_profiler SPHardwareDataType | grep $search_str | sed -E "s/.*${search_str} ?//")

readonly timezone=$(date +%z)

# PERL string replacement patterns that will be used by ExifTool
readonly re='^.+?(2[0-1])(\d{2})-([0-1]\d)-([0-3]\d).+([0-2]\d)\.([0-5]\d)\.([0-5]\d)(\W*?\d+?)?\..+?$'
readonly orig_str_pattern="Filename;s/${re}"
readonly filename_pattern="\${${orig_str_pattern}/\$2\$3\$4_\$5\$6\$7\$8.%e/}"
readonly datetime_pattern="\${${orig_str_pattern}/\$1\$2-\$3-\$4T\$5:\$6:\$7${timezone}/}"

/opt/homebrew/bin/exiftool -P -v -struct      $~FILENAME_PATTERN\
     -directory=../                           "-Filename<$filename_pattern"\
    "-AllDates<$datetime_pattern"             "-OffsetTime*=$timezone"\
    '-MaxAvailHeight<ImageHeight'             '-MaxAvailWidth<ImageWidth'\
    '-RawFileName<FileName'                   '-PreservedFileName<FileName'\
    "-Software=$(sw_vers --productVersion)"   "-Model=$hardware"\
     -@ ~TAG_FILES_DIR/charlesmc.args          -@ ~TAG_FILES_DIR/screenshot.args

if (( $? == 0 )); then
    # Back up the originals in an Apple Archive before deleting them
    aa archive -o "${SCREENSHOTS_DIR:h}/originals_$(date +%y%m%d_%H%M%S).aar"\
        -d . -a lzma -exclude-name .DS_Store\
        && rm ./*(.)
fi

#!/usr/bin/env zsh -f
# A script for renaming screenshots and adding certain metadata

if (( $# < 1 )); then
    echo "No directory to process" >&2
    exit 1
fi

cd $1 2>/dev/null
if (( $? > 0 )); then
    echo "Not a directory: $1" >&2
    exit 2
fi

readonly orig_filename_pattern='*2<-1><-9><-9>-<-1><-9>-<-3><-9>*<-2><-9>.<-5><-9>.<-5><-9>*.*(.)'
if [[ ! ./~orig_filename_pattern ]] 2>/dev/null; then
    echo "No screenshots to process" >&2
    exit 3
fi

# Parse tag files
shift
declare -a tag_files
for arg in $@; do
    [[ ${arg:e} != 'args' ]] && continue

    tag_files+="-@ $arg"
    shift
done

# PERL string replacement patterns that will be used by ExifTool
readonly re='^.+?(2[0-1])(\d{2})-([0-1]\d)-([0-3]\d).+([0-2]\d)\.([0-5]\d)\.([0-5]\d)(\s\(\d+?\))?\..+?$'
readonly orig_str_pattern="Filename;s/${re}"
readonly new_filename_pattern="\${${orig_str_pattern}/\$2\$3\$4_\$5\$6\$7\$8.%e/}"
readonly new_datetime_pattern="\${${orig_str_pattern}/\$1\$2-\$3-\$4T\$5:\$6:\$7${timezone}/}"

readonly timezone=$(date +%z)

readonly search_str='Model Name:'
readonly hardware=$(system_profiler SPHardwareDataType | grep $search_str | sed -E "s/ *${search_str} ?//")

/opt/homebrew/bin/exiftool -P -struct         $=tag_files\
    $~orig_filename_pattern                   "-Filename<$new_filename_pattern"\
    "-AllDates<$new_datetime_pattern"         "-OffsetTime*=$timezone"\
    '-MaxAvailHeight<ImageHeight'             '-MaxAvailWidth<ImageWidth'\
    '-RawFileName<FileName'                   '-PreservedFileName<FileName'\
    "-Software=$(sw_vers --productVersion)"   "-Model=$hardware"

if (( $? == 0)); then
    tmp_dir=$(mktemp -d -t $0:h:t)
    mv $~orig_filename_pattern ~tmp_dir

    aa archive -o "Screenshots_$(date +%y%m%d_%H%M%S).aar" -d ~tmp_dir -a lzma\
        -exclude-name .DS_Store\
        && rm -rf ~tmp_dir
fi
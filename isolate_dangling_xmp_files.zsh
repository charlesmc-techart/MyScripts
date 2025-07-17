#!/usr/bin/env zsh -f
# A script for deleting sidecar files not without an image

if (( $# < 1 )); then
    echo 'Must provide a directory to search' >&2
    exit 1
elif [[ ! -d $1 ]]; then
    echo "Not a directory: $1" >&2
    exit 2
fi

cd $1

readonly target_dir=__dangling_xmps__
if [[ ! -d $target_dir ]];
    then mkdir $target_dir
fi

for file in $1/**/*.xmp; do
    pattern="${file/xmp/*}"
    if (( $(ls -1 $~pattern | wc -l) < 2 )); then
        new_path="${file/"$file:t"/"${target_dir}/${file:t}"}"

        mv "$file" "$new_path"
    fi
done
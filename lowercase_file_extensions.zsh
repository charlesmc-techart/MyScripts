#!/usr/bin/env zsh -f
# A script for converting file extensions to lowercase

function show_help {
    echo 'Usage: lowercase_file_extensions [-v | --verbose, -h | --help ] [directory]' >&2
}

################################################################################

if (( $# < 1 )); then
    show_help
    exit 1
fi

cd "$dir"
echo "Searching: '${dir}'"

for file in ./**/*.*(.); do
    extension=$file:e
    if [[ $extension == $extension:l ]]; then
        continue
    fi

    basename="$file:t"
    new_name="${basename:r}.${extension:l}"

    mv "$file" "${file/${basename}/${new_name}}"
    echo "${basename} -> ${new_name}"
done
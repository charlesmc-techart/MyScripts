#!/usr/bin/env zsh -f
# A script for converting file extensions to lowercase

# todo: Add verbosity settings?

dir=.
if [[ $1 && ! -d $1 ]]; then
    echo "Not a directory: $1" >&2
    exit 1
else
    dir=$1
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
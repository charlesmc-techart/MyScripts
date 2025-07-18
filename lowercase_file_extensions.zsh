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

is_verbose=
dir=
while (( $# > 0 )); do
    case $1 in
    -h | --help   )
        show_help
        exit
    ;;

    -v | --verbose)
        is_verbose=-v
    ;;

    -* | --*)
        echo "Invalid option: $1"
        exit 2
    ;;

    *)
        if [[ -d "$1" ]]; then
            dir="$1"
            break
        else
            echo "Invalid argument: $1"
            exit 2
        fi
    ;;
    esac
    shift
done

if [[ ! $dir ]]; then
    echo No directory >&2
    exit 1
fi

cd $dir || exit $?
autoload zmv || exit $?
zmv $is_verbose '(**/)(*.)(*)' '$1$2${(L)3}'

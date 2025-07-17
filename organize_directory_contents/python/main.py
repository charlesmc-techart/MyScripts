#!/usr/bin/env python3 -OO
"""A CLI script to organize the contents of a directory"""


from argparse import ArgumentParser
from pathlib import Path

MISC_DIR = "Misc"


def read_targets_from_file(file: Path) -> tuple[set[str], dict[str, str]]:
    """Create a targets dictionary from a file."""

    dirs: set[str] = set()
    targets: dict[str, str] = {}
    with file.open() as f:
        for line in f:
            if not line or line.startswith("#"):
                continue
            try:
                key, value = line.split("=")
            except ValueError:
                continue

            value = value.strip()
            dirs.add(value)
            targets[key.strip()] = value

    dirs.add(MISC_DIR)
    return dirs, targets


def move_file(file: Path, target_dir: Path) -> None:
    """Move `file` into `target_dir`."""

    file.rename(target_dir / file.name)


def move_image(image_file: Path, target_dir: Path) -> None:
    """Move an image and its sidecar file to `target_dir`."""

    move_file(image_file, target_dir)

    sidecar_file = image_file.with_suffix(".xmp")
    try:
        move_file(sidecar_file, target_dir)
    except FileNotFoundError:
        pass  # Do nothing if a sidecar file does not exist.


def main(root_dir: Path, config_file: Path) -> None:

    subdirs, target_dirs = read_targets_from_file(config_file)

    for subdir in subdirs:
        (root_dir / subdir).mkdir(parents=True, exist_ok=True)

    # `move_image()` will move an image's existing sidecar file alongside the
    # image, so defer processing XMP files to the end.
    xmp_files: list[Path] = []

    images_dir = target_dirs["jpg"]
    images_raw_dir = target_dirs["dng"]
    for file in root_dir.iterdir():
        if file.name in subdirs or file.name == ".DS_Store":
            continue

        # Utilize short-circuiting by checking if the file extension is "xmp"
        # only at the end.
        if (file_ext := file.suffix) and (
            file_ext := file_ext[1:].lower()
        ) == "xmp":
            xmp_files.append(file)
            continue

        target_dir = target_dirs.get(file_ext, MISC_DIR)
        if target_dir is images_dir or target_dir is images_raw_dir:
            move_image(file, root_dir / target_dir)

        else:
            move_file(file, root_dir / target_dir)

    for xmp_file in xmp_files:
        try:
            move_file(xmp_file, root_dir / MISC_DIR)
        except FileNotFoundError:
            pass  # Do nothing if the image sidecar file had already been moved.


if __name__ == "__main__":
    parser = ArgumentParser(prog="Organize Directory", description=__doc__)
    parser.add_argument("dir", type=Path, help="the directory to organize")
    parser.add_argument(
        "config",
        type=Path,
        help="a map between a file extension and its destination",
    )
    args = parser.parse_args()

    main(args.dir, args.config)

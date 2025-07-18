#!/usr/bin/env python3 -OO
"""A CLI script to organize the contents of a directory"""


__author__ = "Charles Mesa Cayobit"


from argparse import ArgumentParser
from pathlib import Path

import move_file
import targets


def main(root_dir: Path, config_file: Path) -> None:
    subdirs, target_dirs = targets.read_from_file(config_file)

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

        target_dir = target_dirs.get(file_ext, targets.MISC_DIR)
        if target_dir == images_dir or target_dir == images_raw_dir:
            move_file.move_image(file, root_dir / target_dir)

        else:
            move_file.move_file(file, root_dir / target_dir)

    for xmp_file in xmp_files:
        try:
            move_file.move_file(xmp_file, root_dir / targets.MISC_DIR)
        except FileNotFoundError:
            pass  # Do nothing if the image sidecar file had already been moved.


if __name__ == "__main__":
    parser = ArgumentParser(prog="Organize Directory", description=__doc__)
    parser.add_argument("dir", type=Path, help="the directory to organize")
    parser.add_argument(
        "-t",
        "--targets",
        type=Path,
        help="a map between a file extension and its destination",
    )
    args = parser.parse_args()
    targets_file = args.targets or args.dir / "targets.cfg"

    main(args.dir, targets_file)

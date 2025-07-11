"""Functions for organizing the contents of a directory"""

from pathlib import Path


def move_file(file: Path, target_dir: Path) -> None:
    """Move `file` into `target_dir`, creating `target_dir` as necessary."""

    if not target_dir.exists():
        target_dir.mkdir()
    file.rename(target_dir / file.name)


def move_image(image_file: Path, target_dir: Path) -> None:
    """Move an image and its sidecar file to `target_dir`."""

    move_file(image_file, target_dir)

    sidecar_file = image_file.with_suffix(".xmp")
    try:
        sidecar_file.rename(target_dir / sidecar_file.name)
    except FileNotFoundError:
        pass  # Do nothing if a sidecar file does not exist

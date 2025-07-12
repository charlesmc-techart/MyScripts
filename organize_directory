#!/usr/bin/env python3 -OO
"""A CLI script to organize the contents of a directory

Directory/
|--3D/
|  |--Blender/
|  |--Maya/
|
|--Audio/
|
|--Archives/
|
|--Code/
|    |--Assembly/
|    |--C_Cpp/
|    |--Javascript/
|    |--Python/
|    |--Shell/
|
|--Documents/
|
|--Images/
|    |--Raw/
|
|--Misc/
|
|--Videos/
"""

from argparse import ArgumentParser
from pathlib import Path

SUBDIRECTORIES = {
    THREE_D_DIR := "3D",
    AUDIO_DIR := "Audio",
    ARCHIVES_DIR := "Archives",
    CODE_DIR := "Code",
    DOCUMENTS_DIR := "Documents",
    IMAGES_DIR := "Images",
    MISC_DIR := "Misc",
    VIDEOS_DIR := "Videos",
}

# Inner subdirectory paths
THREE_D_BLENDER_DIR = Path(THREE_D_DIR, "Blender")
THREE_D_MAYA_DIR = Path(THREE_D_DIR, "Maya")
CODE_ASSEMBLY_DIR = Path(CODE_DIR, "Assembly")
CODE_C_CPP_DIR = Path(CODE_DIR, "C_Cpp")
CODE_JAVASCRIPT_DIR = Path(CODE_DIR, "Javascript")
CODE_PYTHON_DIR = Path(CODE_DIR, "Python")
CODE_SHELL_DIR = Path(CODE_DIR, "Shell")
IMAGES_RAW_DIR = Path(IMAGES_DIR, "Raws")

# The keys are lowercase file extensions
TARGET_SUBDIRECTORY_PATHS = {
    # 3D
    "abc": THREE_D_DIR,
    "fbx": THREE_D_DIR,
    "obj": THREE_D_DIR,
    # 3D/Blender
    "blend": THREE_D_BLENDER_DIR,
    "blend1": THREE_D_BLENDER_DIR,
    # 3D/Maya
    "ma": THREE_D_MAYA_DIR,
    "mb": THREE_D_MAYA_DIR,
    # Audio
    "mp3": AUDIO_DIR,
    # Archives
    "7z": ARCHIVES_DIR,
    "aar": ARCHIVES_DIR,
    "zip": ARCHIVES_DIR,
    "gz": ARCHIVES_DIR,
    "xz": ARCHIVES_DIR,
    "tar": ARCHIVES_DIR,
    "txz": ARCHIVES_DIR,
    # Code
    "env": CODE_DIR,
    "s": CODE_ASSEMBLY_DIR,
    "c": CODE_C_CPP_DIR,
    "cc": CODE_C_CPP_DIR,
    "cpp": CODE_C_CPP_DIR,
    "h": CODE_C_CPP_DIR,
    "hh": CODE_C_CPP_DIR,
    "hpp": CODE_C_CPP_DIR,
    "js": CODE_JAVASCRIPT_DIR,
    "py": CODE_PYTHON_DIR,
    "pyi": CODE_PYTHON_DIR,
    "sh": CODE_SHELL_DIR,
    "bash": CODE_SHELL_DIR,
    "zsh": CODE_SHELL_DIR,
    # Serialization
    "db": CODE_DIR,
    "json": CODE_DIR,
    "xml": CODE_DIR,
    "yml": CODE_DIR,
    # Documents
    "txt": DOCUMENTS_DIR,
    "md": DOCUMENTS_DIR,
    "rst": DOCUMENTS_DIR,
    "csv": DOCUMENTS_DIR,
    "tsv": DOCUMENTS_DIR,
    "pdf": DOCUMENTS_DIR,
    "doc": DOCUMENTS_DIR,
    "docx": DOCUMENTS_DIR,
    "xls": DOCUMENTS_DIR,
    # Images
    "heic": IMAGES_DIR,
    "jpg": IMAGES_DIR,
    "jpeg": IMAGES_DIR,
    "png": IMAGES_DIR,
    "webp": IMAGES_DIR,
    "tif": IMAGES_DIR,
    "tiff": IMAGES_DIR,
    "psd": IMAGES_DIR,
    # Images/Raw
    "dng": IMAGES_RAW_DIR,
    "orf": IMAGES_RAW_DIR,
    # Videos
    "mkv": VIDEOS_DIR,
    "mov": VIDEOS_DIR,
    "mp4": VIDEOS_DIR,
}


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


def main(dir: Path) -> None:
    """Organize the contents of `dir`."""

    xmp_files = []
    for source_dir in dir.iterdir():
        filename = source_dir.name
        if filename in SUBDIRECTORIES or filename == ".DS_Store":
            continue

        file_ext = source_dir.suffix.lower()[1:]
        if file_ext == "xmp":
            xmp_files.append(source_dir)
            continue

        target_dir = TARGET_SUBDIRECTORY_PATHS.get(file_ext, MISC_DIR)
        if target_dir is IMAGES_DIR or target_dir is IMAGES_RAW_DIR:
            move_func = move_image

        else:
            move_func = move_file

        move_func(source_dir, dir / target_dir)

    for xmp_file in xmp_files:
        try:
            move_file(xmp_file, dir / MISC_DIR)
        except FileNotFoundError:
            pass  # Do nothing if the image sidecar file had already been moved


if __name__ == "__main__":
    parser = ArgumentParser(prog="Organize Directory", description=__doc__)
    parser.add_argument("dir", type=Path, help="the directory to organize")
    args = parser.parse_args()

    main(args.dir)

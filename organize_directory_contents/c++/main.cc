// A CLI program to organize the contents of a directory.

#include "read_targets_file.hh"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

/// \brief Move `file` into `target_dir`.
void move_file(const std::filesystem::path&, const std::filesystem::path&);

/// \brief Move an image and its sidecar file to `target_dir`.
void move_image(const std::filesystem::path&, const std::filesystem::path&);

// main

/// \todo Maybe a better way of handling CLI args?
int main(const int argc, const char* argv[])
{
    if (argc < 3) {
        std::cerr << "Error: Must supply a directory and a config file\n";
        return 1;
    }

    const std::filesystem::path root_dir{argv[1]};
    const std::filesystem::path config_file{argv[2]};

    auto [subdirs, target_dirs]{cmc::read_targets_file(config_file)};

    for (const auto& subdir: subdirs) {
        std::filesystem::create_directories(root_dir / subdir);
    }

    // `move_image()` will move an image's existing sidecar file alongside the
    // image, so defer processing XMP files to the end.
    std::vector<std::filesystem::path> xmp_files;

    const auto& images_dir{target_dirs["jpg"s]};
    const auto& images_raw_dir{target_dirs["dng"s]};
    for (const auto& dir: std::filesystem::directory_iterator{root_dir}) {
        const auto& dir_path{dir.path()};

        if (subdirs.contains(dir_path.filename())
            || dir_path.filename().string() == ".DS_Store"s) {
            continue;
        }

        auto file_ext{dir_path.extension().string()};
        if (file_ext.empty()) { continue; }

        file_ext = file_ext.substr(1);
        for (auto& c: file_ext) { c = static_cast<char>(std::tolower(c)); }

        if (file_ext == "xmp") {
            xmp_files.push_back(dir_path);
            continue;
        }

        std::filesystem::path target_dir;
        try {
            target_dir = target_dirs.at(file_ext);
        }
        catch (const std::out_of_range&) {
            target_dir = cmc::MISC_DIR;
        }

        if (target_dir == images_dir || target_dir == images_raw_dir) {
            move_image(dir_path, root_dir / target_dir);
        }

        else {
            move_file(dir_path, root_dir / target_dir);
        }
    }

    for (const auto& xmp_file: xmp_files) {
        try {
            move_file(xmp_file, root_dir / cmc::MISC_DIR);
        }
        catch (const std::filesystem::filesystem_error&) {
            // Do nothing if the image sidecar file had already been moved.
        }
    }
}

// Implementation details below

void move_file(const std::filesystem::path& file,
               const std::filesystem::path& target_dir)
{
    std::filesystem::rename(file, target_dir / file.filename());
}

void move_image(const std::filesystem::path& image_file,
                const std::filesystem::path& target_dir)
{
    move_file(image_file, target_dir);

    const auto& sidecar_file{const_cast<std::filesystem::path&>(image_file)
                                     .replace_extension("xmp")};
    try {
        move_file(sidecar_file, target_dir);
    }
    catch (const std::filesystem::filesystem_error&) {
        // Do nothing if a sidecar file does not exist.
    }
}

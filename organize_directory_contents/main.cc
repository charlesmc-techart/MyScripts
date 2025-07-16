// A CLI program to organize the contents of a directory.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static const std::filesystem::path MISC_DIR{"Misc"};

/// Create a targets map from a file.
/// \param config_file the file containing the mapping
/// \param out_dirs will contain all the unique directories
/// \param out_targets will contain the map between a file extension and the
/// corresponding file's destination
void read_targets_from_file(
    const std::filesystem::path& config_file,
    std::unordered_set<std::filesystem::path>& out_dirs,
    std::unordered_map<std::string, std::filesystem::path>& out_targets);

// Move `file` into `target_dir`.
void move_file(const std::filesystem::path&, const std::filesystem::path&);

// Move an image and its sidecar file to `target_dir`.
void move_image(const std::filesystem::path&, const std::filesystem::path&);

// main

/// \todo Maybe a better way of handling CLI args?
int main(const int argc, const char* argv[]) {
  if (argc < 3) {
    std::cerr << "Error: Must supply a directory and a config file\n";
    return 1;
  }

  const std::filesystem::path root_dir{argv[1]};
  const std::filesystem::path config_file{argv[2]};

  std::unordered_set<std::filesystem::path> subdirs;
  std::unordered_map<std::string, std::filesystem::path> target_dirs;
  read_targets_from_file(config_file, subdirs, target_dirs);

  for (const auto& subdir : subdirs) {
    std::filesystem::create_directories(root_dir / subdir);
  }

  // `move_image()` will move an image's existing sidecar file alongside the
  // image, so defer processing XMP files to the end.
  std::vector<std::filesystem::path> xmp_files;

  const auto& images_dir{target_dirs["jpg"]};
  const auto& images_raw_dir{target_dirs["dng"]};
  for (const auto& dir : std::filesystem::directory_iterator{root_dir}) {
    const auto dir_path{dir.path()};

    if (subdirs.contains(dir_path.filename()) ||
        dir_path.filename() == ".DS_Store") {
      continue;
    }

    auto file_ext{dir_path.extension().string()};
    if (file_ext.empty()) {
      continue;
    }

    file_ext = file_ext.substr(1);
    for (auto& c : file_ext) {
      c = std::tolower(c);
    }

    if (file_ext == "xmp") {
      xmp_files.push_back(dir_path);
      continue;
    }

    std::filesystem::path target_dir;
    try {
      target_dir = target_dirs.at(file_ext);
    } catch (const std::out_of_range&) {
      target_dir = MISC_DIR;
    }

    if (target_dir == images_dir || target_dir == images_raw_dir) {
      move_image(dir_path, root_dir / target_dir);
    }

    else {
      move_file(dir_path, root_dir / target_dir);
    }
  }

  for (const auto& xmp_file : xmp_files) {
    try {
      move_file(xmp_file, root_dir / MISC_DIR);
    } catch (const std::filesystem::filesystem_error&) {
      // Do nothing if the image sidecar file had already been moved.
    }
  }
}

// Implementation details below

void read_targets_from_file(
    const std::filesystem::path& config_file,
    std::unordered_set<std::filesystem::path>& out_dirs,
    std::unordered_map<std::string, std::filesystem::path>& out_targets) {
  std::ifstream file{config_file};

  std::string line;
  const std::regex pattern{R"(^\s*?(\w+?)\s*?=\s*?(\w+?\/?\w+?)\s*?$)"};
  std::smatch match;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }

    std::regex_search(line, match, pattern);
    if (match.size() < 3) {
      continue;
    }

    const auto key{match[1].str()};
    const std::filesystem::path value{match[2]};

    out_dirs.insert(value);
    out_targets[key] = value;
  }
  out_dirs.insert(MISC_DIR);
}

void move_file(const std::filesystem::path& file,
               const std::filesystem::path& target_dir) {
  std::filesystem::rename(file, target_dir / file.filename());
}

void move_image(const std::filesystem::path& image_file,
                const std::filesystem::path& target_dir) {
  move_file(image_file, target_dir);

  const auto& sidecar_file{
      const_cast<std::filesystem::path&>(image_file).replace_extension("xmp")};
  try {
    move_file(sidecar_file, target_dir);
  } catch (const std::filesystem::filesystem_error&) {
    // Do nothing if a sidecar file does not exist.
  }
}

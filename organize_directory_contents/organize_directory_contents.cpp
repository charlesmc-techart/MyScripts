// A CLI program to organize the contents of a directory.
//
// Directory/
// |--3D/
// |  |--Blender/
// |  |--Maya/
// |
// |--Audio/
// |
// |--Archives/
// |
// |--Code/
// |    |--Assembly/
// |    |--C_Cpp/
// |    |--Javascript/
// |    |--Python/
// |    |--Shell/
// |
// |--Documents/
// |
// |--Images/
// |    |--Raw/
// |
// |--Misc/
// |
// |--Videos/

#include <filesystem>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const std::filesystem::path THREE_D_DIR{"3D"};
const std::filesystem::path AUDIO_DIR{"Audio"};
const std::filesystem::path ARCHIVES_DIR{"Archives"};
const std::filesystem::path CODE_DIR{"Code"};
const std::filesystem::path DOCUMENTS_DIR{"Documents"};
const std::filesystem::path IMAGES_DIR{"Images"};
const std::filesystem::path MISC_DIR{"Misc"};
const std::filesystem::path VIDEOS_DIR{"Videos"};
const std::unordered_set SUBDIRECTORIES{
    THREE_D_DIR,   AUDIO_DIR,  ARCHIVES_DIR, CODE_DIR,
    DOCUMENTS_DIR, IMAGES_DIR, MISC_DIR,     VIDEOS_DIR};

// Sub-subdirectories
const std::filesystem::path THREE_D_BLENDER_DIR{THREE_D_DIR / "Blender"};
const std::filesystem::path THREE_D_MAYA_DIR{THREE_D_DIR / "Maya"};
const std::filesystem::path CODE_ASSEMBLY_DIR{CODE_DIR / "Assembly"};
const std::filesystem::path CODE_C_CPP_DIR{CODE_DIR / "C_Cpp"};
const std::filesystem::path CODE_JAVASCRIPT_DIR{CODE_DIR / "Javascript"};
const std::filesystem::path CODE_PYTHON_DIR{CODE_DIR / "Python"};
const std::filesystem::path CODE_SHELL_DIR{CODE_DIR / "Shell"};
const std::filesystem::path IMAGES_RAW_DIR{IMAGES_DIR / "Raws"};

// The keys are lowercase file extensions
const std::unordered_map<std::string, std::filesystem::path>
    TARGET_SUBDIRECTORY_MAPPING{
        // 3D
        {"abc", THREE_D_DIR},
        {"fbx", THREE_D_DIR},
        {"obj", THREE_D_DIR},
        // 3D/Blender
        {"blend", THREE_D_BLENDER_DIR},
        {"blend1", THREE_D_BLENDER_DIR},
        // 3D/Maya
        {"ma", THREE_D_MAYA_DIR},
        {"mb", THREE_D_MAYA_DIR},
        // Audio
        {"mp3", AUDIO_DIR},
        // Archives
        {"7z", ARCHIVES_DIR},
        {"aar", ARCHIVES_DIR},
        {"zip", ARCHIVES_DIR},
        {"gz", ARCHIVES_DIR},
        {"xz", ARCHIVES_DIR},
        {"tar", ARCHIVES_DIR},
        {"txz", ARCHIVES_DIR},
        // Code
        {"env", CODE_DIR},
        {"s", CODE_ASSEMBLY_DIR},
        {"c", CODE_C_CPP_DIR},
        {"cc", CODE_C_CPP_DIR},
        {"cpp", CODE_C_CPP_DIR},
        {"h", CODE_C_CPP_DIR},
        {"hh", CODE_C_CPP_DIR},
        {"hpp", CODE_C_CPP_DIR},
        {"js", CODE_JAVASCRIPT_DIR},
        {"py", CODE_PYTHON_DIR},
        {"pyi", CODE_PYTHON_DIR},
        {"sh", CODE_SHELL_DIR},
        {"bash", CODE_SHELL_DIR},
        {"zsh", CODE_SHELL_DIR},
        // Serialization
        {"db", CODE_DIR},
        {"json", CODE_DIR},
        {"xml", CODE_DIR},
        {"yml", CODE_DIR},
        // Documents
        {"txt", DOCUMENTS_DIR},
        {"md", DOCUMENTS_DIR},
        {"rst", DOCUMENTS_DIR},
        {"csv", DOCUMENTS_DIR},
        {"tsv", DOCUMENTS_DIR},
        {"pdf", DOCUMENTS_DIR},
        {"doc", DOCUMENTS_DIR},
        {"docx", DOCUMENTS_DIR},
        {"xls", DOCUMENTS_DIR},
        // Images
        {"heic", IMAGES_DIR},
        {"jpg", IMAGES_DIR},
        {"jpeg", IMAGES_DIR},
        {"png", IMAGES_DIR},
        {"webp", IMAGES_DIR},
        {"tif", IMAGES_DIR},
        {"tiff", IMAGES_DIR},
        {"psd", IMAGES_DIR},
        // Images/Raw
        {"dng", IMAGES_RAW_DIR},
        {"orf", IMAGES_RAW_DIR},
        // Videos
        {"mkv", VIDEOS_DIR},
        {"mov", VIDEOS_DIR},
        {"mp4", VIDEOS_DIR},
    };

// Move `file` into `target_dir`.
void move_file(const std::filesystem::path&, const std::filesystem::path&);

// Move an image and its sidecar file to `target_dir`.
void move_image(const std::filesystem::path&, const std::filesystem::path&);

// main

int main(const int, const char* argv[]) {
  const std::filesystem::path root_dir{argv[1]};

  for (const auto& subdir : SUBDIRECTORIES) {
    std::filesystem::create_directories(root_dir / subdir);
  }
  for (const auto& subdir :
       {THREE_D_BLENDER_DIR, THREE_D_MAYA_DIR, CODE_ASSEMBLY_DIR,
        CODE_C_CPP_DIR, CODE_JAVASCRIPT_DIR, CODE_PYTHON_DIR, CODE_SHELL_DIR,
        IMAGES_RAW_DIR}) {
    std::filesystem::create_directories(root_dir / subdir);
  }

  // `move_image()` will move an image's existing sidecar file alongside the
  // image, so defer processing XMP files to the end.
  std::vector<std::filesystem::path> xmp_files;

  for (const auto& dir : std::filesystem::directory_iterator{root_dir}) {
    const auto dir_path{dir.path()};

    if (SUBDIRECTORIES.contains(dir_path.filename()) ||
        dir_path.filename() == ".DS_Store") {
      continue;
    }

    // Utilize short-circuiting by checking if the file extension is "xmp" only
    // at the end.
    auto file_ext{dir_path.extension().string()};
    if (!file_ext.empty() && (file_ext = file_ext.substr(1)) == "xmp") {
      xmp_files.push_back(dir_path);
      continue;
    }

    std::filesystem::path target_dir;
    try {
      target_dir = TARGET_SUBDIRECTORY_MAPPING.at(file_ext);
    } catch (const std::out_of_range&) {
      target_dir = MISC_DIR;
    }

    if (target_dir == IMAGES_DIR || target_dir == IMAGES_RAW_DIR) {
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

void move_file(const std::filesystem::path& file,
               const std::filesystem::path& target_dir) {
  std::filesystem::rename(file, target_dir / file.filename());
}

void move_image(const std::filesystem::path& image_file,
                const std::filesystem::path& target_dir) {
  move_file(image_file, target_dir);

  const auto sidecar_file{
      const_cast<std::filesystem::path&>(image_file).replace_extension("xmp")};
  try {
    move_file(sidecar_file, target_dir);
  } catch (const std::filesystem::filesystem_error&) {
    // Do nothing if a sidecar file does not exist.
  }
}

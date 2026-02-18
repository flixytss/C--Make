#include <exutils.h>
#include <files.h>
#include <filesystem>
#include <print>
#include <string>

const std::string elfstr = {0x7f, 0x45, 0x4c, 0x46};
const std::string astr = {0x21, 0x3C, 0x61, 0x72, 0x63, 0x68, 0x0A, 0x0A};
enum type {
    elf = 0,
    so,
    a,
    h,
    none
};
const enum type filetype(const std::string path) {
    const std::string buff = ReadFile(path);

    if (buff.starts_with(elfstr) && !path.ends_with(".so") && !path.ends_with(".o") && !path.ends_with(".bin") && !path.ends_with(".out")) return elf;
    if (buff.starts_with(elfstr) && path.ends_with(".so")) return so;
    if (buff.starts_with(astr)) return a;
    if (path.ends_with(".h") || path.ends_with(".hpp")) return h;
    return none;
}

void copyFolderRecursive(const std::filesystem::path& source, const std::filesystem::path& destination) {
    try {
    std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
    } catch (const std::filesystem::filesystem_error& e) {
        std::println("{}ERR{}: {}", REDB, RESET, e.what());
        Finish(1);
    }
}

void install_library(const std::string path) {
    const std::string name = std::filesystem::path(path).filename().string();
    for (const auto entry : libraries) {
        if (entry.is_directory()) if (entry.path().filename() == name) {
            std::println("{}ERR{}: There is already a library with that name", REDB, RESET);
            Finish(1);
        }
    }

    std::filesystem::create_directory("." + name);
    std::filesystem::create_directory("." + name + "/include");
    std::filesystem::create_directory("." + name + "/lib");
    std::filesystem::create_directory("." + name + "/bin");

    const auto copy = [name](const std::string where, const std::filesystem::directory_entry entry){
        if ( std::filesystem::exists("." + name + where + entry.path().filename().string()) ) std::filesystem::remove("." + name + where + entry.path().filename().string());
        std::filesystem::copy_file(entry, "." + name + where + entry.path().filename().string());
    };

    for (const std::filesystem::directory_entry entry : std::filesystem::recursive_directory_iterator(path)) {
        if (!entry.is_directory()) switch (filetype(entry.path())) {
            case a:
            case so:
                copy("/lib/", entry); break;
            case elf:
                copy("/bin/", entry); break;
            case h:
                copy("/include/", entry); break;
            default: break;
        }
    }

    copyFolderRecursive("." + name, librariesdirectory + "/" + name);
    std::filesystem::remove_all("." + name);
}
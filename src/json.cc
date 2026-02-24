#include <exutils.h>
#include <filesystem>
#include <print>
#include <simdjson.h>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

extern int files_inside_a_directory(const std::string path);
const EntryInfo GetInfFromJson(const std::filesystem::path path) {
    EntryInfo Inf;
    simdjson::dom::parser parser;
    
    simdjson::dom::element doc;
    try { doc = parser.load(path.string()); }
    catch (const simdjson::simdjson_error& e) {
        std::println("{}ERR{}: {}", REDB, RESET, e.what());
        Finish(1);
    }

    if ( !doc.is_object() ) {
        std::println("{}ERR{}: Main object not found!", REDB, RESET);
        Finish(1);
    }
    const simdjson::simdjson_result<simdjson::dom::object> root = doc.get_object();

    std::string* library = NULL;
    for (const auto json : root) {
        switch (str2int(json.key.data())) {
            case str2int("files"):
                if (json.value.type() == simdjson::dom::element_type::ARRAY) {
                    for (const auto file : json.value) {
                        if (file.is_array()) {
                            if (!file.get_array()->at(0)->is_string()) {
                                std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, json.key);
                                Finish(1);
                            }
                            if (!file.get_array()->at(1)->is_string() && !file.get_array()->at(1).is_null()) {
                                std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, json.key);
                                Finish(1);
                            }

                            if ( !std::filesystem::exists(file.get_array()->at(0)->get_string()->data()) ) {
                                std::println("{}ERR{}: Directory {} dosen't exists", REDB, RESET, file.get_array()->at(0)->get_string()->data());
                                Finish(1);
                            } else {
                                if (!std::filesystem::directory_entry(std::filesystem::path(file.get_array()->at(0)->get_string()->data())).is_directory()) {
                                    std::println("{}ERR{}: File {} isn't a directory", REDB, RESET, file.get_array()->at(0)->get_string()->data());
                                    Finish(1);
                                }
                            }
                            for (const std::filesystem::directory_entry entry : std::filesystem::directory_iterator(file.get_array()->at(0)->get_string()->data())) {
                                if (file.get_array()->at(1)->is_null()) { Inf.Files.push_back(entry.path().string()); }
                                else {
                                    if (entry.path().filename().string().ends_with(file.get_array()->at(1)->get_string()->data()))
                                        Inf.Files.push_back(entry.path().string());
                                }
                            }
                        } else
                            Inf.Files.push_back(file.get_string()->data());
                    }

                } else {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (ARRAY)", REDB, RESET, json.key);
                    Finish(1);
                }
                break;
            case str2int("buildir"):
                if (json.value.type() != simdjson::dom::element_type::STRING) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, json.key);
                    Finish(1);
                }
                Inf.BuildDirectory = json.value;
                break;
            case str2int("link-args"):
                if (json.value.type() != simdjson::dom::element_type::ARRAY) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (ARRAY)", REDB, RESET, json.key);
                    Finish(1);
                }
                for (const auto obj : json.value) { Inf.LinkArgs.push_back(obj.get_string()->data()); }
                break;
            case str2int("compile-args"):
                if (json.value.type() != simdjson::dom::element_type::OBJECT) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (ARRAY)", REDB, RESET, json.key);
                    Finish(1);
                }
                for (const auto obj : json.value.get_object()) {
                    if (obj.value.type() != simdjson::dom::element_type::STRING) {
                        std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, json.key);
                        Finish(1);
                    }
                    Inf.CompileArgs.push_back(std::make_tuple(obj.value.get_string()->data(), obj.key.data()));
                }
                break;
            case str2int("project-name"):
                if (json.value.type() != simdjson::dom::element_type::STRING) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, json.key);
                    Finish(1);
                }
                Inf.ProjectName = json.value.get_string()->data();
                break;
            case str2int("run-before"):
                if (json.value.type() != simdjson::dom::element_type::ARRAY) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (ARRAY)", REDB, RESET, json.key);
                    Finish(1);
                }
                for (const auto command : json.value) {
                    if ( !command.is_string() ) {
                        std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, json.key);
                        Finish(1);
                    }
                    Inf.Run.push_back(command.get_string()->data());
                }
                break;
            case str2int("compilers-filter"):
                if (json.value.type() != simdjson::dom::element_type::OBJECT) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (ARRAY)", REDB, RESET, json.key);
                    Finish(1);
                }
                for (const auto obj : json.value.get_object()) {
                    if (obj.value.type() != simdjson::dom::element_type::STRING) {
                        std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, obj.key, json.key);
                        Finish(1);
                    }
                    Inf.CompilerFilter.push_back(std::make_tuple(obj.key.data(), obj.value.get_string()->data()));
                }
                break;
            case str2int("includes"):
                if (json.value.type() != simdjson::dom::element_type::OBJECT) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (ARRAY)", REDB, RESET, json.key);
                    Finish(1);
                }
                for (const auto obj : json.value.get_object()) {
                    if (obj.value.type() != simdjson::dom::element_type::STRING) {
                        std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, obj.key, json.key);
                        Finish(1);
                    }
                    Inf.CompileArgs.push_back(std::make_tuple("-I" + (std::string){obj.value.get_string()->data()}, obj.key.data()));
                }
                break;
            case str2int("output-file"):
                if (json.value.type() != simdjson::dom::element_type::STRING) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (STRING)", REDB, RESET, json.key);
                    Finish(1);
                }
                Inf.OutputFile = json.value.get_string()->data();
                break;
            case str2int("extra"):
                if (json.value.type() != simdjson::dom::element_type::OBJECT) {
                    std::println("{}ERR{}: IN \"{}\": \"A wrong type\"; Wrong type (ARRAY)", REDB, RESET, json.key);
                    Finish(1);
                }
                for (const auto obj : json.value.get_object()) {
                    switch (str2int(obj.key.data())) {
                        case str2int("use-ccache"):
                            if (obj.value.type() != simdjson::dom::element_type::BOOL) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (BOOL): {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_bool().value());
                                Finish(1);
                            }
                            Inf.Ccache = obj.value.get_bool().value();
                            break;
                        case str2int("use-linker"):
                            if (obj.value.type() != simdjson::dom::element_type::STRING) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (STRING): {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_string()->data());
                                Finish(1);
                            }
                            Inf.Linker = obj.value.get_string()->data();
                            break;
                        case str2int("add-clean"):
                            if (obj.value.type() != simdjson::dom::element_type::BOOL) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (BOOL): {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_bool().value());
                                Finish(1);
                            }
                            Inf.CleanUpFirst = obj.value.get_bool().value();
                            break;
                        case str2int("only-linker"):
                            if (obj.value.type() != simdjson::dom::element_type::BOOL) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (BOOL): {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_bool().value());
                                Finish(1);
                            }
                            Inf.OnlyLinker = obj.value.get_bool().value();
                            break;
                        case str2int("use-launcher"):
                            if (obj.value.type() != simdjson::dom::element_type::ARRAY) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (STRING): {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_string()->data());
                                Finish(1);
                            }
                            if (!obj.value.get_array()->at(0).is_string()) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (STRING)", REDB, RESET, obj.key.data(), json.key);
                                Finish(1);
                            }
                            if (!obj.value.get_array()->at(1).is_string()) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (STRING)", REDB, RESET, obj.key.data(), json.key);
                                Finish(1);
                            }
                            Inf.Launchers.push_back(std::make_tuple(obj.value.get_array()->at(0)->get_string()->data(), obj.value.get_array()->at(1)->get_string()->data()));
                            break;
                        case str2int("use-lib"):
                            if (obj.value.type() != simdjson::dom::element_type::ARRAY) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (STRING): {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_string()->data());
                                Finish(1);
                            }
                            if (!obj.value.get_array()->at(0).is_string()) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (STRING)", REDB, RESET, obj.key.data(), json.key);
                                Finish(1);
                            }
                            if (!obj.value.get_array()->at(1).is_string()) {
                                std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (STRING)", REDB, RESET, obj.key.data(), json.key);
                                Finish(1);
                            }
                            for (const auto li : libraries) if ((std::string){obj.value.get_array()->at(0)->get_string()->data()} == li.path().filename().string()) {
                                    if (!std::filesystem::exists(li.path().string() + "/include") || !std::filesystem::exists(li.path().string() + "/lib")) {
                                        std::println("{}ERR{}: Library {} dosen't have the correct structure", REDB, RESET, li.path().string());
                                        Finish(1);
                                    }

                                    library = new std::string;
                                    library->resize(li.path().string().size());
                                    memcpy(library->data(), li.path().c_str(), li.path().string().size());

                                    if (!files_inside_a_directory(li.path().string() + "/include")) {
                                        std::println("{}WARN{}: Using empty library {}", YELLOW, RESET, obj.value.get_array()->at(0)->get_string()->data());
                                    }
                            }
                            if (!library) {
                                std::println("{}ERR{}: Library not found! (Try installing it in ~/.local/state/c++make/libraries/\"Your library\")", REDB, RESET);
                                Finish(1);
                            }
                            std::println("{}INF{}: Using library {}", YELLOW, RESET, obj.value.get_array()->at(0)->get_string()->data());

                            Inf.CompileArgs.push_back(std::make_tuple(("-I" + (*library) + "/include"), obj.value.get_array()->at(1)->get_string()->data()));
                            Inf.LinkArgs.push_back(("-L" + (*library) + "/lib"));

                            delete library;
                            library = NULL;
                            break;
                        case str2int("set-cores"):
                            if (obj.value.type() == simdjson::dom::element_type::INT64) {
                                if (!obj.value.get_int64()) { std::println("{}WARN{}: Cores was set to zero or a lower number, Disabling paralel builds", YELLOW, RESET); break;}
                                Inf.Cores = obj.value.get_int64();
                            }
                            else if (obj.value.type() == simdjson::dom::element_type::STRING) { if ((std::string){obj.value.get_string()->data()} == "...") Inf.Cores = std::thread::hardware_concurrency(); }
                            else {
                                try { std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Wrong type (BOOL or STRING): {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_bool().value()); }
                                catch (const simdjson::simdjson_error& e) {
                                    std::println("{}ERR{}: {}", REDB, RESET, e.what());
                                    Finish(1);
                                }

                                Finish(1);
                            }
                            break;
                        default:
                            std::println("{}ERR{}: IN \"{}\" THAT IS IN \"{}\": Unknown extra: {}", REDB, RESET, obj.key.data(), json.key, obj.value.get_string()->data());
                            Finish(1);
                    }
                }
                break;
            default: break;
        }
    }
    return Inf;
}
#include <files.h>
#include <print>
#include <string>
#include <exutils.h>
#include <thread>

bool DidTheFileChange(const std::string& file)
{
    static std::string oldContent;
    std::string newContent = ReadFile(file);

    if (oldContent.empty()) {
        oldContent = newContent;
        return false;
    }
    if (oldContent != newContent) {
        oldContent = newContent;
        return true;
    }
    return false;
}
void run(std::string file, EntryInfo* inf) {
    std::println("[DEBUG] Running thread {}...", std::this_thread::get_id());
    MakeFile(inf);
    std::println("[DEBUG] Stopping thread...");
}
void BackgroundProccess(std::string file) {
    while (true) {
        if (DidTheFileChange(file)) {
            auto inf = GetInf(file);
            std::jthread(run, file, inf).detach();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

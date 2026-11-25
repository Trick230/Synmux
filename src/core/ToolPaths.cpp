#include "core/ToolPaths.hpp"
#include <array>
#include <string_view>

// OS specific headers
#ifdef _WIN32
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <limits.h>
#else
    #include <unistd.h>
#endif

// Automatic compile-time executable extension
namespace {
    #ifdef _WIN32
        constexpr std::string_view kExeExt = ".exe";
    #else
        constexpr std::string_view kExeExt = "";
    #endif

    fs::path Bin(const fs::path& dir, std::string_view name) {
        std::string filename(name);
        filename += kExeExt;
        return dir / filename;
    }
}

// Get the executable's real directory
fs::path ToolPaths::GetExeDir() {
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return fs::path(buffer).parent_path();
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        return fs::canonical(buffer).parent_path();
    }
    
    return fs::current_path();
#else
    std::error_code ec;
    auto exePath = fs::read_symlink("/proc/self/exe", ec);
    return ec ? fs::current_path() : exePath.parent_path();
#endif
}

// Mount paths (no RAM allocation)
ToolPaths ToolPaths::LoadDefault() {
    const fs::path base = GetExeDir() / "tools";
    ToolPaths p;

    p.ffmpeg     = Bin(base / "ffmpeg",   "ffmpeg");
    
    p.mkvmerge   = Bin(base / "mkvtools", "mkvmerge");
    p.mkvextract = Bin(base / "mkvtools", "mkvextract");
    
    p.x264       = Bin(base / "Video" / "x264",   "x264");
    p.x265       = Bin(base / "Video" / "x265",   "x265");
    p.svtav1     = Bin(base / "Video" / "Av1",    "SvtAv1EncApp");
    p.nvencc     = Bin(base / "Video" / "NVEncC", "NVEncC64");
    
    p.qaac       = Bin(base / "Audio" / "qaac", "qaac64");
    p.opusenc    = Bin(base / "Audio" / "opus", "opusenc");

    return p;
}

std::vector<std::string> ToolPaths::GetMissing() const {
    const std::array<const fs::path*, 9> all = {
        &ffmpeg,
        &mkvmerge, &mkvextract,
        &x264, &x265, &svtav1, &nvencc,
        &qaac, &opusenc
    };

    std::vector<std::string> missing;
    for (const fs::path* bin : all) {
        if (!fs::exists(*bin)) {
            missing.push_back(bin->filename().string());
        }
    }

    return missing;
}
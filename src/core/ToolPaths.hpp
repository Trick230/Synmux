#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct ToolPaths {
    fs::path ffmpeg;
    fs::path mkvmerge;
    fs::path mkvextract;
    fs::path x264;
    fs::path x265;
    fs::path svtav1;
    fs::path nvencc;
    fs::path qaac;
    fs::path opusenc;

    static fs::path GetExeDir();

    static ToolPaths LoadDefault();

    std::vector<std::string> GetMissing() const;
};
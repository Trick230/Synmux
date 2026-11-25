#include "core/ToolPaths.hpp"
#include <iostream>

int main() {
    std::cout << "=== Synmux v0.1 ===\n";

    const ToolPaths tools = ToolPaths::LoadDefault();
    const auto missing = tools.GetMissing();

    if (missing.empty()) {
        std::cout << "[OK] All 9 binaries detected and portable.\n";
    } else {
        std::cout << "[ERROR] Missing the following binaries in tools/:\n";
        for (const auto& name : missing) {
            std::cout << "  - " << name << "\n";
        }

        return 1;
    }

    return 0;
}
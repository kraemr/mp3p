#include "../include/stdafx.hpp"
std::vector<std::string> splitString(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> result;
    std::size_t pos = 0;
    std::size_t found;
    while ((found = input.find(delimiter, pos)) != std::string::npos) {
        result.push_back(input.substr(pos, found - pos));
        pos = found + delimiter.length();
    }
    result.push_back(input.substr(pos));
    return result;
}

bool fileExists(const std::filesystem::path& p)
{
    std::filesystem::file_status s = std::filesystem::file_status{};
    if (std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(p))
        return true;
    else
        return false;
}

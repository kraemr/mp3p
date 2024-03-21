#pragma once
#include <map>
#include <string>
#include <iostream>
extern std::map<const char*,unsigned long long> Cache;
unsigned long long getCachedDuration(std::string path);
void setCachedDuration(std::string path,unsigned long long duration);
void flushCache();
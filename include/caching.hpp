#pragma once
#include <map>
#include <string>
#include <iostream>
unsigned long long getCachedDuration(std::string path);
void setCachedDuration(std::string path,unsigned long long duration);
void flushCache();
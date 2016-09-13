#ifndef __URL__
#define __URL__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utf8.h>
#include <stdint.h>

using namespace std;

size_t uri_decode(string& decoded, const string& encoded);
size_t uri_encode(string& encoded, const string& decoded);
bool is_reserved(uint32_t check);
#endif // __URL__

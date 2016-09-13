#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utf8.h>

#include "url.hpp"

using namespace std;

static char unreserved[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";

size_t uri_decode(string& decoded , const string& encoded)
{
    size_t pos = 0;
    stringstream ss;
    stringstream result;
    unsigned int decode = 0;

    while(pos < encoded.length())
    {
        if(encoded[pos] == '%')
        {
            ++pos;

            ss.clear();
            ss << hex << encoded.substr(pos, 2);
            ss >> decode;
            result << (char)decode;
            pos+=2;
        }
        else
        {
            result << encoded[pos];
            ++pos;
        }
    }

    decoded = result.str();
    return decoded.length();
}


size_t uri_encode(string& encoded, const string& decoded)
{
    stringstream result;
    uint32_t candidate = 0;
    utf8::iterator<const char*> it_dec(&decoded[0], &decoded[0], &decoded[0] + decoded.length());
    utf8::iterator<const char*> it_dec_end(&decoded[0] + decoded.length(), &decoded[0], &decoded[0] + decoded.length());
    vector<unsigned char> utf8symbol;
    uint32_t utf32symbol[2] = {0,};

    result.clear();
    while(it_dec != it_dec_end)
    {
        candidate = *it_dec;
        if(is_reserved(candidate))
        {
            utf32symbol[0] = candidate;
            utf8symbol.clear();
            utf8::utf32to8(utf32symbol, utf32symbol+1, back_inserter(utf8symbol));
            vector<unsigned char>::iterator it;
            result<< '%';
            for(it = utf8symbol.begin(); it != utf8symbol.end(); ++it)
                result<< hex << uppercase << (unsigned int)(*it);
        }
        else
        {
            utf32symbol[0] = candidate;
            utf8symbol.clear();
            utf8::utf32to8(utf32symbol, utf32symbol+1, back_inserter(utf8symbol));
            vector<unsigned char>::iterator it;
            for(it = utf8symbol.begin(); it != utf8symbol.end(); ++it)
                result<< (char)*it;

        }
        ++it_dec;
    }
    encoded = result.str();
    return encoded.length();
}


bool is_reserved(uint32_t check)
{
    uint32_t comp = 0;
    bool reserved = true;
    string unres(unreserved);
    utf8::iterator<const char*> it(&unres[0], &unres[0], &unres[0] + unres.length());
    utf8::iterator<const char*> it_end(&unres[0] + unres.length(), &unres[0], &unres[0] + unres.length());

    while((it != it_end) && (reserved))
    {
        comp = *it;
        if(comp == check)
            reserved = false;
        ++it;
    }
    return reserved;
}

/*
int main(int argc, char** argv)
{
    bool decode = true;
    string uri = "";
    if(argc == 2)
        uri = argv[1];
    else if (argc == 3)
    {
        if(string(argv[1]).compare("-decode") == 0)
        {
            decode = true;
            uri = argv[2];
        }
        else if(string(argv[1]).compare("-encode") == 0)
        {
            decode = false;
            uri = argv[2];
        }
        else
        {
            cerr<< "Usage: " << argv[0] << " <uri>" << endl;
            return -1;
        }
    }
    else
    {
        cerr<< "Usage: " << argv[0] << " <uri>" << endl;
        return -1;
    }

    if(decode)
    {
        string decoded = "";
        uri_decode(decoded, uri);
        cout << decoded << endl;
    }
    else
    {
        string encoded = "";
        uri_encode(encoded, uri);
        cout << encoded << endl;
    }
    return 0;
}
*/

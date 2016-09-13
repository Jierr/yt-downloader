#ifndef __YTSRC_DEFINITIONS__
#define __YTSRC_DEFINITIONS__

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
class PlaybackContext{
	private:
		string url;
		string quality;
		string fallback;

	public:
		PlaybackContext();
		PlaybackContext(const PlaybackContext& cp);
		~PlaybackContext();
		bool url_set;
		bool quality_set;
		bool fallback_set;
		bool all_set();
		const string& get_url(){return url;}
		const string& get_quality(){return quality;}
		const string& get_fallback(){return fallback;}
		void set_url(const string& val);
		void set_quality(const string& val);
		void set_fallback(const string& val);
        void reset();
};

class Playbacks{
	private:
		vector<PlaybackContext> ctx;
		vector<PlaybackContext>::iterator curr;
	public:
		Playbacks();
		~Playbacks();
		void add_url(const string& url);
		void add_quality(const string& quality);
		void add_fallback(const string& fallback);
		void print();
		bool first();
		bool last();
		bool next();
		string get_url();
		string get_fallback();
		string get_quality();
        void reset();
};

extern "C" Playbacks pbs;

int parse(char* buf);
void* lex_scan_string(char* buf);
void lex_delete_buffer(void* buf);



#endif

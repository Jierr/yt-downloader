#include "ytsrc_definitions.hpp"

Playbacks pbs;

PlaybackContext::PlaybackContext()
{
	url_set = false;
	quality_set = false;
	fallback_set = false;
	url = "";
	quality = "";
	fallback = "";
}

PlaybackContext::~PlaybackContext()
{
}

void PlaybackContext::reset()
{
    url_set = false;
    quality_set = false;
    fallback_set = false;
    url = "";
    quality = "";
    fallback = "";
}

PlaybackContext::PlaybackContext(const PlaybackContext& cp)
{
	if(this != &cp)
		*this = cp;
}

bool PlaybackContext::all_set()
{
	return (url_set && quality_set && fallback_set);
}


void PlaybackContext::set_url(const string& val)
{
	url_set = true;
	url = val;
}

void PlaybackContext::set_quality(const string& val)
{
	quality_set = true;
	quality = val;
}

void PlaybackContext::set_fallback(const string& val)
{
	fallback_set = true;
	fallback = val;
}

Playbacks::Playbacks()
{
	ctx.clear();
	curr = ctx.end();
}

Playbacks::~Playbacks()
{
	ctx.clear();
	curr = ctx.end();
}

void Playbacks::reset()
{
    ctx.clear();
    curr = ctx.end();
}

void Playbacks::add_url(const string& url)
{
	if(ctx.size() > 0)
	{
		curr = ctx.end();
		--curr;
	}

	if((curr == ctx.end()) || curr->url_set)
	{
		PlaybackContext playback;
		playback.set_url(url);
		ctx.push_back(playback);
		curr = ctx.end();
		--curr;
	}
	else
	{
		curr->set_url(url);
	}
}

void Playbacks::add_quality(const string& quality)
{
	if(ctx.size() > 0)
	{
		curr = ctx.end();
		--curr;
	}

	if((curr == ctx.end()) || curr->quality_set)
	{
		PlaybackContext playback;
		playback.set_quality(quality);
		ctx.push_back(playback);
		curr = ctx.end();
		--curr;
	}
	else
	{
		curr->set_quality(quality);
	}
}

void Playbacks::add_fallback(const string& fallback)
{
	if(ctx.size() > 0)
	{
		curr = ctx.end();
		--curr;
	}

	if((curr == ctx.end()) || curr->fallback_set)
	{
		PlaybackContext playback;
		playback.set_fallback(fallback);
		ctx.push_back(playback);
		curr = ctx.end();
		--curr;
	}
	else
	{
		curr->set_fallback(fallback);
	}
}

void Playbacks::print()
{
	int c = 1;
	vector<PlaybackContext>::iterator it = ctx.begin();
	while(it != ctx.end())
	{
		cout<< "Playback " << c << "=============" << endl;
		cout<< "URL: " << it->get_url() << endl;
		cout<< "QUALITY: " << it->get_quality() << endl;
		cout<< "FALLBACK: " << it->get_fallback() << endl;
		++it;
		++c;
	}
}


bool Playbacks::first()
{
	curr = ctx.begin();
	if(curr == ctx.end())
		return false;
	return true;
}

bool Playbacks::last()
{
	curr = ctx.end();
	if(curr != ctx.begin())
		--curr;

	if(curr == ctx.end())
		return false;
	return true;
}

bool Playbacks::next()
{
	if(curr != ctx.end())
		++curr;

	if(curr == ctx.end())
		return false;
	return true;
}

string Playbacks::get_url()
{
	if(curr != ctx.end())
		return curr->get_url();
	return "N/A";
}

string Playbacks::get_fallback()
{
	if(curr != ctx.end())
		return curr->get_fallback();
	return "N/A";
}

string Playbacks::get_quality()
{
	if(curr != ctx.end())
		return curr->get_quality();
	return "N/A";
}

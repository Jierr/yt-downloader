#ifndef YTDOWNLOAD_H
#define YTDOWNLOAD_H

#include <string>
#include <string.h>
#include <curl/curl.h>
#include <QtCore>
#include <ytsrc_definitions.hpp>
#include "url.hpp"

using namespace std;

#define INIT_LEN 1024
#define YT_LEN 1024*1024*16


typedef struct
{
    char* buffer;
    size_t len;
    size_t curr;
    size_t realloc_size;
} curl_ctx_t;

size_t init_curl_ctx(curl_ctx_t *ctx, size_t size);
void clear_curl_ctx(curl_ctx_t *ctx);
void set_realloc_size(curl_ctx_t *ctx, size_t size);

class YTDownload;
typedef struct
{
    YTDownload* ytd;
    double pc;
} curl_progress_ctx_t;

int cpc_init(curl_progress_ctx_t* pctx, YTDownload* ytd);
void cpc_reset(curl_progress_ctx_t* pctx);

size_t write_data(void *buffer, size_t size, size_t nmemb, void *internal);
int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow);
enum {STATE_READY, STATE_FAIL, STATE_INIT};
class YTDownload
{
private:
    curl_ctx_t ctx;
    curl_progress_ctx_t pctx;
    CURL* handle;
    char berror[CURL_ERROR_SIZE];
    string url;
    int qualityIndex;
    int state;
    int (* cb_progress)(void*, double, double, double, double);

public:
    YTDownload();
    ~YTDownload();
    QMutex* mprogress;
    void setProgressMutex(QMutex* mutex);
    void setProgressCallback(int (* cb)(void*, double, double, double, double));
    void resetProgress();
    const curl_progress_ctx_t& getProgress();
    void  setProgress(const double& pc);
    void setUrl(string url);
    void setQualityIndex(int qi);
    int getQualityIndex();
    void setState(int state);
    int getState();
    string getCurlError();


    int init(string url);
    int parseVideoHtml();
    int chooseQuality(string qual);
    int downloadVideo(int tries);
};

#endif // YTDOWNLOAD_H

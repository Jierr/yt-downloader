#include "ytdownload.h"

YTDownload::YTDownload()
{
    url = "";
    qualityIndex = 0;
    state = STATE_FAIL;
    handle = 0;
    mprogress = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    memset(&ctx, 0, sizeof(curl_ctx_t));
    init_curl_ctx(&ctx, INIT_LEN);
    cpc_init(&pctx, this);
}

YTDownload::~YTDownload()
{
    if(handle)
    {
        curl_easy_cleanup(handle);
        handle = 0;
    }
    clear_curl_ctx(&ctx);
    curl_global_cleanup();
}


void YTDownload::setProgressCallback(int (* cb)(void*, double, double, double, double))
{
    this->cb_progress = cb;
}


void YTDownload::setProgressMutex(QMutex* mutex)
{
    mprogress = mutex;
}


void YTDownload::resetProgress()
{
    cpc_reset(&pctx);
}

const curl_progress_ctx_t& YTDownload::getProgress()
{

    return this->pctx;
}


void  YTDownload::setProgress(const double& pc)
{
    mprogress->lock();
    pctx.pc = pc;
    mprogress->unlock();
}

void YTDownload::setUrl(string url)
{
    this->url = url;
}


void YTDownload::setQualityIndex(int qi)
{
    qualityIndex = qi;
}

int YTDownload::getQualityIndex()
{
    return qualityIndex;
}


void YTDownload::setState(int state)
{
    this->state = state;
}

int YTDownload::getState()
{
    return this->state;
}

string YTDownload::getCurlError()
{
    return string(berror);
}

int YTDownload::init(string url)
{
    CURLcode result;
    clear_curl_ctx(&ctx);
    if(init_curl_ctx(&ctx, INIT_LEN)==0)
        return -1;


    setUrl(url);
    resetProgress();

    if(handle)
    {
        curl_easy_cleanup(handle);
        handle = 0;
    }
    handle = curl_easy_init();
    if(handle)
    {
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, FALSE);
        //curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &ctx);
        curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, berror);
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION , 1L);
        curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0");
        result = curl_easy_perform(handle);
        if(result != CURLE_OK)
            return -1;
    }
    else
        return -1;
    return 0;
}


int YTDownload::parseVideoHtml()
{
    pbs.reset();
    return parse(ctx.buffer);
}


int YTDownload::downloadVideo(int tries)
{
    CURLcode result;
    int v = qualityIndex;
    int dtry = 0;
    bool cont = false;
    string url_decoded = "";
    bool done = false;

    cont = pbs.first();
    while(cont && (v > 0))
    {

        --v;
        cont = pbs.next();
    }


    if(v >= 0)
    {
        uri_decode(url_decoded, pbs.get_url());
        curl_easy_setopt(handle, CURLOPT_URL, url_decoded.c_str());
        //curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1);
        //curl_easy_setopt(handle, CURLOPT_TCP_KEEPIDLE, 5);
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, &pctx);
        curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, cb_progress);
        curl_easy_setopt(handle, CURLOPT_EXPECT_100_TIMEOUT_MS , 10000L);
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION , 1L);
        clear_curl_ctx(&ctx);
        if(init_curl_ctx(&ctx, YT_LEN)==0)
        {
            cout << ">>Error<downloadVideo> (init_curl_ctx(&ctx, YT_LEN)==0) failed!" << endl;
            return -2;
        }

        while(!done && ( ((tries > 0) && dtry<tries) || tries<=0 ) )
        {
            resetProgress();

            result = curl_easy_perform(handle);
            if(result != CURLE_OK)
            {
                cout<< ">>Error<downloadVideo> curl_easy_perform(handle) != CURLE_OK" << endl
                    << ">>Message:  " << berror << endl;
               //return -3;
            }
            else
            {
                FILE* file = 0;
                size_t fsize = 0;
                file = fopen("video.mov", "wb");
                //if(!file)
                //    return -5;
                fsize = fwrite(ctx.buffer, ctx.curr, 1, file);
                fclose(file);
                if (fsize > 0)
                    done = true;

            }
            ++dtry;
        }
        cout<< dtry << " Versuche" << endl;
        if (!done)
            return -6;
    }
    else
        return -1;
    return 0;
}




size_t init_curl_ctx(curl_ctx_t *ctx, size_t size)
{
    ctx->buffer = (char*)malloc(size* sizeof(char));
    ctx->len = size;
    ctx->curr = 0;
    ctx->realloc_size = size;
    if(!ctx->buffer)
        return 0;

    return size;
}

void clear_curl_ctx(curl_ctx_t *ctx)
{
    if(ctx->buffer)
        free(ctx->buffer);
    ctx->len = 0;
    ctx->curr = 0;
}

void set_realloc_size(curl_ctx_t *ctx, size_t size)
{
    ctx->realloc_size = size;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *internal)
{

    curl_ctx_t* ctx = (curl_ctx_t*)internal;
    size_t len = ctx->curr+1 + size*nmemb;
    size_t result = 0;
    cout<< "retreived:\t\t" << ctx->curr/1024 << "KiB" << "\trealloc_size: "
        << ctx->realloc_size/1024 << "KiB -> " << ctx->len/1024 << "KiB" <<endl;
    if(len >= ctx->len)
    {
        ctx->buffer = (char*)realloc(ctx->buffer, len+ctx->realloc_size * sizeof(char));
        ctx->len = len+ctx->realloc_size;
    }
    if(ctx->buffer)
    {
        result = size*nmemb;
        memcpy(&ctx->buffer[ctx->curr], buffer, size*nmemb);
        ctx->curr+=size*nmemb;
        ctx->buffer[ctx->curr] = 0;
    }

    return result;

}


int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    curl_progress_ctx_t* pctx = (curl_progress_ctx_t*)clientp;
    double pc = 0;
    pc = (dlnow*100.0)/dltotal;
    pctx->ytd->setProgress(pc);

    return 0;
}

int cpc_init(curl_progress_ctx_t* pctx, YTDownload* ytd)
{
    pctx->ytd = ytd;
    pctx->pc = 0.0;
}

void cpc_reset(curl_progress_ctx_t* pctx)
{
    pctx->ytd->mprogress->lock();
    pctx->pc = 0.0;
    pctx->ytd->mprogress->unlock();
}


#include "bench.h"

#ifdef TEST_LEVEL
void test_level()
{
    dy::log::set_sync_log_level(dy::log::SYNC_LOG_LEVEL_INFO);

    dy::common::LogSyncConfig &config = dy::common::LogSyncConfig::get_mutable_instance();
    config.set_server_id(280001);
    config.set_server_name("token");

    test_stt_warn_log("warn",60);
    test_stt_info_log("info",60);

    Text_Log_Cache *pc = Text_Log_Cache::get_cache_ins();
    pc->flash_cache();
}
#endif

int main(int argc, char *argv[])
{
#ifdef TEST_LEVEL
    test_level();
#endif

#ifndef TEST_LEVEL
    if(argc!=5){
        printf("usage:%s n-file(1-5) choice(1|2) timespan(60s|3600s|86400s) timeout(timer to stop.)\n",argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if(n<=1){
        n = 1;
    }else if(n>=5){
        n = 5;
    }

    Choice choice = atoi(argv[2]) == 1 ? FASTLOG:STTLOG;
    int timespan = atoi(argv[3]);
    int timeout = atoi(argv[4]);

    std::string basename = "test";

    WorkPool wp(n,basename,choice,timespan,timeout);

    wp.execute();

    printf("time elapsed:%lf\n",wp.elapsed());
#endif
    return 0;
}

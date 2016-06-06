#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <stdio.h>
#include <string>

#include <Common/DistTextLog.h>
#include <textlog/textlog.h>

#define FLUSH_LOG {\
    Text_Log_Cache *cache = Text_Log_Cache::get_cache_ins();\
    cache->flash_cache();\
}\

const char *text_log = "this is TEST log.";
const char *token = "token";
const int RUNTIME = 300;

void work_error()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            STT_ERROR_LOG(0,token,STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),"test:%s", text_log);
        }
    }catch(boost::thread_interrupted& ){
        FLUSH_LOG
    }
}

void work_warn()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            STT_WARN_LOG(token,STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),"test:%s", text_log);
        }
    }catch(boost::thread_interrupted& ){
        FLUSH_LOG

    }
}

void work_info()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            STT_INFO_LOG(token,STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),"test:%s", text_log);
        }
    }catch(boost::thread_interrupted& ){
        FLUSH_LOG

    }
}

void work_debug()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            STT_DEBUG_LOG(token,STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),"test:%s", text_log);
        }
    }catch(boost::thread_interrupted& ){
        FLUSH_LOG

    }
}

void work_flow()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            STT_FLOW_LOG(token,STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),"test:%s", text_log);
        }
    }catch(boost::thread_interrupted& ){
        FLUSH_LOG

    }
}


int main(int argc, char *argv[])
{
    if(argc!=2){
        printf("usage:%s runtime.\n",argv[0]);
        exit(1);
    }
    int runtime = atoi(argv[1]);

    dy::common::LogSyncConfig &config = dy::common::LogSyncConfig::get_mutable_instance();
    config.set_server_id(280001);
    config.set_server_name("token");

    dy::log::set_sync_log_level(dy::log::SYNC_LOG_LEVEL_DEBUG);


    boost::thread_group pool;
    pool.create_thread(boost::bind(work_error));
    pool.create_thread(boost::bind(work_error));
    pool.create_thread(boost::bind(work_error));
    pool.create_thread(boost::bind(work_error));
    pool.create_thread(boost::bind(work_error));


    boost::this_thread::sleep(boost::posix_time::seconds(runtime));
    pool.interrupt_all();

    //pool.join_all();

    FLUSH_LOG

    return 0;
}

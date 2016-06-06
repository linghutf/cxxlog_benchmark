#ifndef BENCH__H__
#define BENCH__H__

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/timer.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <sstream>

#include <Common/DistTextLog.h>
#include <textlog/textlog.h>

const char *text_log = "this is ERROR log.";

const int LEVEL_COUNT = 5;

void test_tl_fast_log(const std::string& filename,int t)
{

    TextLogSplitTime type;

    switch(t){
        case 60:type = tlstMinute;
                break;
        case 3600:type = tlstHour;
                  break;
        case 86400:type = tlstDay;
                   break;
        default:
                   printf("ERROR timespan!\n");
    }

    TL_FAST_LOG(filename.c_str(),type, "test:%s count:%d uid:%d",text_log, 3721, 1234);
}

//t --- no use
void test_stt_error_log(const std::string& filename,int t)
{
    //printf("%s\n",filename.c_str());
    STT_ERROR_LOG(0, filename.c_str(), STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),
            "test:%s", text_log);
}

void test_stt_debug_log(const std::string& filename,int t)
{
    //printf("%s\n",filename.c_str());
    STT_DEBUG_LOG(filename.c_str(), STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),
            "test:%s", text_log);
}

void test_stt_info_log(const std::string& filename,int t)
{
    //printf("%s\n",filename.c_str());
    STT_INFO_LOG(filename.c_str(), STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),
            "test:%s", text_log);
}

void test_stt_warn_log(const std::string& filename,int t)
{
    //printf("%s\n",filename.c_str());
    STT_WARN_LOG(filename.c_str(), STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),
            "test:%s", text_log);
}

void test_stt_flow_log(const std::string& filename,int t)
{
    //printf("%s\n",filename.c_str());
    STT_FLOW_LOG(filename.c_str(), STT_ADD_KEY_LOG("count", 3721 , "uid", 1234),
            "test:%s", text_log);
}

enum Choice{
    FASTLOG = 1,
    STTLOG = 2
};

enum LogLevel{
    L_None = 0,
    L_FLOW = 1,
    L_DEBUG = 5,
    L_INFO = 4,
    L_WARN = 3,
    L_ERROR = 2
};

class Work:public boost::enable_shared_from_this<Work>{
    public:
        // type is top level value
        Work(const std::string& filename,Choice choice,LogLevel highlevel,int timespan,int timeout):
            choice_(choice),filename_(filename),timespan_(timespan),timeout_(timeout),is_flushed_(false){

                switch(choice){
                    case FASTLOG:
                        funcs_.push_back(test_tl_fast_log);

                        filename_ = "./log/"+filename_+".log";
                        break;

                    case STTLOG:
                        //default level
                        funcs_.push_back(test_stt_flow_log);

                        switch(highlevel){
                            case L_DEBUG:
                                funcs_.push_back(test_stt_debug_log);
                                //continue add
                            case L_INFO:
                                funcs_.push_back(test_stt_info_log);
                            case L_WARN:
                                funcs_.push_back(test_stt_warn_log);
                            case L_ERROR:
                                funcs_.push_back(test_stt_error_log);
                        }
                }
                size_ = funcs_.size();
            }
/*
        virtual ~Work()
        {
            Text_Log_Cache *cache = Text_Log_Cache::get_cache_ins();
            cache->flash_cache();
        }
*/
        void run()
        {
            if(choice_==FASTLOG){
                boost::thread t(boost::bind(&Work::writelog,shared_from_this(),0));
                //no change filename
                boost::this_thread::sleep(boost::posix_time::seconds(timeout_));
                t.interrupt();
            }else if(choice_=STTLOG){
                boost::thread_group pool;
                for(int i=0;i<size_;++i){
                    pool.create_thread(boost::bind(&Work::writelog,shared_from_this(),i));
                }

                boost::this_thread::sleep(boost::posix_time::seconds(timeout_));
                pool.interrupt_all();

            }
        }


    private:
        Choice choice_;

        std::string filename_;
        int timespan_;
        int timeout_;

        bool is_flushed_;

        std::vector<boost::function<void(const std::string&,int)> > funcs_;
        int size_;

        void writelog(int idx)
        {
            try{

                while(true){
                    boost::this_thread::interruption_point();
                    //write by flow way
                    funcs_[idx](filename_,timespan_);
                }
            }catch(...){

                if(!is_flushed_){
                    is_flushed_ = true;

                    Text_Log_Cache *cache = Text_Log_Cache::get_cache_ins();
                    cache->flash_cache();
                }
            }
        }

        //stt_log serial write
        //not use
        void stt_writelog()
        {
            try{
                while(true){
                    boost::this_thread::interruption_point();
                    //write by flow way
                    for(int i=0;i<size_;++i){
                        funcs_[i](filename_,timespan_);
                    }
                }
            }catch(...){
                if(!is_flushed_){
                    is_flushed_ = true;

                    Text_Log_Cache *cache = Text_Log_Cache::get_cache_ins();
                    cache->flash_cache();
                }
            }
        }

};

class WorkPool{
    public:
        WorkPool(int n,const std::string& basename,Choice c,int timespan,int timeout):
            timespan_(timespan),timeout_(timeout),elapsed_(0){

                dy::common::LogSyncConfig &config = dy::common::LogSyncConfig::get_mutable_instance();
                config.set_server_id(280001);
                config.set_server_name("token");


                n_ = n<LEVEL_COUNT?n:LEVEL_COUNT;
                //dy::log::open_flow_log_switch();

                switch(n_){
                    case 2:
                        dy::log::set_sync_log_level(dy::log::SYNC_LOG_LEVEL_ERROR);
                        break;
                    case 3:
                        dy::log::set_sync_log_level(dy::log::SYNC_LOG_LEVEL_WARN);
                        break;
                    case 4:
                        dy::log::set_sync_log_level(dy::log::SYNC_LOG_LEVEL_INFO);
                        break;
                    case 5:
                        dy::log::set_sync_log_level(dy::log::SYNC_LOG_LEVEL_DEBUG);
                        break;
                }

                std::stringstream ss;
                switch(c){
                    case FASTLOG:
                        for(int i=0;i<n_;++i){
                            std::string tmp;
                            ss<<i;
                            ss>>tmp;
                            ss.clear();
                            executions_.push_back(
                                    boost::make_shared<Work>(
                                        Work(basename+tmp,c,L_None,timespan,timeout))
                                    );
                        }
                        break;

                    case STTLOG:

                        executions_.push_back(
                                boost::make_shared<Work>(
                                    Work(basename,c,(LogLevel)(n_),timespan,timeout))
                                );
                        /*
                        for(int i=0;i<n_;++i){
                            executions_.push_back(
                                    boost::make_shared<Work>(
                                        Work(basename,c,(LogLevel)(i+1),timespan,timeout))
                                    );
                        }*/
                        break;
                }
            }

        virtual ~WorkPool()
        {
            executions_.clear();

        }

        void execute()
        {
            boost::timer t;
            for(int i=0;i<executions_.size();++i){
                pool_.create_thread(boost::bind(
                            &Work::run,executions_[i]));
            }
            pool_.join_all();
            elapsed_ = t.elapsed();
        }

        double elapsed()
        {
            return elapsed_;
        }

    private:
        int n_;
        int timespan_;
        int timeout_;
        double elapsed_;

        std::vector<boost::shared_ptr<Work> > executions_;
        boost::thread_group pool_;
};
#endif

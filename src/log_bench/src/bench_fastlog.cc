#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <stdio.h>
#include <string>

#include <Common/DistTextLog.h>
#include <textlog/textlog.h>

const char *text_log = "this is TEST log.";
const int N = 5;
const int RUNTIME = 300;

void work(const std::string& filename)
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            TL_FAST_LOG(filename.c_str(),tlstNoSplit,"count:%d uid:%d test:%s",3721,1234,text_log);
        }
    }catch(boost::thread_interrupted& ){

        Text_Log_Cache *cache = Text_Log_Cache::get_cache_ins();
        cache->flash_cache();
    }
}

int main(int argc, char *argv[])
{
    if(argc!=2){
        printf("usage:%s runtime.\n",argv[0]);
        exit(1);
    }
    int runtime = atoi(argv[1]);

    std::string filenameArr[]={
        "./log/test-1.log",
        "./log/test-2.log",
        "./log/test-3.log",
        "./log/test-4.log",
        "./log/test-5.log"
    };

    boost::thread_group pool;
    for(int i=0;i<N;++i){
        pool.create_thread(boost::bind(work,filenameArr[i]));
    }

    boost::this_thread::sleep(boost::posix_time::seconds(runtime));
    pool.interrupt_all();


    return 0;
}

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <stdio.h>
#include <string>

#include <glog/logging.h>

const char *text_log = "this is TEST log.";
const int RUNTIME = 300;

void work_error()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            LOG(ERROR)<<text_log;
        }
    }catch(...){

    }
}

void work_warn()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            LOG(WARNING)<<text_log;
        }
    }catch(...){

    }
}

void work_info()
{
    try{
        boost::this_thread::interruption_point();
        while(true){
            LOG(INFO)<<text_log;
        }
    }catch(boost::thread_interrupted& ){

    }
}

int main(int argc, char *argv[])
{
    if(argc!=2){
        printf("usage:%s runtime.\n",argv[0]);
        exit(1);
    }
    int runtime = atoi(argv[1]);

    google::InitGoogleLogging(argv[0]);
    google::SetLogFilenameExtension(".log");
    google::SetLogDestination(google::INFO,"./glog/info");
    google::SetLogDestination(google::WARNING,"./glog/warn");
    google::SetLogDestination(google::ERROR,"./glog/error");
    google::SetStderrLogging(google::FATAL);

    boost::thread_group pool;

    pool.create_thread(boost::bind(work_info));
    pool.create_thread(boost::bind(work_info));
    pool.create_thread(boost::bind(work_info));

    boost::this_thread::sleep(boost::posix_time::seconds(runtime));
    pool.interrupt_all();


    return 0;
}

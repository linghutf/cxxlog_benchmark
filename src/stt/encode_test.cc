#include <stdio.h>
#include <string>
#include <boost/bind.hpp>

#include "SttEncoding.h"
#include "Unicode.h"

class Message{
    public:
        Message():
            uid_(-1),posttime_(""),msg_(""){}

        Message(int uid,const char *posttime,const char *msg):
            uid_(uid),posttime_(posttime),msg_(msg){}

    public:
        int uid(){return uid_;}
        std::string post_time(){return posttime_;}
        std::string message(){return msg_;}

        void setUid(int uid){uid_ = uid;}
        void setPostTime(const char *tstr)
        {
            posttime_.clear();
            //posttime_ = std::sring(tstr);
            int len = strlen(tstr);
            posttime_.resize(len);
            for(int i=0;i<len;++i){
                posttime_[i]=tstr[i];
            }
        }
        void setMessage(const char *msg)
        {
            msg_.clear();
            int len = strlen(msg);
            msg_.resize(len);
            for(int i=0;i<len;++i){
                msg_[i]=msg[i];
            }
        }

    private:
        int uid_;
        std::string posttime_;
        std::string msg_;
};

void test_encode(const char *filename);
void test_decode(const char *filename);


int main(int argc, char *argv[])
{
    //auto GetResualt = boost::bind(&SttEncoder::GetResualt,_1);

    char filename[] = "data";

    test_encode(filename);

    test_decode(filename);


    return 0;
}

void test_encode(const char *filename)
{
    SttEncoder encode;
    const int nums = 3;

    SttEncoder enc[nums];
    SttString person[nums];

    Message msg;

    char post_times[3][30]={
        "A2016-05-18 14:00:00S",
        "@A@A2016-05-18 15:00:00@S@S",
        "@S2016-05-18 16:00:00@A",
    };

    char messages[3][50]={
        "@第一条弹幕@",
        "/First popup message.\\",
        "hello world!"
    };

    for(int i=0;i<nums;++i){

        msg.setUid(i);
        msg.setPostTime(post_times[i]);
        msg.setMessage(messages[i]);

        //printf("DEBUG:%d,%s,%s\n",msg.uid(),msg.post_time().c_str(),msg.message().c_str());

        enc[i].AddItem("uid",msg.uid());
        enc[i].AddItem("post_time",msg.post_time().c_str());
        enc[i].AddItem("message",msg.message().c_str());

        enc[i].GetResualt(person+i);
    }

    encode.AddItem("tid-1",person[0].c_UTF8());
    encode.AddItem("tid-2",person[1].c_UTF8());
    encode.AddItem("tid-3",person[2].c_UTF8());

    SttString final_result;
    encode.GetResualt(&final_result);

    const Utf8Char *p = final_result.c_UTF8();
    int len = get_utf8_length(p);

    printf("encode:\n%s\n",p);

    FILE *fp = fopen(filename,"wb");
    assert(fp!=NULL);

    const char *q = final_result.c_Local();
    int write_len = strlen(q);

    //长度不:同
    //printf("DEBUG:len(utf-8):%d,len(local):%d\n",len,write_len);
    //写入UTF-8会有长度不准确的问题
    size_t n = fwrite(q,sizeof(char),write_len,fp);
    assert(n==write_len);

    fclose(fp);
}

void test_decode(const char *filename)
{
    printf("DEBUG decode:\n");

    FILE *fp = fopen(filename,"rb");
    assert(fp!=NULL);

    char buf[BUFSIZ];

    size_t n = fread(buf,sizeof(Utf8Char),BUFSIZ,fp);

    fclose(fp);

    SttDecoder decoder;
    SttEncondingItem item;

    int ret = 0,nums = 0,fields = 0;

    int count = decoder.Parse(buf);
    if(0==count){
        fprintf(stderr,"parse failed.\n");
        return;
    }

    for(int i=0;i<count;++i){
        ret = decoder.GetDecoderItemPair(i,item);

        if(ret == 0){
            //printf("DEBUG [item]:%s\n",item.Key->c_UTF8());
            if(item.Value == NULL)
            {
                continue;
            }
            printf("DEBUG [value]%s\n",item.Value->c_UTF8());

            //需要新的decoder,不然之前Parse的内容丢失
            SttDecoder dec;

            nums = dec.Parse(item.Value->c_UTF8());

            //printf("DEBUG:%s\n",decoder.GetItem(i)->c_UTF8());
            if(3!=nums){
                fprintf(stderr,"field number ERROR!\n");
                continue;
            }

            for(int j=0;j<nums;++j){
                fields = dec.GetDecoderItemPair(j,item);

                if(0 == fields){
                    if(item.Value == NULL){
                        continue;
                    }

                    printf("%s:%s\n",item.Key->c_UTF8(),item.Value->c_UTF8());
                }
            }
        }
        printf("\n\n");


    }
}

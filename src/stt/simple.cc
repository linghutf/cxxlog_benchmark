/**
 *
 * 前一次的编码结果作为下一次的编码source,再次发生替换
 *
 *
 */
#include <stdio.h>

#include "SttEncoding.h"


int main(int argc, char *argv[])
{
    //first
    SttEncoder enc;

    enc.AddItem("@S","@S");

    SttString str;

    enc.GetResualt(&str);//因为有/,替换成了@S

    printf("[frst]%s\n",str.c_UTF8());

    //twice
    SttEncoder two_enc;

    two_enc.AddItem("@S",str.c_UTF8());

    SttString two_str;
    two_enc.GetResualt(&two_str);
    printf("[twic]%s\n",two_str.c_UTF8());

    //third
    SttEncoder tri_enc;

    tri_enc.AddItem("@S",two_str.c_UTF8());

    SttString tri_str;
    tri_enc.GetResualt(&tri_str);
    printf("[thir]%s\n",tri_str.c_UTF8());

    //fourth
    SttEncoder four_enc;

    four_enc.AddItem("@S",tri_str.c_UTF8());

    SttString four_str;
    four_enc.GetResualt(&four_str);
    printf("[four]%s\n",four_str.c_UTF8());



    SttDecoder four_dec;
    int c = four_dec.Parse(four_str.c_UTF8());
    assert(c!=0);
    SttEncondingItem item;
    four_dec.GetDecoderItemPair(0,item);

    printf("[four]%s:%s\n",item.Key->c_UTF8(),item.Value->c_UTF8());//third ecnode result
    return 0;
}

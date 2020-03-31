#include <iconv.h>
#include <convert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <locale.h>
#include <stdlib.h>

bool Utf82gbk(std::string &gbkStr, std::string &srcStr)
{

    //????utf-8?????unicode??
    if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//?????unicode???,???utf8??
    {
        printf("Bad Parameter\n");
        return false;
    }

    int unicodeLen=mbstowcs(NULL,srcStr.c_str(),0);//????????
    if(unicodeLen<=0)
    {
        printf("Can not Transfer!!!\n");
        return false;
    }
    wchar_t *unicodeStr=(wchar_t *)calloc(sizeof(wchar_t),unicodeLen+1);
    mbstowcs(unicodeStr,srcStr.c_str(),srcStr.size());//?gbk???unicode

    //?unicode?????gbk??
    if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//??unicode?????,???gbk
    {
        printf("Bad Parameter\n");
        return false;
    }
    int gbkLen=wcstombs(NULL,unicodeStr,0);//????????
    if(gbkLen<=0)
    {
        printf("Can not Transfer!!!\n");
        return false;
    }
    char gbkbuf[gbkLen];
    wcstombs(gbkbuf,unicodeStr,gbkLen);
    gbkbuf[gbkLen]=0;//?????
    gbkStr = gbkbuf;
    free(unicodeStr);
    return true;
}

int utf8togb2312(char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen) {

iconv_t cd;

if( (cd = iconv_open("gb2312","utf-8")) ==0 )

      return -1;

memset(destbuf,0,destlen);
char **source = &sourcebuf;

char **dest = &destbuf;
if(-1 == iconv(cd,source,&sourcelen,dest,&destlen))

     return -1;

iconv_close(cd);

return 0;

}

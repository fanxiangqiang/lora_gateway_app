#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cJSON.h"
#include "stddef.h"
#include <time.h>
#include "curl.h"
#include "curlbuild.h"
#include "curlrules.h"
#include "curlver.h"
#include "easy.h"
#include <string.h>


struct MemoryStruct 
{
    char *memory;
    size_t size;
};

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{ //一次返回可能对应多次调用，此处用于拼接完整版的返回信息
    size_t realsize = size * nmemb;
    /*   struct MemoryStruct *mem = (struct MemoryStruct *)data;
     mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
     if (mem->memory)
     {
          memcpy(&(mem->memory[mem->size]), ptr, realsize);
          mem->size += realsize;
          mem->memory[mem->size] = 0;
     }*/
      printf("recive data \r\n%s\r\n",ptr);
     return realsize;
}


int main(void)
{

     time_t timep;
     time(&timep);

     cJSON *root;
     cJSON *rfid_dev = cJSON_CreateObject();

     root = cJSON_CreateObject();

     cJSON_AddItemToObject(root, "rfid_dev", rfid_dev);
     cJSON_AddNumberToObject(rfid_dev, "node_device_number", 1);
     cJSON_AddStringToObject(rfid_dev, "node_device_mac", "123");
     cJSON_AddNumberToObject(rfid_dev, "number", 1);
     cJSON_AddStringToObject(rfid_dev, "rec_time", asctime(gmtime(&timep)));
     cJSON_AddStringToObject(rfid_dev, "position", "position");
     char *out = cJSON_Print(root);
     printf("%s\n", out);

     CURL *curl;
     curl = curl_easy_init();
     if (!curl)
     {
          printf("curl init failed \r\n");
          return -1;
     }
#define POSTURL "192.168.1.237:8090"

     curl_easy_setopt(curl, CURLOPT_URL, POSTURL);      //设置post address
     curl_easy_setopt(curl, CURLOPT_POST,1);           ///使用post
     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); //不验证证书以及hosts
     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
     curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3L);          ///设置超时
     curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);   //重连超时

     curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);        //启用时会汇报所有的信息
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);  //得到请求结果后的回调函数

     struct MemoryStruct oDataChunk; //请求结果的保存格式
     oDataChunk.memory = (char *)malloc(1);
     oDataChunk.size = 0;
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &oDataChunk);

     curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向 //告诉libcurl在输出请求体时包含头部信息 
     curl_easy_setopt(curl, CURLOPT_HEADER, 0L); //关闭中断信号响应，如果是多线程，请将该参数置为1。这个选项用于unix环境下的多线程应用仍然可以使用各种timeout选项，而不会引发信号中断致使程序退出。 
     curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L); 

       struct curl_slist *pList = NULL;

       pList = curl_slist_append(pList, "User-Agent: libcurl_toone_taingongyun__used_by_fanxiangqiang_lora_gateway");
       pList = curl_slist_append(pList, "Accept-language:  en-US,en;q=0.5");
       pList = curl_slist_append(pList, "Content-Type: application/json");
     pList = curl_slist_append(pList, "Connection: close");
      

       unsigned char temp[64] = {0};
       char *p = cJSON_Print(root);
       sprintf(temp, "Content-Length: %d\r\n", strlen(p));
       pList = curl_slist_append(pList, temp);
      // pList = curl_slist_append(pList, "\r\n");
       curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pList);


       pList = curl_slist_append(pList, p);
       /// curl_easy_setopt(curl, CURLOPT_HTTPHEADER,array("expect:"));///此处问题一直没解决，expect :100 array 找不到头文件
       cJSON_Delete(root);

       //   while (pList != NULL)
       //   {
       //        printf("http data is  \r\n%s\r\n", pList->data);
       //      //  pList->data = pList->next;
       //       // pList->next++;
       //       pList++;
       //   }
       //   printf("http data is  \r\n%s\r\n",pList->data);
       //   printf("http data is  \r\n%s\r\n",pList->data);
       //   printf("http data is  \r\n%s\r\n",pList->data);
       //   printf("http data is  \r\n%s\r\n",pList->data);
       //   while(1);

       //用于执行CURL对象，是一个阻塞类型的操作
       char res = -1;
       res = curl_easy_perform(curl); //执行请求
       if (res != CURLE_OK)
       {
            //curl_easy_perform执行失败
            printf("curl_easy_perform error,err_msg:[%ld]\n", res);
       }
       printf("curl_easy_perform ok  %d\r\n",res);
       
       long res_code = -1;
       res = -1;
       res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
      // while(1);
       //正确响应后，请求转写成本地文件的文件
       int ret_flag = -1;
       if (res == CURLE_OK)
       {
            if (res_code == 200 || res_code == 201)
            {
                // printf(oDataChunk.memory);
                printf("result code is %d\r\n",res_code);
            }
       }

       free(oDataChunk.memory); 
       //free(full_url); 
       curl_slist_free_all(pList); 
       curl_easy_cleanup(curl);
        curl_global_cleanup(); 
        //return ret_flag;


       printf("test \r\n");
       while (1)
       {
            printf("---->>test code \r\n");
            sleep(1);
     }

     return 0;
}

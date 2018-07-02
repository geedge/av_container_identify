#include "vdefine.h"
#define SIZE_BUFF 16*16*16*16*16*16*16*16-1//6*16*16*16*16*16
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct buff_inf_h
{
	char *buff;
	size_t size;
}buff_inf;

buff_inf *buff_get(char *vfile) {
	buff_inf *buff_inf_h = (buff_inf *)malloc(sizeof(buff_inf));

	FILE *fr = NULL;
	char *buff_ori = (char *)malloc(sizeof(char *)*SIZE_BUFF);//CH
	char *buff = buff_ori;//CH
    memset(buff, 0, sizeof(buff));
	if ((fr = fopen(vfile, "rb")) == NULL)
	{
		perror("fopen error:");
		buff_inf_h->buff = NULL;
		buff_inf_h->size = 0;
		return buff_inf_h;//-1;
	}
	size_t i = 0;//unsigned int i = 0;//size_t i = 0;
	//int j = 0;
	while (!feof(fr))
	{
		fread(buff, 1, 1, fr);
		buff += 1;
		i++;
	}
	buff_inf_h->buff = (char *)malloc(sizeof(char *) * (i-2));
    memcpy(buff_inf_h->buff, buff - i, i-2);
    buff_inf_h->size = i - 2;
    //buff_inf_h.buff = buff - i;
	//buff_inf_h.size = i;
	free(buff_ori);//CH--free(buff - i);//?
    buff_ori = NULL;
    fclose(fr);
	return buff_inf_h;

}
int main(int argc, char *argv[])
{
    DIR *dirp;
    struct dirent *dp;
    char *vdir = argv[1];
    char *vfile_ori = (char *)malloc(30);
    memcpy(vfile_ori,vdir,strlen(vdir));
    strcat(vfile_ori, "/");
    char *vfile = (char *)malloc(30);
    //strcat(vdir,".dir");
    dirp = opendir(vdir);
    struct stat info;
    
    buff_inf *buff_inf_h;
    //CH--char *buff;// = (char *)malloc(sizeof(char *)*size);//;
    int ret;
    int ret_sig = atoi(argv[2]);
    int sig_show_all = atoi(argv[3]);
    int sig_choose_all = atoi(argv[4]);
    size_t size;
    while ((dp = readdir(dirp)) != NULL) 
    {
            memcpy(vfile,vfile_ori,30);
            strncat(vfile, dp->d_name, strlen(dp->d_name));//strcat(vfile,dp->d_name);//vfile = dp->d_name;//fileinfo->name;//&((fileinfo->name)[0]);//argv[1];// "0003.dat";
            
            stat(vfile, &info);
            if(S_ISDIR(info.st_mode))
                continue;
            buff_inf_h = buff_get(vfile);
            if (buff_inf_h->size == 0)
            {
                    printf("read file failed.\n");
                    return -1;
            }
            if (sig_choose_all == 0)
            {
                    int divide_h = 10;
                    int divide_s = 0;
                    int divide_e = 1;

                    size = (buff_inf_h->size / divide_h)*(divide_e - divide_s);

                    //CH--buff = (char *)malloc(sizeof(char *)*size);
                    //CH--memcpy(buff, buff_inf_h->buff + (buff_inf_h->size / divide_h)*divide_s, size);

                    //printf("buff_whole is               %p.\n", buff_inf_h->buff);
                    //printf("buff_inf_h.size/divide_h is %x.\n", buff_inf_h->size / divide_h);
                    //printf("buff starts from(>=)        %p.\n", buff);
                    //printf("buff ends at(<)             %p.\n", buff + size - 1);//+size
                    ret = AV_container_identify(buff_inf_h->buff + (buff_inf_h->size / divide_h)*divide_s, size);;//CCH--ret = AV_container_identify(buff, size);//CH--(const char *)?
                    //CH--free(buff);
                    //CH--buff = NULL;
                    //printf("define : %d.\n", ret);
                    //printf("size of whole buff is  %d.\n", buff_inf_h->size);//size - 1
                    //printf("size of chosen buff is %d.\n", size);
            }
            else
                    ret = AV_container_identify(buff_inf_h->buff, buff_inf_h->size);
            if (sig_show_all == 0)
            {
                    if (ret != ret_sig)
                    {
                            printf("-----------------data of [%s] is kind of [%d].\n", dp->d_name, ret);
                            //printf("define : %d.\n", ret);
                            printf("size of whole buff is %d.\n", buff_inf_h->size);
                            if  (sig_choose_all == 0)
                                printf("size of chosen buff is %d.\n", size);

                    }
            }
            else
            {
                    printf("-----------------data of [%s] is kind of [%d].\n", dp->d_name, ret);
                    printf("size of whole buff is %d.\n", buff_inf_h->size);
                    if  (sig_choose_all == 0)
                            printf("size of chosen buff is %d.\n", size);
            }
            free(buff_inf_h->buff);//CH--
            buff_inf_h->buff = NULL;//CH--
            free(buff_inf_h);//CH--
            buff_inf_h = NULL;//CH--
    }
    /*
    free(buff);

    free(buff_inf_h->buff);
    buff_inf_h = NULL;
    free(buff_inf_h);
    return ret;
    */
   
    //CH--free(buff_inf_h->buff);
    //CH--buff_inf_h = NULL;
    //CH--free(buff_inf_h);

    free(vfile);
    free(vfile_ori);
    
    return ret;
}

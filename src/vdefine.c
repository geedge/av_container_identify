#include "vdefine.h"

#define AV_CONTAINER_MPEGTS	1
#define AV_CONTAINER_FLV	2
#define AV_CONTAINER_MP4	3
#define UNKNOWN 4
#define FIT_TIMES 2
#define MAX_FLV_FRAME max_flv_frame;//?
int xtoint(const char* buff, size_t size)
{
	int size_h = 0;
	int i;
    unsigned char *add=(unsigned char *)malloc(sizeof(unsigned char *));
	for (i = 0; i < size; i++)
	{
        memcpy(add,(buff+i),1);
		size_h +=((*add) << (8 * (size - i - 1)));
	}
    free(add);
	add = NULL;
    return size_h;

}
int is_mpeg_ts_each(const char *buff, size_t size)
{
	const char *p = buff;
	int fit_times = 0;
	p += 188;

    if ((buff+3)>=(buff+size))
        return 0;
    int c_count_ori = xtoint(buff + 3, 1);
    c_count_ori = c_count_ori % 16;
	int c_count_aft;
	while (p < (buff + size))//=?
	{
		if (*p == 0x47)
		{
            if ((p+3) < (buff+size))
            {
                c_count_aft = xtoint(p + 3, 1);
                c_count_aft = c_count_aft % 16;
                c_count_aft = (c_count_aft == 0 ? 16 : c_count_aft);
                if (c_count_aft == c_count_ori + 1)
                    fit_times++;
                else
                    return 0;

            }
            if (fit_times > FIT_TIMES)
                return 1;
        }
		else
			return 0;
		
        c_count_ori = c_count_aft;
        p += 188;
	}
	return 0;
}

int is_mpeg_ts(const char* buff, size_t size)
{
	const char*p = buff;
	const char *p_each = buff;
	size_t size_move = 0;
    int ret; 
	while (size_move < size)
	{
		p_each = memchr(p_each, 0x47, size - size_move);
		if (p_each == NULL)
			return 0;
		else
		{
            ret = is_mpeg_ts_each(p_each,size-(p_each-buff));
            if (ret == 1)
			{
				return 1;
			}
			p_each++;
			size_move = p_each - buff;
		}
	}
	return 0;
}
void * my_memchr_flv(const char *buff, size_t size, int count_type)//void * my_memchr_flv(const void * buff, size_t size, int count_type)
{
    int sig;
    int i;
	while (size)
	{
		sig = 0;
		i = 0;
		for (i = 0; i < count_type; i++)
		{
				if (*buff != (const char)flv_tag_type_all[i])
				    sig++;
		}
		if (sig == count_type)
		{
			buff ++;
			size --;
		}
		else
			return (void *)buff;
	}
	return NULL;
}

int is_flv_each(const char* buff, size_t size)
{
	const char *p = buff;
	int fit_times = 1;
    unsigned int left;
    if ((p+3)>=(buff+size))
        return 0;
	int right = 11 + xtoint(p + 1, 3);
    p += (right + 4);

	while (p < (buff + size))
	{

        left = xtoint(p - 4, 4);
        if (right==left&&(*(p) == 0x08 || *(p) == 0x09 || *(p) == 0x12))
		{
			fit_times++;
			if (fit_times > FIT_TIMES)
			{
				return 1;
			}
		}
		else
			return 0;
        if ((p+3)>=(buff+size))
            return 0;
		right = 11 + xtoint(p + 1, 3);
		p += (right + 4);
	}
	return 0;
}

int is_flv(const char* buff, size_t size)
{
	const char*p = buff;
	const char *p_each = buff;
	size_t size_move = 0;

	while (size_move < size)
	{
		p_each = my_memchr_flv(p_each, size - size_move, 3);
		if (p_each==NULL)
			return 0;

		else
		{
			int ret = is_flv_each(p_each, size - (p_each - buff));
			if (ret == 1)
			{
				return 1;
			}
			p_each++;///
			size_move = p_each - buff;
		}
	}
	return 0;
}


void * my_memmem_mp4(const char * buf, size_t size, int count_type)
{
	int i;
	const char *ret;
    const char *ret_rec;//??
    for (i = 0; i < count_type; i++)
    {
        ret = (const char *)memmem(buf,size, mp4_box_type_all[i], strlen(mp4_box_type_all[i]));
        if (ret != NULL)
            break;
    }
    if (ret == NULL)
        return NULL;
	for (i = 0; i < count_type; i++)
	{
        ret_rec = ret;
        ret = (const char *)memmem(buf,size, mp4_box_type_all[i], strlen(mp4_box_type_all[i]));
        if (ret != NULL)
        {
            if (ret_rec > ret)
                return (void *)ret;
            else if (ret_rec < ret)
                return (void *)ret_rec;
            else
                continue;
        }
        else
            ret = ret_rec;
    }
	return (void *)ret;//return NULL;
}

int is_mp4_each_part(const char *buff, int count_type)
{
	int i;
	size_t size_type_h;
	for (i = 0; i < count_type; i++)
	{
		size_type_h = strlen(mp4_box_type_all[i]);
        if (memcmp(buff, mp4_box_type_all[i], size_type_h) == 0)
            return 1;
	}
	return 0;
}

int is_mp4_each(const char* buff, size_t size, const char *buff_s, int count_type)
{
	const char *p = buff;
	int fit_times = 1;
	
    unsigned int size_h;
    int ret;
	while (p < (buff + size))
	{
		p += 8;
		if (p < (buff + size))
		{
			if ((p+4)>=(buff+size))
                return 0;
            ret = is_mp4_each_part(p, count_type);
			if (ret == 1)
			{
				fit_times++;
				if (fit_times > FIT_TIMES)
				{
					return 1;
				}
			}				
			else
			{
				if ((p-8-4)<buff_s)
					return 0;
				else
				{
                    size_h = xtoint(p -8 - 4, 4);
                    p += (size_h-8);
                    if ((p+4)>=(buff+size))
                        return 0;
					ret = is_mp4_each_part(p, count_type);
					if (ret == 1)
					{
						fit_times++;
						if (fit_times > FIT_TIMES)
							return 1;
					}
					else
						return 0;///adding
				}
			}
		}
		else
			return 0;
	}
	return 0;
}

int is_mp4(const char* buff, size_t size)
{
	const char *p = buff;
	const char *p_each = buff;
	size_t size_move = 0;

	int count_type = sizeof(mp4_box_type_all) / sizeof(char *);
    int ret;
	while (size_move < size)
	{
		p_each = (const char *)my_memmem_mp4(p_each, size - size_move, count_type);
		if (p_each==NULL)
			return 0;
		else
		{			
			ret = is_mp4_each(p_each, size - (p_each - buff), buff,count_type);
			if (ret == 1)
			{
				return 1;
			}
			p_each++;
			size_move = p_each - buff;
		}
	}
	return 0;
}

int AV_container_identify(const char* buff, size_t size)
{
	if (1 == is_mpeg_ts(buff, size))
	{
		return AV_CONTAINER_MPEGTS;
	}
	if (1 == is_flv(buff, size))
	{
	    return AV_CONTAINER_FLV;
	}
	if (1 == is_mp4(buff, size))
	{
		return AV_CONTAINER_MP4;
	}
	return UNKNOWN;
}



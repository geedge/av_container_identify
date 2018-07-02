#ifndef _VDEFINE_H
#define _VDEFINE_H

#include <arpa/inet.h>
#include <stdlib.h>
#define _GNU_SOURCE
#define __USE_GNU
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>  

#include <sys/io.h>

const char *mp4_box_type_all[] = { "ftyp", "pdin", "moov", "mvhd", "trak", "tkhd", "tref", "edts", "elst", "mdia", "mdhd",
"hdlr", "minf", "vmhd", "smhd", "hmhd", "nmhd", "dinf", "dref", "stbl", "stsd", "stts", "ctts", "stsc", "stsz",
"stz2", "stco", "co64", "stss", "stsh", "padb", "stdp", "sdtp", "sbgp", "sgpd", "subs", "mvex", "mehd", "trex",
"ipmc", "moof", "mfhd", "traf", "tfhd", "trun", "sdtp", "sbgp", "subs", "mfra", "tfra", "mfro", "mdat", "free",
"skip", "udta", "cprt", "meta", "hdlr", "dinf", "dref", "ipmc", "iloc", "ipro", "sinf", "frma", "imif", "schm",
"schi", "iinf", "xml", "bxml", "pitm", "fiin", "paen", "fpar", "fecr", "segr", "gitn", "tsel", "meco", "mere" };
const int flv_tag_type_all[] = { 0x08,0x09,0x12 };

int xtoint(const char* buff, size_t size);
int is_mpeg_ts_each(const char *buff, size_t size);
int is_mpeg_ts(const char* buff, size_t size);
void * my_memchr_flv(const char * buffer, size_t size, int count_set);//void * my_memchr_flv(const void * buffer, size_t size, int count_set);
int is_flv_each(const char* buff, size_t size);
int is_flv(const char* buff, size_t size);
void * my_memmem_mp4(const char * buf, size_t size, int count_type);
int is_mp4_each_part(const char *buff, int count_type);
int is_mp4_each(const char* buff, size_t size, const char *buff_s, int count_type);
int is_mp4(const char* buff, size_t size);
int AV_container_identify(const char* buff, size_t size);

#endif



#ifndef _FTP_H
#define _FTP_H
#include <stm32f10x.h>



typedef struct
{
	uint32_t hardware_version;
	uint32_t software_version;
	uint8_t hardware_name[50];
	uint8_t software_name[50];
	uint8_t corporation[50];
	uint8_t date[50];
} Pro_Info;

typedef struct
{
	uint8_t download_flag;
	uint8_t updata_flag;
} Ftp_Ini;




int picked_str(uint8_t *src_str, uint8_t *str1, uint8_t *str2, uint8_t *dst_str);
int picked_str2(uint8_t *src_str, uint8_t *str1, uint8_t *str2, uint8_t *dst_str);
uint8_t ftp_connect(void);
uint32_t ftp_size(uint8_t *cmd, uint8_t *file);
int ftp_read_data(uint8_t *data);
uint32_t ftp_get(uint8_t *cmd, uint8_t *file);
uint8_t ftp_download_app(void);
uint8_t ftp_download_ini(void);


#endif

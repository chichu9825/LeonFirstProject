#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

#pragma pack(push, 1)
struct log_LWEI_s {
       uint16_t frame_header;
       uint64_t gyro_data;
       uint64_t accel_data;
       uint64_t gyro_sub_data;
       uint64_t accel_sub_data;
       uint16_t baro;
       uint16_t temp_baro;
       uint8_t  __pad;
 };

struct log_string_t {
	char frame_header[32];
	char gyro_data[32];
	char accel_data[32];
	char gyro_sub_data[32];
	char accel_sub_data[32];
	char baro[32];
	char temp_baro[32];
} log_string;
#pragma pack(pop)

#define MAX_FILE_SIZE = 20*1024*1024;

int ret = 0;
FILE *input = NULL;
FILE *output = NULL;
char *output_name = "output.txt";

int main(int argc, char *argv[]) {
	if(argc < 2)
		return -1;

	char *file = argv[1];	

	input = fopen(file, "r");
	if(input == NULL) {
		printf("Input open failed\n");
		return -1;
	}

	int cur_pos = ftell(input);
	fseek(input, 0L, SEEK_END);
	int length = ftell(input);
	fseek(input, cur_pos, SEEK_SET);

	int data_count = length/sizeof(struct log_LWEI_s);
	length = length - length%sizeof(struct log_LWEI_s);

	char *buf = (char *)malloc(length);
	if(buf == NULL) {
		printf("No more memory\n");
		return -1;
	}

	ret = fread(buf, sizeof(struct log_LWEI_s), data_count, input);
	if(ret != data_count) {
		printf("Reading failed\n");
		close(input);
		return -1;
	}

	close(input);
	output = fopen(output_name, "w+");
	if(output == NULL) {
		printf("Output file create failed\n");
		close(output);
		return -1;
	}

	struct log_LWEI_s *temp = (struct log_LWEI_s *)buf;
	int i=0;
	for(i=0; i<data_count; i++) {
		memset(&log_string, '\0', sizeof(struct log_string_t));
		sprintf(log_string.frame_header, "%d", temp[i].frame_header);
		sprintf(log_string.gyro_data, "%ld", temp[i].gyro_data);
		sprintf(log_string.accel_data, "%ld", temp[i].accel_data);
		sprintf(log_string.gyro_sub_data, "%ld", temp[i].gyro_sub_data);
		sprintf(log_string.accel_sub_data, "%ld", temp[i].accel_sub_data);
		sprintf(log_string.baro, "%d", temp[i].baro);
		sprintf(log_string.temp_baro, "%d", temp[i].temp_baro);
		fprintf(output, "%s %s %s %s %s %s %s\n", log_string.frame_header, log_string.gyro_data, log_string.accel_data, log_string.gyro_sub_data, log_string.accel_sub_data, log_string.baro, log_string.temp_baro);
	}

	close(output);

	return 0;
}

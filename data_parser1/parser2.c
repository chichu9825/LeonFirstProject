#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

#pragma pack(push, 1)
struct log_LWEI_s {
       int16_t frame_header;
       int16_t gyro_data_x;
       int16_t gyro_data_y;
       int16_t gyro_data_z;
       int16_t gyro_temp;
       int16_t accel_data_x;
       int16_t accel_data_y;
       int16_t accel_data_z;
       int16_t accel_temp;
       int16_t gyro_sub_data_x;
       int16_t gyro_sub_data_y;
       int16_t gyro_sub_data_z;
       int16_t gyro_sub_temp;
       int16_t accel_sub_data_x;
       int16_t accel_sub_data_y;
       int16_t accel_sub_data_z;
       int16_t accel_sub_temp;
       uint32_t baro;
       uint16_t temp_baro;
       uint32_t time_stamp;
 };

struct log_string_t {
       char frame_header[32];
       char gyro_data_x[32];
       char gyro_data_y[32];
       char gyro_data_z[32];
       char gyro_temp[32];
       char accel_data_x[32];
       char accel_data_y[32];
       char accel_data_z[32];
       char accel_temp[32];
       char gyro_sub_data_x[32];
       char gyro_sub_data_y[32];
       char gyro_sub_data_z[32];
       char gyro_sub_temp[32];
       char accel_sub_data_x[32];
       char accel_sub_data_y[32];
       char accel_sub_data_z[32];
       char accel_sub_temp[32];
       char baro[32];
       char temp_baro[32];
       char time_stamp[32];
 } log_string;

#if 0
struct log_string_t {
	char frame_header[32];
	char gyro_data[32];
	char accel_data[32];
	char gyro_sub_data[32];
	char accel_sub_data[32];
	char baro[32];
	char temp_baro[32];
	char time_stamp[32];
} log_string;
#endif

#pragma pack(pop)

#define MAX_FILE_SIZE = 20*1024*1024;

int ret = 0;
FILE *input = NULL;
FILE *output = NULL;
char output_name[64] = "";

int main(int argc, char *argv[]) {
	if(argc < 2)
	{
		warnx("****************************");
		warnx("usage: log_parser <filename>");
		warnx("spicify a file to parse");
		warnx("****************************");
		return -1;
	}

	char *file = argv[1];	

	strcat(output_name,argv[1]);
	strcat(output_name,".csv");


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

	fprintf(output, "time_stamp:,"
					"frame_header,"
					"gyro_x,"
					"gyro_y,"
					"gyro_z,"
					"gyro_temp,"
					"accel_x,"
					"accel_y,"
					"accel_z,"
					"accel_temp,"
					/*
					"gyro_sub_x,"
					"gyro_sub_y,"
					"gyro_sub_z,"
					"gyro_sub_temp,"
					"accel_sub_x,"
					"accel_sub_y,"
					"accel_sub_z,"
					"accel_sub_temp,"
					*/
					"baro,"
					"temp_baro\n"
				);

	for(i=0; i<data_count; i++) {
		memset(&log_string, '\0', sizeof(struct log_string_t));
		sprintf(log_string.frame_header, "%08d", temp[i].frame_header);

		sprintf(log_string.gyro_data_x, "%08d", temp[i].gyro_data_x);
		sprintf(log_string.gyro_data_y, "%08d", temp[i].gyro_data_y);
		sprintf(log_string.gyro_data_z, "%08d", temp[i].gyro_data_z);
		sprintf(log_string.gyro_temp, "%08d", temp[i].gyro_temp);

		sprintf(log_string.accel_data_x, "%08d", temp[i].accel_data_x);
		sprintf(log_string.accel_data_y, "%08d", temp[i].accel_data_y);
		sprintf(log_string.accel_data_z, "%08d", temp[i].accel_data_z);
		sprintf(log_string.accel_temp, "%08d", temp[i].accel_temp);

		sprintf(log_string.gyro_sub_data_x, "%08d", temp[i].gyro_sub_data_x);
		sprintf(log_string.gyro_sub_data_y, "%08d", temp[i].gyro_sub_data_y);
		sprintf(log_string.gyro_sub_data_z, "%08d", temp[i].gyro_sub_data_z);
		sprintf(log_string.gyro_sub_temp, "%08d", temp[i].gyro_sub_temp);

		sprintf(log_string.accel_sub_data_x, "%08d", temp[i].accel_sub_data_x);
		sprintf(log_string.accel_sub_data_y, "%08d", temp[i].accel_sub_data_y);
		sprintf(log_string.accel_sub_data_z, "%08d", temp[i].accel_sub_data_z);
		sprintf(log_string.accel_sub_temp, "%08d", temp[i].accel_sub_temp);

		sprintf(log_string.baro, "%08d", temp[i].baro);
		sprintf(log_string.temp_baro, "%08d", temp[i].temp_baro);
		sprintf(log_string.time_stamp, "%08d", temp[i].time_stamp);
		//fprintf(output, "%s:,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
		  fprintf(output, "%s:,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
				log_string.time_stamp, 
				log_string.frame_header,
				log_string.gyro_data_x, 
				log_string.gyro_data_y, 
				log_string.gyro_data_z, 
				log_string.gyro_temp,
				log_string.accel_data_x, 
				log_string.accel_data_y, 
				log_string.accel_data_z, 
				log_string.accel_temp, 
				//log_string.gyro_sub_data_x, 
				//log_string.gyro_sub_data_y, 
				//log_string.gyro_sub_data_z, 
				//log_string.gyro_sub_temp,
				//log_string.accel_sub_data_x, 
				//log_string.accel_sub_data_y, 
				//log_string.accel_sub_data_z, 
				//log_string.accel_sub_temp,
			    log_string.baro, 
			    log_string.temp_baro
				);
	}

	close(output);

	warnx("Now convert data file to output.txt");

	if(buf) {
		free(buf);
		buf = NULL;
	}

	return 0;
}

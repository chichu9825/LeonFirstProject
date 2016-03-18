#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

#pragma pack(push, 1)

struct pv_data_IMU_s {
	float acc_x_law;
	float acc_y_law;
	float acc_z_law;
	float acc_x_law_filter;
	float acc_y_law_filter;
	float acc_z_law_filter;
	// float gyro_x;
	// float gyro_y;
	// float gyro_z;
	// float mag_x;
	// float mag_y;
	// float mag_z;
	// float temp_acc;
	// float temp_gyro;
	// float temp_mag;
};
struct pv_data_IMU_str_s {
	char acc_x_law[32];
	char acc_y_law[32];
	char acc_z_law[32];
	char acc_x_law_filter[32];
	char acc_y_law_filter[32];
	char acc_z_law_filter[32];
	// float gyro_x;
	// float gyro_y;
	// float gyro_z;
	// float mag_x;
	// float mag_y;
	// float mag_z;
	// float temp_acc;
	// float temp_gyro;
	// float temp_mag;
};


#pragma pack(pop)

#define MAX_FILE_SIZE = 20*1024*1024;

int ret = 0;
FILE *input = NULL;
FILE *output = NULL;
char output_name[64] = "";

struct pv_data_IMU_str_s log_string;


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

	int data_count = length/sizeof(struct pv_data_IMU_s);
	length = length - length%sizeof(struct pv_data_IMU_s);

	char *buf = (char *)malloc(length);
	if(buf == NULL) {
		printf("No more memory\n");
		return -1;
	}

	ret = fread(buf, sizeof(struct pv_data_IMU_s), data_count, input);
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

	struct pv_data_IMU_s *temp = (struct pv_data_IMU_s *)buf;
	int i=0;


	fprintf(output, "acc_x_law,"
					"acc_y_law,"
					"acc_z_law,"
					"acc_x_law_filter,"
					"acc_y_law_filter,"
					"acc_z_law_filter,"
					/*
					"accel_x,"
					"accel_y,"
					"accel_z,"
					"accel_temp,"
					*/
					"\r\n"
				);

	for(i=0; i<data_count; i++) {
		memset(&log_string, '\0', sizeof(struct pv_data_IMU_str_s));

		sprintf(log_string.acc_x_law, "%8.8f", temp[i].acc_x_law);
		sprintf(log_string.acc_y_law, "%8.8f", temp[i].acc_y_law);
		sprintf(log_string.acc_z_law, "%8.8f", temp[i].acc_z_law);

		sprintf(log_string.acc_x_law_filter, "%8.8f", temp[i].acc_x_law_filter);
		sprintf(log_string.acc_y_law_filter, "%8.8f", temp[i].acc_y_law_filter);
		sprintf(log_string.acc_z_law_filter, "%8.8f", temp[i].acc_z_law_filter);


		  fprintf(output, "%s,%s,%s,%s,%s,%s\n", 
				log_string.acc_x_law,
				log_string.acc_y_law, 
				log_string.acc_z_law, 
				log_string.acc_x_law_filter, 
				log_string.acc_y_law_filter,
			    log_string.acc_z_law_filter
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

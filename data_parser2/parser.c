#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

#pragma pack(push, 1)

struct pv_data_IMU_s {
	uint64_t numCnt;
	uint64_t timestamp;
	int16_t accelerometer_raw[3];//[9];
	float accelerometer_m_s2[3];//[9];
};

struct pv_data_IMU_str_s {

	char numCnt[32];
	char timestamp[32];
	char accelerometer_raw[3][32];//[9];
	char accelerometer_m_s2[3][32];//[9];

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

	char numCnt[32];
	char timestamp[32];
	char accelerometer_raw[3][32];//[9];
	char accelerometer_m_s2[3][32];//[9];


	fprintf(output, "numCnt,"
					"timestamp,"
					"accelerometer_raw0,"
					"accelerometer_raw1,"
					"accelerometer_raw2,"
					"accelerometer_m_s2_0,"
					"accelerometer_m_s2_1,"
					"accelerometer_m_s2_1,"
					"\r\n"
				);

	for(i=0; i<data_count; i++) {
		memset(&log_string, '\0', sizeof(struct pv_data_IMU_str_s));

		sprintf(log_string.numCnt, "%lu", temp[i].numCnt);
		sprintf(log_string.timestamp, "%lu", temp[i].timestamp);
		sprintf(log_string.accelerometer_raw[0], "%d", temp[i].accelerometer_raw[0]);
		sprintf(log_string.accelerometer_raw[1], "%d", temp[i].accelerometer_raw[1]);
		sprintf(log_string.accelerometer_raw[2], "%d", temp[i].accelerometer_raw[2]);
		sprintf(log_string.accelerometer_m_s2[0], "%8.4f", temp[i].accelerometer_m_s2[0]);
		sprintf(log_string.accelerometer_m_s2[1], "%8.4f", temp[i].accelerometer_m_s2[1]);
		sprintf(log_string.accelerometer_m_s2[2], "%8.4f", temp[i].accelerometer_m_s2[2]);


		  fprintf(output, "%s,%s,%s,%s,%s,%s,%s,%s\n", 
				log_string.numCnt,
				log_string.timestamp,
				log_string.accelerometer_raw[0],
				log_string.accelerometer_raw[1],
				log_string.accelerometer_raw[2],
				log_string.accelerometer_m_s2[0],
				log_string.accelerometer_m_s2[1],
				log_string.accelerometer_m_s2[2]
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

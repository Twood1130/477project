//program to test the output of getDATA function

#include <stdio.h>
#include <stdlib.h>

void getDATA (char *NMEA, char *time, char *active, char *LAT, char *LONG, char *speed, char *date);

int main(void){
	
	char *string = '$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A';
	
	char *time, *active, *LAT, *LONG, *speed, *date;
	
	getDATA(string, time, active, LAT, LONG, speed, date);
	
	printf("time = %s \n active = %s \n Latitude = %s \n longitude = %s \n speed = %s \n date = %s", time, active, LAT, LONG, speed, date);
	
	
	return 0;
}

void getDATA (char *NMEA, char *time, char *active, char *LAT, char *LONG, char *speed, char *date) {

	char *start, *end;
	
	start = strchr(NMEA, ','); //Find the beginning field 1
	end = strchr(++start, ','); //Find the end of field 1
	*end = 0; //truncate the string
	*time = start;

	start = ++end; //start of next field 
	end = strchr(start, ',');
	*end = 0;
	*active = start;
	
	start = ++end; //start at the beginning of Latitude field
	end = strchr(start, ','); //find the next comma 
	end = strchr(++end, ','); //find next comma to include the cardinal direction 
	*end = 0; //zero terminate
	*LAT = start; //assign leftover string to variable. 
	
	start = ++end; //find the start of the Longitude field 
	end = strchr(start, ',');
	end = strchr(++end, ','); //include the cardinal direction. 
	*end = 0;
	*LONG = start;
	
	start = ++end;
	end = strchr(start, ',');
	*end = 0;
	*speed = start; 
	
	start = strchr(++end, ',');
	start = ++start;
	end = strchr(start, ',');
	*end = 0;
	*date = start;
	
	//use atof(start) if need to convert to float. 
}	
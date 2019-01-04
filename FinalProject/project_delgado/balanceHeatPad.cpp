#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

const int SENSOR_PIN = 7;
//const int HEATINGPAD_PIN = 16;

int buffer[5] = {0, 0, 0, 0, 0}; //buffer to hold humidity, temperature, and humidity
int lastStateBuffer[4] = {0, 0, 0, 0}; //buffer to hold the previous humidity and temperature

uint8_t laststate;
uint8_t counter;
uint8_t j;
float fahrenheit, temperatureAPI;
bool heaterON = false;		//represents ON & OFF states for heating pad

void resetBuffer();
void extractSensorData();
void setLastStateBuffer();
bool buffersAreEqual();
void setInitialTemperature();
void* getTemperatureAPI(void *param);

int main(){
	pthread_t tid;
	pthread_attr_t attr;

	cout << "Raspberry Pi wiringPi DHT11 Temperature test program" << endl ;
	
	//if initializing wiritingPi library does not work exit
	if ( wiringPiSetup() == -1 )
		exit( 1 );
	
	cout << "Trying to set Initial value..." << endl;	
	setInitialTemperature(); //get initial temperature
	cout << endl << "checking for change in temperature..." << endl;

	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, getTemperatureAPI, NULL);

	while ( 1 ){  //infinit loop
		extractSensorData(); //extract instance of temperature and humidity

		if ( (j >= 40) &&
	     	(buffer[4] == ( (buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xFF) ) && !buffersAreEqual()){
		     	//0xFF to get least significant bits only - checksum only looks at the end of all precceding values
		     	
			fahrenheit = buffer[2] * 9. / 5. + 32;		//change celcius to fahrenheit	
			printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",
								buffer[0], buffer[1], buffer[2], buffer[3], fahrenheit );
			setLastStateBuffer();
		}
		//else --> if if-block did not execute data was not proccessed, sent correctly and dropped this iteration
				//or it is the same tempa and humidity values as last iteration
		


		/*if(fahrenheit <= temperatureAPI && !heaterON){ //if heating pad is on it is in the process of heating
			pinMode( HEATINGPAD_PIN, OUTPUT );
			digitalWrite( HEATINGPAD_PIN, HIGH ); //turn on heating pad to heat raise temperature
			heaterON = true;
		}else if(fahrenheit >= temperatureAPI && heaterON){
			digitalWrite( HEATINGPAD_PIN, LOW ); //turn off heating pad
			heaterON = false;
		}*/

		delay(1000); //let sensor and PI reset	
	}
	return(0);
}

void* getTemperatureAPI(void *param){

	ifstream input("file.txt");

	while(1){	//infinit loop
		input >> temperatureAPI; //get temperature from python program
		sleep(60); //grabs temperature every minute ---> should realistically be longer (maby every 10 minutes)
	}
}

void resetBuffer(){ //function to reset buffer for next iteration
	int i;
	for(i = 0; i < 5; i++){
		buffer[i] = 0;
	}
}

void extractSensorData(){
	///////////*re-initialize values*/////////
	laststate= HIGH;						//
	counter	= 0;							//
	j= 0;									//
											//
	resetBuffer();							//
	//////////////////////////////////////////

	delay( 1000 ); //wait one second to refresh
		
	pinMode( SENSOR_PIN, OUTPUT );
	
	//*initialize PI's request to sensor*//
	digitalWrite( SENSOR_PIN, LOW );	///
	delay( 18 );						//delay to let Sensor detect signal --> initialize DHT11 in a sense
	digitalWrite( SENSOR_PIN, HIGH );	///
	delayMicroseconds( 40 );   			//delay to let DHT11 to respond or give it time to sense temperature and humidity	
	///////////////////////////////////////

	pinMode(SENSOR_PIN, INPUT);
		
	int i;
	for(i = 0; i < 85; i++){ //avoid infinit loop
		counter = 0;
		while(digitalRead(SENSOR_PIN) == laststate){ //keep looping until a change occurs
			counter++;
			delayMicroseconds(1);
			if(counter == 255){  //if no change within the size of uint8_t(size --> 0-255), break out of loop
				break;
			}
		}
			
		laststate = digitalRead(SENSOR_PIN);

		if(counter == 255){ //no change
			break;
		}

		if((i >= 4) && (i % 2 == 0)){
			buffer[j/8] <<= 1;
			if(counter > 16){
				buffer[j/8] |= 1;
			}
			j++;
		}
	}
}

void setLastStateBuffer(){
	int i;
	for(i = 0; i < 4; i++){
		lastStateBuffer[i] = buffer[i]; //duplicate buffer to lastStateBuffer
	}
	
}

bool buffersAreEqual(){
	if(lastStateBuffer[0] != buffer[0] || lastStateBuffer[2] != buffer[2] ){
		return false;
	}else{
		return true;	
	}
}

void setInitialTemperature(){
	bool initialTemp = false; //intial temperature not set yet

	while ( !initialTemp ){  //loop until intial temperature and humidity are set
	
		extractSensorData(); //extract instance of temperature and humidity

		if ( (j >= 40) &&
	     	(buffer[4] == ( (buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xFF) ) ){
	     		//0xFF to get least significant bits only - checksum only looks at the end of all precceding values
	     	
	     	cout << "initial temperature is set" << endl << endl;
	     		
			fahrenheit = buffer[2] * 9. / 5. + 32;		//change celsius to fahrenheit
			printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",
										buffer[0], buffer[1], buffer[2], buffer[3], fahrenheit );
			setLastStateBuffer();
			initialTemp = true; //intial temperature set yet
			
		}else  {
			//data was not proccessed or sent correctly, drop this iteration and try again
		}
		delay(1000); //let sensor and PI reset	
	}
}







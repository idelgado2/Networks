 /*************************************************************************
 * Networks Assignment 3
 * Isaac Delgado
 * COSC_4342
 * 11/8/2016
 * Purpose: This program will be givin a subnet mask and a network address in
 *			dotted decimal notation and will find the DNS names assigned to all
 *			host on that particular subnet.
 *
 * Input:   This program will take dotted notation subnetmask and network address
 *
 * Output:  The ouput of this program will be a listing of all available host
 *			by IP address and the corresponding DNS name.
 **************************************************************************/

#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <fcntl.h>
#include <math.h>

char* addHostToIP(const char *s1, int num){
    char *result = malloc(strlen(s1)+1);
	strcpy(result, s1);

	char host[10];
    sprintf(host, "%d", num);

	strcat(result, host); //adding host number to the netwrok address

    return result;
}

char* getNetworkAddress(const char *s1, const char *s2, const char *s3){
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, "."); 	//add "."
    strcat(result, s2);		//add second octet
    strcat(result, "."); 	//add "."
    strcat(result, s3);		//add third octet
    strcat(result, ".");	//add "."
	
    return result;
}

void listAllHost(char * address, int start, int stop){
	struct hostent *hostptr;
 	u_long ip_addr_long; //to hold binary IP address
 	char* addr_ptr;

 	int i;
 	for(i = start; i <= stop; i++){ //this will loop depedning on the subnetmask
    			char * IPaddress = addHostToIP(address, i);
    			ip_addr_long = inet_addr(IPaddress);
				addr_ptr = (char *) &ip_addr_long;
				hostptr = gethostbyaddr(addr_ptr, sizeof(addr_ptr), AF_INET);


				if(hostptr == NULL){
					printf("%s  ====> ** HOST NOT FOUND **\n", IPaddress);
				}
				else{
					printf("%s  ====> %s\n", IPaddress, hostptr -> h_name);
				}
    }
}


int main(int argc, char *argv[]){


 	if(argc != 3){
        printf("*** Invalid, This program takes two arguments\n");
        exit(1);
    }

    char temp[50];
    char *testing;

    strcpy(temp, argv[1]); //put subnetmask into temporary array

    testing = strtok(temp, ".");
    if(strcmp(testing, "255") != 0){
    	exit(1);// invalid subnet mask
    }

    testing = strtok(NULL, ".");
    if(strcmp(testing, "255") != 0){
    	exit(1);// invalid subnet mask
    }

    testing = strtok(NULL, ".");
    if(strcmp(testing, "255") != 0){
    	exit(1);// invalid subnet mask
    }

    testing = strtok(NULL, ".");
    char * numOfSubnet_temp;
    strcpy(numOfSubnet_temp, testing); //copy last number in subnet mask string to exclude extra memory

    int numOfSubnet = atoi(numOfSubnet_temp);	//change to integer value
    printf("the numOfSubnet: %d\n", numOfSubnet);

    ///////////////*******///////////////

    char temp2[50];
    char * holder;

    holder = strtok(argv[2], ".");		//take first octet from provided IP address
    char * octet_1 = malloc(strlen(holder) + 1);
    strcpy(octet_1, holder);

    holder = strtok(NULL, ".");			//take second octet from provided IP address
    char * octet_2 = malloc(strlen(holder) + 1);
    strcpy(octet_2, holder);

    holder = strtok(NULL, ".");			//take third octet from provided IP address
    char * octet_3 = malloc(strlen(holder) + 1);
    strcpy(octet_3, holder);

    char * networkAddr = getNetworkAddress(octet_1, octet_2, octet_3);	//get network address
    printf("Network Address: %s\n", addHostToIP(networkAddr, 0));

    switch(numOfSubnet){
    	case 0: 	//only one subnet
    		listAllHost(networkAddr, 1, 254);
    		break;
    	case 128: 	//two available subnets
    		listAllHost(networkAddr, 1, 126);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 129, 254);
    		break;
    	case 192: 	//four available subnets
    		listAllHost(networkAddr, 1, 62);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 64, 126);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 127, 190);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 193, 254);
    		break;
    	case 224: 	//8 available subnets
    		listAllHost(networkAddr, 1, 30);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 33, 62);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 65, 94);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 97, 126);
    		listAllHost(networkAddr, 129, 158);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 161, 190);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 193, 222);
    		printf("\n*** end of subnet ****\n\n");
    		listAllHost(networkAddr, 225, 254);
    		break;
    	default:
    		break;
    }

 }












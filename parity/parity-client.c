#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int countones(char *str) {
	int count1 = 0;
	for(int i = 0; i < strlen(str); i++) {
			if(str[i] == '1') {
				count1++;
			}
	}
	return count1;
}

int main() {
  int sd, cadl;
  struct sockaddr_in sad, cad;
  char str[50];

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");
  sad.sin_port = htons(9995);
  connect(sd, (struct sockaddr*)&sad, sizeof(sad));
  printf("Enter the databit: ");
  scanf("%s",str);
  int len = strlen(str); //string length
  // If there are even number of 1's then parity bit is 0
  int count1 = countones(str);
  if(count1%2 == 0){
  	str[len] = '0'; // send this converted data to server
  } else {
	str[len] = '1'; // send this converted data to server
  }
  str[len+1] = '\0'; // make sure the next bit to be null
  send(sd,str,sizeof(str),0);
  close(sd);
}

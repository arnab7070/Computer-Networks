#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
void binary_addition(char *result, char *bin1, char *bin2) {
  int carry = 0;
  for (int i = 7; i >= 0; i--) {
    int bit1 = bin1[i] - '0';
    int bit2 = bin2[i] - '0';
    int sum = bit1 + bit2 + carry;
    result[i] = (sum % 2) + '0';
    carry = sum / 2;
  }

  // Handle overflow
  if (carry) {
    for (int i = 7; i >= 0; i--) {
      int bit = result[i] - '0';
      int sum = bit + carry;
      result[i] = (sum % 2) + '0';
      carry = sum / 2;
    }
  }
}
int main() {
  int sd, cd, cadl;
  char segmentedstr[10][10];
  struct sockaddr_in sad, cad;

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");
  sad.sin_port = htons(9995);
  bind(sd, (struct sockaddr*)&sad, sizeof(sad));
  listen(sd, 10);
  cadl = sizeof(cad);
  cd = accept(sd, (struct sockaddr*)&cad, &cadl);
  recv(cd, segmentedstr, sizeof(segmentedstr), 0);
  char sum[9] = "00000000";
  char temp1[9];
  for (int i = 0; i < 5; i++) {
    strcpy(temp1, sum);
    binary_addition(sum, temp1, segmentedstr[i]);
  }
  for(int i = 0; i < strlen(sum); i++) {
      if(sum[i] != '1'){
      	printf("There is some error in data received\n");
        return 1;
      }
  }
  printf("Success! Data received is absolutely ok.\n");
  close(cd);
  close(sd);
}

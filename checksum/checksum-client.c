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
void ones_complement(char *bin) {
  for (int i = 0; i < 8; i++) {
    bin[i] = (bin[i] == '0') ? '1' : '0';
  }
}
int main(void) {
  int sd, cadl;
  struct sockaddr_in sad, cad;
  char str[64];
  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");
  sad.sin_port = htons(9995);
  connect(sd, (struct sockaddr*)&sad, sizeof(sad));
  printf("Enter the databit: ");
  scanf("%s", str);
  printf("Enter segment length: ");
  int seglen;
  scanf("%d", &seglen);
  char segmentedstr[10][10];
  int totalSegments = strlen(str) / seglen;
  for (int i = 0; i < totalSegments; i++) {
    int temp = 8 * i; // temp value for next round
    for (int j = 0; j < seglen; j++) {
      segmentedstr[i][j] = str[temp + j];
    }
  }
  char sum[9] = "00000000";
  char temp[9];

  for (int i = 0; i < 4; i++) {
    strcpy(temp, sum);
    binary_addition(sum, temp, segmentedstr[i]);
  }
  ones_complement(sum);
  strcpy(segmentedstr[4], sum);
  send(sd,segmentedstr,sizeof(segmentedstr),0);
  close(sd);
}

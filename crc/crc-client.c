#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
void xorOperation(char *remainder, const char *divisor) {
  for (int i = 0; i < strlen(divisor); i++) {
    remainder[i] = (remainder[i] == divisor[i]) ? '0' : '1';
  }
}
int main(void) {
  int sd, cadl;
  struct sockaddr_in sad, cad;
  char str[64], genpoly[8], remainder[7] = "000000";
  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");
  sad.sin_port = htons(9995);
  connect(sd, (struct sockaddr*)&sad, sizeof(sad));
  printf("Enter the data to be transmitted: ");
  scanf("%s", str);
  printf("Enter the generation polynomial: ");
  scanf("%s", genpoly);
  int len = strlen(str);
  int polylen = strlen(genpoly);
  // zeros will be 1 less than the length of generation polynomial
  for (int i = len; i < len + polylen - 1; i++) {
    str[i] = '0'; // pad with zeros
  }
  str[len + polylen - 1] = '\0';
  // Iterate through each bit of the dividend
  for (int i = 0; i < strlen(str); i++) {
    // Append the current bit to the current remainder
    remainder[polylen - 1] = str[i];

    // If the length of the current remainder is greater than or equal to the
    // length of the divisor
    if (strlen(remainder) >= strlen(genpoly)) {
      // Perform a bitwise XOR operation between the current remainder and the
      // divisor
      xorOperation(remainder, genpoly);
    }
  }

  printf("Databit padded with zeros: %s\n", str);
  printf("Remainder is: %s\n", remainder);
  int rem_len = strlen(remainder); //6
  char trimmedRemainder[10];
  int curridx = (polylen - 1) - 1;
  for (int i = rem_len - 1; i >= rem_len - (polylen - 1); i--) {
    trimmedRemainder[curridx--] = remainder[i];
  }
  trimmedRemainder[polylen-1] = '\0'; // make sure to terminate the string
  printf("CRC bits are: %s\n", trimmedRemainder);
  // update the final databit
  curridx = 0;
  for (int i = len; i < len + polylen - 1; i++) {
    str[i] = trimmedRemainder[curridx++];
  }
  send(sd,str,sizeof(str),0);
  close(sd);
}

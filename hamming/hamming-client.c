// NOTE: Please try to compile this code with -lm flag.
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void printCodeword(char *codeword) {
  printf("Codeword is: ");
  int len = strlen(codeword);
  for (int i = 1; i <= len; i++) {
    printf("%c", codeword[i]);
  }
  printf("\n");
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
  printf("Enter the data: ");
  scanf("%s", str);
  int data_len = strlen(str);
  int redundant_bits = 0;
  // no. of redundant bits [pow(2,i) >= dl+i+1]
  while (true) {
    if (pow(2, redundant_bits) >= data_len + redundant_bits + 1)
      break;
    redundant_bits++;
  }
  printf("Total number of redundant bits are: %d\n", redundant_bits);
  char codeword[64];
  int total_len = data_len + redundant_bits;
  for (int i = 0; i < redundant_bits; i++) {
    codeword[total_len + 1 - (int)pow(2, i)] = 'P';
  }
  // update the actual codeword
  int curridx = 0;
  for (int i = 1; i <= total_len; i++) {
    if (codeword[i] != 'P') // P means parity bit
      codeword[i] = str[curridx++];
  }
  codeword[total_len + 1] = '\0'; // Terminal of codeword
  printCodeword(codeword);

  for (int i = total_len; i >= 1; i--) {
    if (codeword[i] == 'P') {
      int limit = total_len - i + 1;
      int index = i;
      bool ignore = false;
      int countOne = 0;
      while (index > 0) {      // Ensure index is greater than 0
        int codelimit = limit; // Renew the limit value
        if (!ignore) {
          while (codelimit > 0 && index > 0) { // Select bits
            if (codeword[index] != 'P' && codeword[index] == '1') {
              countOne++;
            }
            index--;
            codelimit--;
          }
        } else {
          while (codelimit > 0 && index > 0) { // Ignore bits
            index--;
            codelimit--;
          }
        }
        ignore = !ignore; // Toggle the ignore flag
      }
      codeword[i] = countOne % 2 ? '1' : '0';
      countOne = 0; // refresh the countOne value for next loop
    }
  }
  send(sd,codeword,sizeof(codeword),0);
  close(sd);
}

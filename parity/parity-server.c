#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
  int sd, cd, cadl;
  char str[50];
  struct sockaddr_in sad, cad;

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");
  sad.sin_port = htons(9995);
  bind(sd, (struct sockaddr*)&sad, sizeof(sad));
  listen(sd, 10);
  cadl = sizeof(cad);
  cd = accept(sd, (struct sockaddr*)&cad, &cadl);
  recv(cd, str, sizeof(str), 0);
  int count = 0;
  for(int i = 0; i < strlen(str); i++) {
      if(str[i] = '1') count++;
  }
  if(count % 2 == 0) printf("There is no error in data\n");
  else printf("There is error in data\n");
  close(cd);
  close(sd);
}

#include <unistd.h>

int ft_strlen(char *str);

int ft_strlen(char *str){
  int i = 0;
  while(str[i]){
    i++;
  }
  return i;
}

// int main(void)
// {
//   char x;
//
//   x = ft_strlen("ABCD") + '0';
//   write(1,&x,1);
//   return 0;
// }

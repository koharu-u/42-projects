#include <unistd.h>

void ft_putstr(char *str);

void ft_putstr(char *str){
  write(1, str,1);
}

// int main(void)
// {
//   ft_putstr("A");
// }

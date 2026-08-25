#include <unistd.h>

// Test library prototype
int	ft_isalpha(char *str);

// Main test
int	main(void)
{
	int	test_string;

	test_string = '9';
	if (ft_isalpha(test_string))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = 'a';
	if (!(ft_isalpha(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = 'A';
	if (ft_isalpha(test_string))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = ' ';
	if (!(ft_isalpha(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = '&';
	if (!(ft_isalpha(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
}

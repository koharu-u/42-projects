#include <unistd.h>

// Test library prototype
int	ft_isdigit(int str);

// Main test
int	main(void)
{
	int	test_string;

	test_string = 'z';
	if (!(ft_isdigit(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = '0';
	if (ft_isdigit(test_string))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = 'A';
	if (!(ft_isdigit(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = '$';
	if (!(ft_isdigit(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = '*';
	if (!(ft_isdigit(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
}

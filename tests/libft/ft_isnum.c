#include <unistd.h>

// Test library prototype
int	ft_isnum(char *str);

// Main test
int	main(void)
{
	char	*test_string;

	test_string = "91h072913h";
	if (ft_isnum(test_string))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = "1234567890";
	if (ft_isnum(test_string))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = "HDKJAdshdjsad";
	if (!(ft_isnum(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = "";
	if (!(ft_isnum(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	test_string = "&(^*(!@%#^(*@!%&(@)";
	if (!(ft_isnum(test_string)))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
}

#include "philo_one.h"

int		ft_atoi(const char *str)
{
	int			result;
	int			digit;
	long int	lon;

	result = 0;
	lon = 0;
	while (*str)
	{
		digit = *str - '0';
		lon = (10 * lon) + digit;
		result = (10 * result) + digit;
		if (lon < 0)
			return -1;
		str++;
	}
	return result;
}

void	ft_putnbr(int n)
{
	long lnbr;
	char c;

	lnbr = n;
	if (lnbr >= -2147483648 && lnbr <= 2147483648)
	{
		if (lnbr > 9)
		{
			ft_putnbr(lnbr / 10	);
			c = lnbr % 10 + '0';
			write(1, &c, 1);
		}
		else
		{
			c = lnbr + '0';
			write(1, &c, 1);
		}
	}
}

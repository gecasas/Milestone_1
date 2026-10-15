#include "push_swap.h"

/*
** Radix LSD en base 2 sobre los indices (0..n-1, todos >= 0).
** Por cada bit: los que tienen ese bit a 0 van a b (pb),
** los que lo tienen a 1 se quedan en a (ra). Luego todo vuelve con pa.
** Si a queda ordenado antes de acabar los bits, se para.
*/
static void	radix_pass(t_stack **a, t_stack **b, int size, int bit)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (((*a)->index >> bit) & 1)
			ra(a);
		else
			pb(a, b);
		i++;
	}
	while (*b)
		pa(a, b);
}

void	sort_radix(t_stack **a, t_stack **b)
{
	int	size;
	int	bits;
	int	bit;

	size = stack_size(*a);
	if (size < 2)
		return ;
	bits = get_max_bits(size - 1);
	bit = 0;
	while (bit < bits && !is_sorted(*a))
	{
		radix_pass(a, b, size, bit);
		bit++;
	}
}

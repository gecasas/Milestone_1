#include "push_swap.h"

/*
** Pasa a b todos los indices de [start, end).
** Los de la mitad baja del chunk se mandan al fondo de b (rb),
** asi b queda "medio ordenado" y la vuelta atras es mas barata.
*/
static void	push_chunk(t_stack **a, t_stack **b, int start, int end)
{
	int	pushed;
	int	mid;

	pushed = 0;
	mid = start + (end - start) / 2;
	while (pushed < end - start)
	{
		if ((*a)->index >= start && (*a)->index < end)
		{
			pb(a, b);
			if ((*b)->index < mid && (*b)->next)
				rb(b);
			pushed++;
		}
		else
			ra(a);
	}
}

/*
** b contiene los indices 0..size-1, asi que el maximo es size - 1.
** Se sube por el camino mas corto (rb o rrb) y se devuelve con pa.
*/
static void	push_back(t_stack **a, t_stack **b)
{
	int	size;
	int	pos;

	while (*b)
	{
		size = stack_size(*b);
		pos = get_position(*b, size - 1);
		if (pos <= size / 2)
		{
			while (pos-- > 0)
				rb(b);
		}
		else
		{
			while (pos++ < size)
				rrb(b);
		}
		pa(a, b);
	}
}

void	sort_chunks(t_stack **a, t_stack **b)
{
	int	size;
	int	chunk;
	int	start;
	int	end;

	size = stack_size(*a);
	if (size < 2 || is_sorted(*a))
		return ;
	chunk = ft_sqrt(size);
	start = 0;
	while (start < size)
	{
		end = start + chunk;
		if (end > size)
			end = size;
		push_chunk(a, b, start, end);
		start = end;
	}
	push_back(a, b);
}

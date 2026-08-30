#ifndef LIBFT_REQUIRED_SYMBOL
# error "LIBFT_REQUIRED_SYMBOL must name the direct-test function"
#endif

/*
 * A link-time reference keeps source-only checks honest even when that
 * function has no behavioural test registered yet.  Its declaration need not
 * match the student's full prototype because this harness never calls it.
 */
extern void	LIBFT_REQUIRED_SYMBOL(void);

void	(*libft_required_symbol)(void) = LIBFT_REQUIRED_SYMBOL;

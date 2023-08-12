void	sel_new_tgt_corn(int ***pts, t_point p, int (*curr)[3])
{
	t_point	**movs;
	int		i;
	int len_pts;

	len_pts = 0;
	while (pts[len_pts] != NULL)
		len_pts++;
	i = 4 * len_pts;
	movs = ptn_tgt_moves(pts, len_pts, p);
	rev_p(sort_p(movs, i), i);
	//sort_p(movs, i);
	i = 0;
	while (movs[i] != NULL)
	{
		if (movs[i]->val != INT_MAX && !a_contains(*curr, movs[i]->val2, 3))
			break ;
		i++;
	}
	if (movs[i] != NULL)
		a_shift(*curr, movs[i]->val2, 3);
	else
		null_a(*curr, 3);
	free_p(movs);
}

void	sel_new_tgt_obj(int ***pts, t_point p, int (*curr)[3])
{
	t_point	**movs;
	int		i;
	int len_pts;

	len_pts = 0;
	while (pts[len_pts] != NULL)
		len_pts++;
	i = 4 * len_pts;
	movs = ptn_tgt_moves(pts, len_pts, p);
	sort_p(movs, i);
	i = 0;
	while (movs[i] != NULL && a_contains(*curr, movs[i]->val2, 3))
		i++;
	if (movs[i] != NULL)
		a_shift(*curr, movs[i]->val2, 3);
	else
	{
		null_a(*curr, 3);
		a_shift(*curr, movs[0]->val2, 3);
	}
	free_p(movs);
}

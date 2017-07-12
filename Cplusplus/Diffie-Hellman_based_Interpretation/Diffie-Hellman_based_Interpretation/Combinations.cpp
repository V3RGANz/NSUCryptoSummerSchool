unsigned C(unsigned n, unsigned k) {
	if (k > n >> 1)
		k = n - k;
	unsigned ret = 1;
	for (unsigned i = 1; i <= k; ++i)
		ret = ret * (n - k + i) / i;
	return ret;
}
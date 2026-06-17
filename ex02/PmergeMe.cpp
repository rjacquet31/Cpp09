#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other) : _vec(other._vec), _deq(other._deq) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

static bool	isValidNumber(const std::string &s, int &out)
{
	if (s.empty())
		return false;
	for (std::size_t i = 0; i < s.size(); i++)
		if (!std::isdigit(s[i]))
			return false;
	std::istringstream iss(s);
	long n;
	iss >> n;
	if (n > 2147483647)
		return false;
	out = static_cast<int>(n);
	return true;
}

static std::vector<int>	buildJacobsthal(int n)
{
	std::vector<int> jac;
	jac.push_back(0);
	jac.push_back(1);
	while (jac.back() < n)
		jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);

	std::vector<int> order;
	for (std::size_t i = 1; i < jac.size(); i++)
	{
		int curr = std::min(jac[i], n);
		int prev = jac[i - 1];
		for (int k = curr; k > prev; k--)
			order.push_back(k - 1);
	}
	return order;
}

void	PmergeMe::mergeInsertVector(std::vector<int> &v)
{
	if (v.size() <= 1)
		return;

	bool hasStraggler = v.size() % 2 != 0;
	int straggler = hasStraggler ? v.back() : 0;
	if (hasStraggler)
		v.pop_back();

	std::vector<std::pair<int, int> > pairs;
	for (std::size_t i = 0; i < v.size(); i += 2)
	{
		int big = v[i] > v[i + 1] ? v[i] : v[i + 1];
		int small = v[i] > v[i + 1] ? v[i + 1] : v[i];
		pairs.push_back(std::make_pair(big, small));
	}

	std::vector<int> winners;
	for (std::size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);

	mergeInsertVector(winners);

	std::vector<int> chain(winners);
	std::vector<int> pending;
	for (std::size_t i = 0; i < pairs.size(); i++)
		pending.push_back(pairs[i].second);

	chain.insert(std::lower_bound(chain.begin(), chain.end(), pending[0]), pending[0]);

	std::vector<int> order = buildJacobsthal(pending.size());

	for (std::size_t i = 0; i < order.size(); i++)
	{
		int idx = order[i];
		if (idx == 0)
			continue;
		std::vector<int>::iterator pos = std::lower_bound(chain.begin(), chain.end(), pending[idx]);
		chain.insert(pos, pending[idx]);
	}

	if (hasStraggler)
	{
		std::vector<int>::iterator pos = std::lower_bound(chain.begin(), chain.end(), straggler);
		chain.insert(pos, straggler);
	}

	v = chain;
}

void	PmergeMe::mergeInsertDeque(std::deque<int> &d)
{
	if (d.size() <= 1)
		return;

	bool hasStraggler = d.size() % 2 != 0;
	int straggler = hasStraggler ? d.back() : 0;
	if (hasStraggler)
		d.pop_back();

	std::deque<std::pair<int, int> > pairs;
	for (std::size_t i = 0; i < d.size(); i += 2)
	{
		int big = d[i] > d[i + 1] ? d[i] : d[i + 1];
		int small = d[i] > d[i + 1] ? d[i + 1] : d[i];
		pairs.push_back(std::make_pair(big, small));
	}

	std::deque<int> winners;
	for (std::size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);

	mergeInsertDeque(winners);

	std::deque<int> chain(winners);
	std::deque<int> pending;
	for (std::size_t i = 0; i < pairs.size(); i++)
		pending.push_back(pairs[i].second);

	chain.insert(std::lower_bound(chain.begin(), chain.end(), pending[0]), pending[0]);

	std::vector<int> order = buildJacobsthal(pending.size());

	for (std::size_t i = 0; i < order.size(); i++)
	{
		int idx = order[i];
		if (idx == 0)
			continue;
		std::deque<int>::iterator pos = std::lower_bound(chain.begin(), chain.end(), pending[idx]);
		chain.insert(pos, pending[idx]);
	}

	if (hasStraggler)
	{
		std::deque<int>::iterator pos = std::lower_bound(chain.begin(), chain.end(), straggler);
		chain.insert(pos, straggler);
	}

	d = chain;
}

void	PmergeMe::printSequence(const std::string &label, const std::vector<int> &v) const
{
	std::cout << label;
	for (std::size_t i = 0; i < v.size(); i++)
		std::cout << " " << v[i];
	std::cout << std::endl;
}

void	PmergeMe::printSequence(const std::string &label, const std::deque<int> &d) const
{
	std::cout << label;
	for (std::size_t i = 0; i < d.size(); i++)
		std::cout << " " << d[i];
	std::cout << std::endl;
}

void	PmergeMe::sortVector()
{
	std::clock_t start = std::clock();
	mergeInsertVector(_vec);
	std::clock_t end = std::clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
	std::cout << "Time to process a range of " << _vec.size()
		<< " elements with std::vector : " << elapsed << " us" << std::endl;
}

void	PmergeMe::sortDeque()
{
	std::clock_t start = std::clock();
	mergeInsertDeque(_deq);
	std::clock_t end = std::clock();

	double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
	std::cout << "Time to process a range of " << _deq.size()
		<< " elements with std::deque : " << elapsed << " us" << std::endl;
}

void	PmergeMe::parseAndSort(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		int n;
		if (!isValidNumber(std::string(argv[i]), n))
			throw std::runtime_error("Error: invalid input: " + std::string(argv[i]));
		_vec.push_back(n);
		_deq.push_back(n);
	}

	printSequence("Before:", _vec);
	sortVector();
	sortDeque();
	printSequence("After: ", _vec);
}
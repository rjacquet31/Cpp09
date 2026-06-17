#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <deque>
# include <ctime>

class PmergeMe
{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		void	parseAndSort(int argc, char **argv);

	private:
		std::vector<int>	_vec;
		std::deque<int>		_deq;

		void	sortVector();
		void	sortDeque();

		void	mergeInsertVector(std::vector<int> &v);
		void	mergeInsertDeque(std::deque<int> &d);

		std::vector<int>	jacobsthalSequence(int n) const;

		void	printSequence(const std::string &label, const std::vector<int> &v) const;
		void	printSequence(const std::string &label, const std::deque<int> &d) const;
};

#endif
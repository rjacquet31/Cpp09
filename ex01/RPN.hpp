#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <stack>

class RPN
{
	public:
		RPN();
		RPN(const RPN &other);
		RPN &operator=(const RPN &other);
		~RPN();

		int	evaluate(const std::string &expression) const;

	private:
		bool	isOperator(char c) const;
		int		applyOperator(char op, int a, int b) const;
};

#endif
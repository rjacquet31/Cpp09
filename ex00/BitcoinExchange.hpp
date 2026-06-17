#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include <string>
# include <stdexcept>
# include <cctype>
# include <map>

class BitcoinExchange
{
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void	loadDatabase(const std::string &filename);
		void	processInput(const std::string &filename) const;

	private:
		std::map<std::string, float>	_db;

		bool	isValidDate(const std::string &date) const;
		bool	isValidValue(const std::string &valueStr, float &value) const;
		float	getRate(const std::string &date) const;
};

#endif

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _db(other._db) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_db = other._db;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

static int	toInt(const std::string &s)
{
	int n = 0;
	std::istringstream(s) >> n;
	return n;
}

static std::string	trim(const std::string &s)
{
	std::size_t start = s.find_first_not_of(" \t\r\n");
	std::size_t end   = s.find_last_not_of(" \t\r\n");
	if (start == std::string::npos)
		return "";
	return s.substr(start, end - start + 1);
}

void	BitcoinExchange::loadDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open database file.");

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		line = trim(line);
		if (line.empty())
			continue;

		std::size_t comma = line.find(',');
		if (comma == std::string::npos)
			continue;

		std::string date    = trim(line.substr(0, comma));
		std::string rateStr = trim(line.substr(comma + 1));

		float rate = 0.0f;
		std::istringstream(rateStr) >> rate;
		_db[date] = rate;
	}
	file.close();
}

bool	BitcoinExchange::isValidDate(const std::string &date) const
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(date[i]))
			return false;
	}

	int year  = toInt(date.substr(0, 4));
	int month = toInt(date.substr(5, 2));
	int day   = toInt(date.substr(8, 2));

	if (month < 1 || month > 12 || day < 1 || day > 31)
		return false;
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		return false;
	if (month == 2)
	{
		bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (day > (leap ? 29 : 28))
			return false;
	}
	return true;
}

bool	BitcoinExchange::isValidValue(const std::string &valueStr, float &value) const
{
	std::istringstream iss(valueStr);
	iss >> value;
	return !iss.fail() && iss.eof();
}

float	BitcoinExchange::getRate(const std::string &date) const
{
	std::map<std::string, float>::const_iterator it = _db.lower_bound(date);

	if (it != _db.end() && it->first == date)
		return it->second;
	if (it == _db.begin())
		return -1.0f;
	--it;
	return it->second;
}

void	BitcoinExchange::processInput(const std::string &filename) const
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		line = trim(line);
		if (line.empty())
			continue;

		std::size_t sep = line.find(" | ");
		if (sep == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date     = trim(line.substr(0, sep));
		std::string valueStr = trim(line.substr(sep + 3));

		if (!isValidDate(date))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		float value = 0.0f;
		if (!isValidValue(valueStr, value))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (value < 0.0f)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000.0f)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}

		float rate = getRate(date);
		if (rate < 0.0f)
		{
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		std::cout << date << " => " << value << " = " << value * rate << std::endl;
	}
	file.close();
}
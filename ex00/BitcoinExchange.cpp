#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const std::string &dbFilename)
{
	this->_loadDatabase(dbFilename);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src)
{
	*this = src;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange& src)
{
	if (this != &src)
		this->_database = src._database;
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::_loadDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "Error:could not open database file." << std::endl;
		std::exit(1);
	}

	std::string line;
	std::getline(file, line);  //so we can skip the first line "date, exchange_rate"

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		size_t commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;
		
		std::string date = line.substr(0, commaPos);
		std::string rateStr = line.substr(commaPos + 1);
		float rate = std::atof(rateStr.c_str());

		this->_database[date] = rate;
	}
}

static bool isLeapYear(int year) //validation helper outside of the class
{
	return ((year % 4 ==0 && year % 100 != 0) || year % 400 == 0);
}


bool BitcoinExchange::_isValidDate(const std::string &date) const
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	
	for (size_t i = 0; i <date.size(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(date[i]))
			return(false);
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12)
		return (false);
	if (day < 1 )
		return (false);
	
	int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (isLeapYear(year))
		daysInMonth[1] = 29;
	
	if (day > daysInMonth[month - 1])
		return (false);
	
	return (true);
}

bool BitcoinExchange::_isValidValue(const std::string &value, float &result) const
{
	if (value.empty())
		return (false);

	size_t i = 0;
	bool hasDigit = false;
	bool hasDot = false;

	if (value[0] == '-' || value[0] == '+')
		i = 1;
	
	for (; i< value.size(); ++i)
	{
		if (value[i] == '.' && !hasDot)
			hasDot = true;
		else if (std::isdigit(value[i]))
			hasDigit = true;
		else 
			return(false);
	}
	if (!hasDigit)
		return (false);

	result = std::strtod(value.c_str(), NULL);

	if (result < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return (false);
	}
	if (result > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return (false);
	}
	return (true);
}



void BitcoinExchange::_parseLine(const std::string &line) const
{
	size_t sepPos = line.find('|');

	if (sepPos == std::string::npos)
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return ;
	}

	std::string date = line.substr(0, sepPos);
	std::string valueStr = line.substr(sepPos + 1);

	while (!date.empty() && date[date.size() - 1] == ' ')
		date.erase(date.size() - 1);
	while (!valueStr.empty() && valueStr[0] == ' ')
		valueStr.erase(0, 1);

	if (!this->_isValidDate(date))
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return ;
	}

	float value;
	if (!this->_isValidValue(valueStr, value))
		return;
	if (this->_database.empty())
	{
		std::cerr << "Error: database is empty." << std::endl;
		return;
	}
	std::map<std::string, float>::const_iterator it = this->_database.lower_bound(date);
	if (it == this->_database.end() || it->first != date)
	{
		if (it == this->_database.begin())
		{
			std::cerr << "Error: no earlier date in database for => " << date << std::endl;
			return ;
		}
		--it;
	}
	std::cout << date << " => " << valueStr << " = " << (it->second * value) << std::endl;
}


void BitcoinExchange::processFile(const std::string &filename) const
{
	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "Error:could not open file." << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);

	while(std::getline(file, line))
	{
		if (line.empty())
			continue;
		this->_parseLine(line);
	}
}
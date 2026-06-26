#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <exception>
#include <string>

class BitcoinExchange
{
	private:
		std::map<std::string, float> _database;

		bool _isValidDate(const std::string &date) const;
		bool _isValidValue(const std::string &value, float &result) const;
		void _loadDatabase(const std::string &filename);
		void _parseLine(const std::string &line) const;

	public:
		BitcoinExchange();
		BitcoinExchange(const std::string &dbFilename);
		BitcoinExchange(const BitcoinExchange& src);
		BitcoinExchange& operator=(const BitcoinExchange& src);
		~BitcoinExchange();

		void processFile(const std::string &filename) const;
};

#endif
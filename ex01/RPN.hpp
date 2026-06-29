#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

class RPN
{
	private:
		std::stack<double> _stack;
		
		bool _isOperator(const std::string	&token) const;
		bool _isNumber(const std::string &token) const;
		void _applyOperator(const std::string &op);

	public:
		RPN();
		RPN(const RPN& source);
		RPN& operator=(const RPN& source);
		~RPN();

		double evaluate(const std::string &expression);
};

#endif
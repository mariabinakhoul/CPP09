#include "RPN.hpp"

RPN::RPN()
{
}
RPN::RPN(const RPN& source)
{
	*this = source;
}
RPN &RPN::operator=(const RPN& source)
{
	if (this != &source)
		this->_stack = source._stack;
	return (*this);
}

RPN::~RPN()
{
}

bool RPN::_isOperator(const std::string	&token) const
{
	return(token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::_isNumber(const std::string &token) const
{
	if (token.empty())
		return(false);

	size_t i = 0;
	if (token[0] == '-' || token[0] == '+')
		i = 1;
	if (i == token.size())
		return(false);

	for (; i < token.size(); ++i)
	{
		if (!std::isdigit(token[i]))
			return (false);
	}
	return (true);
}

void RPN::_applyOperator(const std::string &op)
{
	if (this->_stack.size() < 2)
		throw std::runtime_error("Error");
	double b = this->_stack.top();
	this->_stack.pop();
	double a = this->_stack.top();
	this->_stack.pop();

	double result;

	if (op == "+")
		result = a+ b;
	else if (op == "-")
		result = a - b;
	else if (op == "*")
		result = a * b;
	else
	{
		if (b == 0)
			throw std::runtime_error("Error");
		result = a / b;
	}
	this->_stack.push(result);
}


double RPN::evaluate(const std::string &expression)
{
	std::istringstream iss(expression);
	std::string	token;

	while (iss >> token)
	{
		if (this->_isNumber(token))
		{
			double value = std::atof(token.c_str());
			this->_stack.push(value);
		}
		else if (this->_isOperator(token))
			this->_applyOperator(token);
		else
			throw std::runtime_error("Error");
	}
	if (this->_stack.size() != 1)
		throw std::runtime_error("Error");
	double result = this->_stack.top();
	this->_stack.pop();

	return (result);
}
#pragma once

#include "IBeverage.h"
#include "Beverage.h"

enum CoffeeSize
{
	Standart,
	Double
};

// Кофе
class CCoffee : public CBeverage
{
public:
	CCoffee(const std::string& description = "Coffee", CoffeeSize size = CoffeeSize::Standart)
		:CBeverage(description),
		m_size(size)
	{
		SetDescription(GetCoffeeSizeDescription() + "Coffee");
	}

	double GetCost() const override
	{
		return 60;
	}

protected:
	std::string const GetCoffeeSizeDescription()
	{
		std::string sizeDescription;
		switch (m_size)
		{
		case CoffeeSize::Standart:
			sizeDescription = "Standart ";
			break;

		case CoffeeSize::Double:
			sizeDescription = "Double ";
			break;

		default:
			break;
		}

		return sizeDescription;
	}

	CoffeeSize m_size;
};

class CCapuccino : public CCoffee
{
public:
	CCapuccino(CoffeeSize size = CoffeeSize::Standart)
		:CCoffee("Capuccino", size)
	{}

	double GetCost() const override
	{
		return m_size == CoffeeSize::Standart ? 80 : 120;
	}
};

class CLatte : public CCoffee
{
public:
	CLatte(CoffeeSize size = CoffeeSize::Standart)
		:CCoffee("Latte", size)
	{}

	double GetCost() const override
	{
		return m_size == CoffeeSize::Standart ? 90 : 130;
	}
};

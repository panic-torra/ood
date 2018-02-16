#pragma once

#include "IBeverage.h"
#include "Beverage.h"

enum MilkshakeSize
{
	Small,
	Medium,
	Large
};

// Молочный коктейль
class CMilkshake : public CBeverage
{
public:
	CMilkshake(MilkshakeSize size = MilkshakeSize::Large)
		:CBeverage("Milkshake"),
		m_size(size)
	{
		SetDescription(GetMilkshakeSizeDescription() + "Milkshake");
	}

	double GetCost() const override
	{
		double cost = 80; //const for large size
		if (m_size == MilkshakeSize::Small)
		{
			cost = 50;
		}

		if (m_size == MilkshakeSize::Medium)
		{
			cost = 60;
		}

		return cost;
	}

private:
	std::string const GetMilkshakeSizeDescription()
	{
		std::string sizeDescription;
		switch (m_size)
		{
		case MilkshakeSize::Small:
			sizeDescription = "Small ";
			break;

		case MilkshakeSize::Medium:
			sizeDescription = "Medium ";
			break;

		case MilkshakeSize::Large:
			sizeDescription = "Large ";
			break;

		default:
			break;
		}

		return sizeDescription;
	}

	MilkshakeSize m_size;
};
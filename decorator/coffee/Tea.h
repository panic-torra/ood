#pragma once

#include "IBeverage.h"
#include "Beverage.h"

enum TeaGrade
{
	Green,
	Black,
	MilkyOolong,
	Carcade
};

class CTea : public CBeverage
{
public:
	CTea(TeaGrade grade = TeaGrade::Green)
		:CBeverage("Tea"),
		m_grade(grade)
	{
		SetDescription(GetTeaGradeDescription() + "Tea");
	}

	double GetCost() const override
	{
		return 30;
	}

private:
	std::string const GetTeaGradeDescription()
	{
		std::string gradeDescription;
		switch (m_grade)
		{
		case TeaGrade::Green:
			gradeDescription = "Green ";
			break;

		case TeaGrade::Black:
			gradeDescription = "Black ";
			break;

		case TeaGrade::MilkyOolong:
			gradeDescription = "Milky Oolong ";
			break;

		case TeaGrade::Carcade:
			gradeDescription = "Carcade ";
			break;

		default:
			break;
		}

		return gradeDescription;
	}

	TeaGrade m_grade;
};

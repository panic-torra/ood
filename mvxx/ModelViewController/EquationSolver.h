#pragma once

#include "Signals.h"
#include "FunctionType.h"

struct NoRealRoots {};
struct InfiniteNumberOfRoots {};

typedef boost::variant<NoRealRoots, InfiniteNumberOfRoots, double, std::pair<double, double>> EquationRoots;


class CEquationSolver
{
public:
	CEquationSolver();
	~CEquationSolver();

	typedef sig::signal<void()> SolutionChangeSignal;
	sig::connection DoOnSolutionChange(const SolutionChangeSignal::slot_type & handler);

	EquationRoots GetEquationRoots()const;
	std::vector<std::pair<double, double>> GetChartPoints()const;

	double GetQuadraticCoeff()const;
	void SetQuadraticCoeff(double a);

	double GetLinearCoeff()const;
	void SetLinearCoeff(double b);

	double GetConstantCoeff()const;
	void SetConstantCoeff(double c);

	int GetFrequency() const;
	int GetHorizontalLimit() const;

	void SetFunctionType(FunctionType newType);
	FunctionType GetFunctionType();
private:
	double m_a = 0;
	double m_b = 0;
	double m_c = 0;

	int m_frequency = 100;
	int m_horizontalLimit = 5;
	FunctionType m_function = FunctionType::SIN;

	SolutionChangeSignal m_solutionChanged;
};


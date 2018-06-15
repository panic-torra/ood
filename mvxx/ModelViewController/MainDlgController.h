#pragma once
#include "IMainDlgController.h"
#include "MainDlg.h"

class CEquationSolver;

class CMainDlgController :
	private IMainDlgController
{
public:
	CMainDlgController(CEquationSolver & solver, CWnd * parent = nullptr);
	CMainDlg & GetDialog();
	void ShowDialog();
private:
	void SetCoeffA(double a) override;
	void SetCoeffB(double b) override;
	void SetCoeffC(double c) override;
	void SetFunctionType(FunctionType newType) override;

	CMainDlg m_dlg;
	CEquationSolver & m_solver;
};


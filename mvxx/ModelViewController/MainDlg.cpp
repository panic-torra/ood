
// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVCApp.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "EquationSolver.h"
#include "MainDlgController.h"
#include "FunctionType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMainDlg::CMainDlg(CEquationSolver & solver, IMainDlgController & controller, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MODELVIEWCONTROLLER_DIALOG, pParent)
	, m_controller(controller)
	, m_solver(solver)
	, m_coeffA(solver.GetQuadraticCoeff())
	, m_coeffB(solver.GetLinearCoeff())
	, m_coeffC(solver.GetConstantCoeff())
	, m_drawArea()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CMainDlg::PreTranslateMessage(MSG * msg)
{
	if (msg->message == WM_KEYDOWN && msg->wParam == VK_RETURN)
	{
		auto focus = GetFocus();
		if (focus == GetDlgItem(IDC_COEFF_A))
		{
			OnChangeCoeffA();
			return TRUE;
		}
		else if (focus == GetDlgItem(IDC_COEFF_B))
		{
			OnChangeCoeffB();
			return TRUE;
		}
		else if (focus == GetDlgItem(IDC_COEFF_C))
		{
			OnChangeCoeffC();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(msg);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COEFF_A, m_coeffA);
	DDX_Text(pDX, IDC_COEFF_B, m_coeffB);
	DDX_Text(pDX, IDC_COEFF_C, m_coeffC);
	DDX_Control(pDX, IDC_GRAPH, m_drawArea);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_EN_KILLFOCUS(IDC_COEFF_A, &CMainDlg::OnKillfocusCoeffA)
	ON_EN_KILLFOCUS(IDC_COEFF_B, &CMainDlg::OnKillfocusCoeffB)
	ON_EN_KILLFOCUS(IDC_COEFF_C, &CMainDlg::OnKillfocusCoeffC)
	ON_BN_CLICKED(IDC_RADIO1, &CMainDlg::OnRadio1Clicked)
	ON_BN_CLICKED(IDC_RADIO2, &CMainDlg::OnRadio2Clicked)
END_MESSAGE_MAP()


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_solutionChangeConnection = m_solver.DoOnSolutionChange([=] {
		UpdateEquation();
	});

	UpdateEquation();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::SetSolutionText(const std::wstring & text)
{
	SetDlgItemText(IDC_SOLUTION, text.c_str());
}

void CMainDlg::SetEquationText(const std::wstring & text)
{
	SetDlgItemText(IDC_EQUATION, text.c_str());
}

void CMainDlg::DrawChart()
{
	CClientDC dc(&m_drawArea);
	CRect rc;
	m_drawArea.GetClientRect(&rc);
	int w = rc.Width();
	int h = rc.Height();

	int x_start = 10;
	int y_start = h / 2;

	double horizontalCoef = ((double)w - 2.0 * (double)x_start) / m_solver.GetHorizontalLimit();
	double verticalCoef = ((double)h / 2.0) / m_solver.GetQuadraticCoeff();

	CPen pnPenBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen * pOldPen = dc.SelectObject(&pnPenBlack);
	dc.FillSolidRect(rc, RGB(255, 255, 255));
	dc.MoveTo(x_start, y_start);
	dc.LineTo(x_start + w - 20, y_start);
	dc.MoveTo(x_start, 5);
	dc.LineTo(x_start, h - 5);

	CPen pnPenRed(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&pnPenRed);
	dc.MoveTo(x_start, y_start);

	std::vector<std::pair<double, double>> chartPoints = m_solver.GetChartPoints();
	for (auto point : chartPoints)
	{
		dc.LineTo((double)x_start + (point.first * horizontalCoef), (double)y_start + (point.second * verticalCoef));
	}

	dc.SelectObject(pOldPen);
}

void CMainDlg::UpdateEquation()
{
	auto FunctionToString = [](FunctionType function) {
		return ((function == FunctionType::SIN) ? L"sin" : L"cos");
	};

	SetEquationText((boost::wformat(L"%1%*%2%(%3%x + %4%)") % m_solver.GetQuadraticCoeff() % FunctionToString(m_solver.GetFunctionType()) % m_solver.GetLinearCoeff() % m_solver.GetConstantCoeff()).str());
	DrawChart();
}

void CMainDlg::OnChangeCoeffA()
{
	if (UpdateData())
	{
		m_controller.SetCoeffA(m_coeffA);
	}
}

void CMainDlg::OnChangeCoeffB()
{
	if (UpdateData())
	{
		m_controller.SetCoeffB(m_coeffB);
	}
}

void CMainDlg::OnChangeCoeffC()
{
	if (UpdateData())
	{
		m_controller.SetCoeffC(m_coeffC);
	}
}

void CMainDlg::OnOK()
{
}

void CMainDlg::OnKillfocusCoeffA()
{
	OnChangeCoeffA();
}

void CMainDlg::OnKillfocusCoeffB()
{
	OnChangeCoeffB();
}

void CMainDlg::OnKillfocusCoeffC()
{
	OnChangeCoeffC();
}

void CMainDlg::OnRadio1Clicked()
{
	if (UpdateData())
	{
		m_controller.SetFunctionType(FunctionType::SIN);
		UpdateEquation();
	}
}

void CMainDlg::OnRadio2Clicked()
{
	if (UpdateData())
	{
		m_controller.SetFunctionType(FunctionType::COS);
		UpdateEquation();
	}
}

// stdafx.cpp: �������� ����, ���������� ������ ����������� ���������� ������
// libeditor-tests.pch ����� ������������������� ����������
// stdafx.obj ����� ��������� �������������� ����������������� �������� � ����

#include "stdafx.h"

// �������� ���������� boost test, ��� ������� main ����� ������������� ���� (��� ���������������� ���������������� ����������), 
// � �� ������������� boost-�� �� ���
#define BOOST_TEST_NO_MAIN 
// ���������� ���������� boost test
#pragma warning (push, 3)
#pragma warning (disable:4702)
#include <boost/test/included/unit_test.hpp>
#pragma warning (pop)
// stdafx.cpp: �������� ����, ���������� ������ ����������� ���������� ������
// composite_test.pch ����� ������������������� ����������
// stdafx.obj ����� ��������� �������������� ����������������� �������� � ����

#include "stdafx.h"

#define BOOST_TEST_NO_MAIN 
// ���������� ���������� boost test
#pragma warning (push, 3)
#pragma warning (disable:4702)
#include <boost/test/included/unit_test.hpp>
#pragma warning (pop)

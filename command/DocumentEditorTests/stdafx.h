// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <sstream>

#define BOOST_TEST_INCLUDED

#pragma warning (push, 3)
#pragma warning (disable:4456 4702)
#include <boost/test/unit_test.hpp>
#pragma warning (pop)

#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>


// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
#include <memory>
#include <sstream>
#include <chrono>
#include <boost\filesystem.hpp>
#include <boost\format.hpp>
#include <boost\bind.hpp>
#include <list>
#include <algorithm>
#include <boost\regex.hpp>
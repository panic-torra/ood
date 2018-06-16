// stdafx.cpp: исходный файл, содержащий только стандартные включаемые модули
// libeditor-tests.pch будет предкомпилированным заголовком
// stdafx.obj будет содержать предварительно откомпилированные сведения о типе

#include "stdafx.h"

// Сообщаем библиотеке boost test, что функция main будет предоставлена нами (для предварительного конфигурирования библиотеки), 
// а не сгенерирована boost-ом за нас
#define BOOST_TEST_NO_MAIN 
// подключаем реализацию boost test
#pragma warning (push, 3)
#pragma warning (disable:4702)
#include <boost/test/included/unit_test.hpp>
#pragma warning (pop)
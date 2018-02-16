#include <iostream>
#include <string>
#include <functional>
#include "Beverage.h"
#include "Coffee.h"
#include "Tea.h"
#include "Milkshake.h"
#include "Condiments.h"

using namespace std;

struct MakeLemon
{
	MakeLemon(unsigned quantity)
		:m_quantity(quantity)
	{}

	auto operator()(IBeveragePtr && beverage)const
	{
		return make_unique<CLemon>(move(beverage), m_quantity); 
	}

private:
	unsigned m_quantity;
};

function<IBeveragePtr(IBeveragePtr &&)> MakeCinnamon()
{
	return [] (IBeveragePtr && b) {
		return make_unique<CCinnamon>(move(b));
	};
}

template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&...args)
{
	return [=](auto && b) {
		return make_unique<Condiment>(forward<decltype(b)>(b), args...);
	};
}

template <typename Component, typename Decorator>
auto operator << (Component && component, const Decorator & decorate)
{
	return decorate(forward<Component>(component));
}

void DialogWithUser()
{
	cout << "Type 1 for coffee or 2 for tea\n";
	int beverageChoice;
	cin >> beverageChoice;

	unique_ptr<IBeverage> beverage;

	if (beverageChoice == 1)
	{
		beverage = make_unique<CCoffee>();
	}
	else if (beverageChoice == 2)
	{
		beverage = make_unique<CTea>(TeaGrade::Carcade);
	}
	else
	{
		return;
	}

	int condimentChoice;
	for (;;)
	{
		cout << "1 - Lemon, 2 - Cinnamon, 0 - Checkout" << endl;
		cin >> condimentChoice;

		if (condimentChoice == 1)
		{
			beverage = move(beverage) << MakeCondiment<CLemon>(2);
		}
		else if (condimentChoice == 2)
		{
			beverage = move(beverage) << MakeCondiment<CCinnamon>();
		}
		else if (condimentChoice == 0)
		{
			break;
		}
		else
		{
			return;
		}
	}

	cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
}

int main()
{
	DialogWithUser();
	cout << endl;
	{
		auto latteDouble = make_unique<CLatte>(CoffeeSize::Double);
		auto cinnamon = make_unique<CCinnamon>(move(latteDouble));
		auto lemon = make_unique<CLemon>(move(cinnamon), 2);
		auto iceCubes = make_unique<CIceCubes>(move(lemon), 2, IceCubeType::Dry);
		auto beverage = make_unique<CChocolateCrumbs>(move(iceCubes), 2);

		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	{
		auto beverage =
			make_unique<CChocolateCrumbs>(			// Внешний слой: шоколадная крошка
				make_unique<CIceCubes>(				// | под нею - кубики льда
					make_unique<CLemon>(			// | | еще ниже лимон
						make_unique<CCinnamon>(		// | | | слоем ниже - корица
							make_unique<CLatte>()),	// | | |   в самом сердце - Латте
						2),							// | | 2 дольки лимона
					2, IceCubeType::Dry),			// | 2 кубика сухого льда
				2);									// 2 грамма шоколадной крошки

		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	{
		auto lemon2 = MakeCondiment<CLemon>(2);
		auto iceCubes3 = MakeCondiment<CIceCubes>(3, IceCubeType::Water);
		
		auto tea = make_unique<CTea>();

		auto lemonIceTea = iceCubes3(lemon2(move(tea)));	
		auto oneMoreLemonIceTea =
			make_unique<CTea>()	// Берем чай
			<< MakeCondiment<CLemon>(2)	// добавляем пару долек лимона
			<< MakeCondiment<CIceCubes>(3, IceCubeType::Water); // и 3 кубика льда
	}

	{
		auto lemon2 = MakeCondiment<CLemon>(2);
		auto iceCubes3 = MakeCondiment<CIceCubes>(3, IceCubeType::Water);

		auto tea = make_unique<CTea>(TeaGrade::MilkyOolong);

		auto lemonIceTea = iceCubes3(lemon2(move(tea)));
		auto oneMoreLemonIceTea =
			make_unique<CTea>()	// Берем чай
			<< MakeCondiment<CLemon>(2)	// добавляем пару долек лимона
			<< MakeCondiment<CIceCubes>(3, IceCubeType::Water); // и 3 кубика льда
	}

	{
		auto beverage = 
			make_unique<CLatte>()							// Наливаем чашечку латте,
			<< MakeCondiment<CCinnamon>()					// оборачиваем корицей,
			<< MakeCondiment<CLemon>(2)						// добавляем пару долек лимона
			<< MakeCondiment<CIceCubes>(2, IceCubeType::Dry)// брасаем пару кубиков сухого льда
			<< MakeCondiment<CChocolateCrumbs>(2);			// посыпаем шоколадной крошкой

		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	{
		auto beverage = 
			make_unique<CMilkshake>()					// Наливаем молочный коктейль
			<< MakeCondiment<CSyrup>(SyrupType::Maple)	// заливаем кленовым сиропом
			<< MakeCondiment<CCoconutFlakes>(8);		// посыпаем кокосовой стружкой

		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}
}

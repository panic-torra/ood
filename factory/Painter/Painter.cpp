// Painter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../libpainter/Designer.h"
#include "../libpainter/Painter.h"
#include "../libpainter/ShapeFactory.h"
#include "../libpainter/PictureDraft.h"
#include "../libpainter/Canvas.h"

using namespace std;

void PrintInfo()
{
	cout << "Rectangle <color> <left top point> <right bottom point>" << endl;
	cout << "Triangle <color> <point1> <point2> <point3>" << endl;
	cout << "Ellipse <color> <center point> <horizontal radius> <vertical radius>" << endl;
	cout << "RegularPolygon <color> <center point> <radius> <vertex count>" << endl;
}

int main()
{
	PrintInfo();
	CShapeFactory shapeFactory;
	CDesigner designer(shapeFactory);

	CCanvas canvas(std::cout);
	CPainter painter;
	CPictureDraft draft;

	stringstream stream;
	stream << "polygon pink 5 5 10 6";
	stream << "rectangle black 1 1 4 4";

	try
	{
		draft = designer.CreateDraft(stream);
		unique_ptr<CShape> newPolygon = shapeFactory.CreateShape("polygon green 100 100 70 5");

		draft.AddShape(move(newPolygon));
		painter.DrawPicture(draft, canvas);
	}
	catch (exception & e)
	{
		cout << e.what() << endl;
	}

	//painter.DrawPicture(designer.CreateDraft(std::cin), canvas);

	return EXIT_SUCCESS;
}


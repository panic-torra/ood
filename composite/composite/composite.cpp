#include "stdafx.h"
#include "CommonTypes.h"
#include "SVGCanvas.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Ellipse.h"
#include "Style.h"
#include "GroupShape.h"
#include "Slide.h"

using namespace std;

const int RED_COLOR = 14296871;
const int GREEN_COLOR = 10930456;
const int YELLOW_COLOR = 15657729;
const int BLUE_COLOR = 1402838;

shared_ptr<CGroupShape> CreateCar(CSVGCanvas & canvas);
void ResizeCarFrame(shared_ptr<CGroupShape> & car, CSVGCanvas & canvas);
void UpdateCarFrameStyle(shared_ptr<CGroupShape> & car, CSVGCanvas & canvas);
void CreateSlideWithCar(shared_ptr<CGroupShape> & car, CSVGCanvas & canvas);

int main()
{
	string canvasFileName = "../canvas.svg";

	try
	{
		ofstream canvasFile(canvasFileName.c_str());
		if (!canvasFile.is_open())
		{
			throw runtime_error("Error while opening file for canvas.");
		}

		CSVGCanvas canvas(canvasFile);

		auto car = CreateCar(canvas);
		ResizeCarFrame(car, canvas);
		UpdateCarFrameStyle(car, canvas);
		CreateSlideWithCar(car, canvas);
	}
	catch (exception & e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

shared_ptr<CGroupShape> CreateCar(CSVGCanvas & canvas)
{
	auto wheelOne = make_shared<CEllipse>(Vertex{ 150, 170 }, 30, 30);
	wheelOne->GetFillStyle()->SetColor(GREEN_COLOR);

	auto wheelTwo = make_shared<CEllipse>(Vertex{ 380, 170 }, 30, 30);
	wheelTwo->GetFillStyle()->SetColor(YELLOW_COLOR);

	auto carBody = make_shared<CRectangle>(Vertex{ 150, 70 }, Vertex{ 380, 140 });
	carBody->GetFillStyle()->SetColor(RED_COLOR);
	carBody->GetOutlineStyle()->SetColor(BLUE_COLOR);

	auto car = make_shared<CGroupShape>();
	car->InsertShape(carBody);
	car->InsertShape(wheelOne);
	car->InsertShape(wheelTwo);
	car->Draw(canvas);

	return car;
}

void ResizeCarFrame(shared_ptr<CGroupShape> & car, CSVGCanvas & canvas)
{
	RectangleDouble newFrame = { 500, 50, 250, 20 };

	car->SetFrame(newFrame);
	car->Draw(canvas);
}

void UpdateCarFrameStyle(shared_ptr<CGroupShape> & car, CSVGCanvas & canvas)
{
	RectangleDouble newFrame = { 900, 50, 170, 220 };
	car->SetFrame(newFrame);
	car->GetFillStyle()->SetColor(YELLOW_COLOR);
	car->GetOutlineStyle()->SetColor(BLUE_COLOR);
}

void CreateSlideWithCar(shared_ptr<CGroupShape> & car, CSVGCanvas & canvas)
{
	CSlide slide;
	slide.InsertShape(car);
	slide.Draw(canvas);
}
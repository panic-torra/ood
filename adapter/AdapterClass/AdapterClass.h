#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdint>

using namespace std;

// ������������ ���� ����������� ���������� (���������� ��� ���������)
namespace graphics_lib
{
	// ����� ��� ���������
	class ICanvas
	{
	public:
		// ������ "����" � ����� x, y
		virtual void MoveTo(int x, int y) = 0;
		// ������ ����� � ������� �������, ���������� ���� � ����� x,y 
		virtual void LineTo(int x, int y) = 0;
		virtual ~ICanvas() = default;
	};

	// ���������� ������ ��� ���������
	class CCanvas : public ICanvas
	{
	public:
		void MoveTo(int x, int y) override
		{
			cout << "MoveTo (" << x << ", " << y << ")" << endl;
		}
		void LineTo(int x, int y) override
		{
			cout << "LineTo (" << x << ", " << y << ")" << endl;
		}
	};
}

// ������������ ���� ���������� ��� ��������� ����� (���������� graphics_lib)
// ��� ���������� ���������� ��� ���������
namespace shape_drawing_lib
{
	struct Point
	{
		int x;
		int y;
	};

	// ��������� ��������, ������� ����� ���� ���������� �� ������ �� graphics_lib
	class ICanvasDrawable
	{
	public:
		virtual void Draw(graphics_lib::ICanvas & canvas)const = 0;
		virtual ~ICanvasDrawable() = default;
	};

	class CTriangle : public ICanvasDrawable
	{
	public:
		CTriangle(const Point & p1, const Point & p2, const Point & p3)
			: m_point1(p1)
			, m_point2(p2)
			, m_point3(p3)
		{
		}

		void Draw(graphics_lib::ICanvas & canvas)const override
		{
			canvas.MoveTo(m_point1.x, m_point1.y);

			canvas.LineTo(m_point2.x, m_point2.y);
			canvas.LineTo(m_point3.x, m_point3.y);
			canvas.LineTo(m_point1.x, m_point1.y);
		}
	private:
		Point m_point1;
		Point m_point2;
		Point m_point3;
	};

	class CRectangle : public ICanvasDrawable
	{
	public:
		CRectangle(const Point & leftTop, int width, int height)
			: m_leftTop(leftTop)
			, m_width(width)
			, m_height(height)
		{
		}

		void Draw(graphics_lib::ICanvas & canvas)const override
		{
			canvas.MoveTo(m_leftTop.x, m_leftTop.y);

			canvas.LineTo(m_leftTop.x + m_width, m_leftTop.y);
			canvas.LineTo(m_leftTop.x + m_width, m_leftTop.y + m_height);
			canvas.LineTo(m_leftTop.x, m_leftTop.y + m_height);
			canvas.LineTo(m_leftTop.x, m_leftTop.y);
		}
	private:
		Point m_leftTop;
		int m_width;
		int m_height;
	};

	// ��������, ��������� �������� ICanvasDrawable-������� �� ICanvas
	class CCanvasPainter
	{
	public:
		CCanvasPainter(graphics_lib::ICanvas & canvas)
			: m_canvas(canvas)
		{
		}
		void Draw(const ICanvasDrawable & drawable)
		{
			drawable.Draw(m_canvas);
		}
	private:
		graphics_lib::ICanvas & m_canvas;
	};
}

// ������������ ���� ����������� ����������� ���������� (���������� ��� ���������)
namespace modern_graphics_lib
{
	class CPoint
	{
	public:
		CPoint(int x, int y) :x(x), y(y) {}

		int x;
		int y;
	};

	// ����� ��� ������������ ��������� �������
	class CModernGraphicsRenderer
	{
	public:
		CModernGraphicsRenderer(ostream & strm) : m_out(strm)
		{
		}

		~CModernGraphicsRenderer()
		{
			if (m_drawing) // ��������� ���������, ���� ��� ���� ������
			{
				EndDraw();
			}
		}

		// ���� ����� ������ ���� ������ � ������ ���������
		void BeginDraw()
		{
			if (m_drawing)
			{
				throw logic_error("Drawing has already begun");
			}
			m_out << "<draw>" << endl;
			m_drawing = true;
		}

		// ��������� ��������� �����
		void DrawLine(const CPoint & start, const CPoint & end)
		{
			if (!m_drawing)
			{
				throw logic_error("DrawLine is allowed between BeginDraw()/EndDraw() only");
			}
			
			m_out << "(<line fromX=" << start.x << " fromY=" << start.y << " toX=" << end.x << " toY=" << end.y << "/>)" << std::endl;
		}

		// ���� ����� ������ ���� ������ � ����� ���������
		void EndDraw()
		{
			if (!m_drawing)
			{
				throw logic_error("Drawing has not been started");
			}
			m_out << "</draw>" << endl;
			m_drawing = false;
		}
	private:
		ostream & m_out;
		bool m_drawing = false;
	};
}

// ������������ ���� ���������� (�������� ��� �����������)
namespace app
{
	class CModernGraphicsRendererAdapter : public graphics_lib::ICanvas, modern_graphics_lib::CModernGraphicsRenderer
	{
	public:
		CModernGraphicsRendererAdapter(ostream & strm)
			: modern_graphics_lib::CModernGraphicsRenderer(strm)
			, m_currentPoint({ 0, 0 })
		{
			BeginDraw();
		}

		void MoveTo(int x, int y) override
		{
			m_currentPoint = { x, y };
		}

		void LineTo(int x, int y) override
		{
			DrawLine(m_currentPoint, { x, y });
			MoveTo(x, y);
		}
	private:
		modern_graphics_lib::CPoint m_currentPoint;
	};


	void PaintPicture(shape_drawing_lib::CCanvasPainter & painter)
	{
		using namespace shape_drawing_lib;

		CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 });
		CRectangle rectangle({ 30, 40 }, 18, 24);

		painter.Draw(triangle);
		painter.Draw(rectangle);
	}

	void PaintPictureOnCanvas()
	{
		graphics_lib::CCanvas simpleCanvas;
		shape_drawing_lib::CCanvasPainter painter(simpleCanvas);
		PaintPicture(painter);
	}

	void PaintPictureOnModernGraphicsRenderer()
	{
		CModernGraphicsRendererAdapter graphicsRendererAdapter(std::cout);
		shape_drawing_lib::CCanvasPainter painter(graphicsRendererAdapter);
		PaintPicture(painter);
	}
}

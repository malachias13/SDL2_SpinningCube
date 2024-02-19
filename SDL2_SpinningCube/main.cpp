#include "Screen.h"
#undef main
#include <numeric>

struct vec3
{
	float x = 0, y = 0, z = 0;
};

struct Float4
{
	union             // total size of this union is only 16 bytes (4 floats)
	{
		struct
		{
			float V[4];
		};
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};

struct Matrix4x4
{
	union
	{
		struct
		{
			float  V[16];
		};
		struct
		{
			float  xx;
			float  xy;
			float  xz;
			float  xw;
			float  yx;
			float  yy;
			float  yz;
			float  yw;
			float  zx;
			float  zy;
			float  zz;
			float  zw;
			float  wx;
			float  wy;
			float  wz;
			float  ww;
		};
		struct
		{
			Float4 AxisX;
			Float4 AxisY;
			Float4 AxisZ;
			Float4 AxisW;
		};
	};
};

inline Float4 operator *(const Float4& v, const Matrix4x4& M) {
	Float4 value;
	value.x = v.x * M.AxisX.x + v.y * M.AxisY.x + v.z * M.AxisZ.x + v.w * M.AxisW.x;
	value.y = v.x * M.AxisX.y + v.y * M.AxisY.y + v.z * M.AxisZ.y + v.w * M.AxisW.y;
	value.z = v.x * M.AxisX.z + v.y * M.AxisY.z + v.z * M.AxisZ.z + v.w * M.AxisW.z;
	value.w = v.x * M.AxisX.w + v.y * M.AxisY.w + v.z * M.AxisZ.w + v.w * M.AxisW.w;
	return value;
}

struct connection
{
	float start = 0, end = 0;
};

void rotate(vec3& point, float x = 1, float y = 1, float z = 1)
{
	float rad = 0;
	
	rad = x;
	point.y = cos(rad) * point.y - sin(rad) * point.z;
	point.z = sin(rad) * point.y + cos(rad) * point.z;

	rad = y;
	point.x = cos(rad) * point.x + sin(rad) * point.z;
	point.z = -sin(rad) * point.x + cos(rad) * point.z;

	rad = z;
	point.x = cos(rad) * point.x - sin(rad) * point.y;
	point.y = sin(rad) * point.x + cos(rad) * point.y;
}

void translate(vec3& point, float x = 1, float y = 1, float z = 1)
{
	Float4 position{x,y,z,1};

	Matrix4x4 value = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	point.x,point.y,point.z,1
	};
	Float4 result = position * value;
	point.x = result.x;
	point.y = result.y;
	point.z = result.z;
}

void scale(vec3& point, float x = 1, float y = 1, float z = 1)
{
	point.x *= x;
	point.y *= y;
	point.z *= z;
}

void line(Screen& screen, float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	float length = sqrt(dx * dx + dy * dy);
	float angle = atan2f(dy, dx);

	for (float i = 0; i < length; i++)
	{
		screen.pixel(x1 + cos(angle) * i, y1 + sin(angle) * i);
	}
}

int main()
{
	Screen screen;

	std::vector<vec3> points{
		{100,100,100},
		{200,100,100},
		{200,200,100},
		{100,200,100},

		{100,100,200},
		{200,100,200},
		{200,200,200},
		{100,200,200}
	};

	std::vector<connection> connections
	{
		{0,4},
		{1,5},
		{2,6},
		{3,7},
		
		// front face
		{0,1},
		{1,2},
		{2,3},
		{3,0},

		// back face
		{4,5},
		{5,6},
		{6,7},
		{7,4}
	};


	/* Calculate of cube centroid */
	vec3 centeroid;
	for (auto& p : points)
	{
		centeroid.x += p.x;
		centeroid.y += p.y;
		centeroid.z += p.z;
	}
	centeroid.x /= points.size();
	centeroid.y /= points.size();
	centeroid.z /= points.size();


	// scale and translate cube.
	for (auto& p : points)
	{

		p.x -= centeroid.x;
		p.y -= centeroid.y;
		p.z -= centeroid.z;

		scale(p, 2, 2, 2);
		translate(p, (screen.getWidth() / 2), (screen.getHeight() / 2), 0);

	}

	/* Calculate of cube centroid */
	for (auto& p : points)
	{
		centeroid.x += p.x;
		centeroid.y += p.y;
		centeroid.z += p.z;
	}
	centeroid.x /= points.size();
	centeroid.y /= points.size();
	centeroid.z /= points.size();


	while (true)
	{
		for (auto& p : points)
		{
			p.x -= centeroid.x;
			p.y -= centeroid.y;
			p.z -= centeroid.z;
			rotate(p, 0.005, 0.002, 0.008);
			p.x += centeroid.x;
			p.y += centeroid.y;
			p.z += centeroid.z;


			screen.pixel(p.x, p.y);
		}

		for (auto& conn : connections)
		{
			line(screen, points[conn.start].x, points[conn.start].y,
				points[conn.end].x, points[conn.end].y);
		}

		screen.show();
		screen.clear();
		screen.inputHandle();
		SDL_Delay(30);
	}

	return 0;
}
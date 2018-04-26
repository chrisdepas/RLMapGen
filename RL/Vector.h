#pragma once 
struct Vector2f
{
	float X;
	float Y;

	Vector2f(){}
	Vector2f(float X, float Y);
	float Length();

	Vector2f operator/(float f);
	void operator+=(Vector2f c);
	void operator*=(float f);
	Vector2f operator*(float f);
	Vector2f operator*(int i);
};

struct Vector2i
{
	int X;
	int Y;

	float Length();
	Vector2i(){}
	Vector2i(int X, int Y);
	Vector2i operator/(int factor);
	Vector2i operator-(Vector2i c);
	Vector2i operator+(Vector2i c);
	Vector2i operator*(int factor);
	bool operator==(Vector2i c);
	bool operator!=(Vector2i c);
};

struct Vector3f
{
	float X;
	float Y;
	float Z;

	float Length();
};

struct Vector3i
{
	int X;
	int Y;
	int Z;
};

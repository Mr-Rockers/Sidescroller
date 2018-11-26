#ifndef EXTRAMATHS_HELPER_H
#define EXTRAMATHS_HELPER_H

/*GEOMETRY*/
struct Vec2 {
	float x;
	float y;
	Vec2(float x, float y) : x(x), y(y) {}
	Vec2(float val) : Vec2(val, val) {}
	Vec2() : Vec2(0.0f, 0.0f) {}

	Vec2 operator+ (const Vec2& rhs) {
		return Vec2(this->x + rhs.x, this->y + rhs.y);
	}
	Vec2 operator+ (const float& rhs) {
		return Vec2(this->x + rhs, this->y + rhs);
	}
	Vec2 operator- (const Vec2& rhs) {
		return Vec2(this->x - rhs.x, this->y - rhs.y);
	}
	Vec2 operator- (const float& rhs) {
		return Vec2(this->x - rhs, this->y - rhs);
	}
	Vec2 operator* (const Vec2& rhs) {
		return Vec2(this->x * rhs.x, this->y * rhs.y);
	}
	Vec2 operator* (const float& rhs) {
		return Vec2(this->x * rhs, this->y * rhs);
	}
	Vec2 operator/ (const Vec2& rhs) {
		return Vec2(this->x / rhs.x, this->y / rhs.y);
	}
	Vec2 operator/ (const float& rhs) {
		return Vec2(this->x / rhs, this->y / rhs);
	}
	Vec2 operator^ (const float& rhs) {
		return Vec2(powf(this->x, rhs), powf(this->y, rhs));
	}
	Vec2 &operator+= (const Vec2& rhs) {
		*this = *this + rhs; return *this;
	}
	Vec2 &operator+= (const float& rhs) {
		*this = *this + rhs; return *this;
	}
	Vec2 &operator-= (const Vec2& rhs) {
		*this = *this - rhs; return *this;
	}
	Vec2 &operator-= (const float& rhs) {
		*this = *this - rhs; return *this;
	}
	Vec2 &operator*= (const Vec2& rhs) {
		*this = *this * rhs; return *this;
	}
	Vec2 &operator*= (const float& rhs) {
		*this = *this * rhs; return *this;
	}
	Vec2 &operator/= (const Vec2& rhs) {
		*this = *this / rhs; return *this;
	}
	Vec2 &operator/= (const float& rhs) {
		*this = *this / rhs; return *this;
	}
	Vec2 &operator^= (const float& rhs) {
		*this = *this ^ rhs;
		return *this;
	}
	/*Vec2 &operator= (const float& rhs) {
		*this = Vec2(rhs, rhs);
		return *this;
	}*/
};
static Vec2* LineIntersection(Vec2 line1A, Vec2 line1B, Vec2 line2A, Vec2 line2B, bool vectorCheck) {

	float a1 = line1B.y - line1A.y;
	float b1 = line1A.x - line1B.x;
	float c1 = a1 * (line1A.x) + b1 * (line1A.y);

	float a2 = line2B.y - line2A.y;
	float b2 = line2A.x - line2B.x;
	float c2 = a2 * (line2A.x) + b2 * (line2A.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0) {
		return nullptr;
	}
	else {

		float xIntersect = (b2 * c1 - b1 * c2) / determinant;
		float yIntersect = (a1 * c2 - a2 * c1) / determinant;

		float line1Mag = sqrtf(powf(line1B.x - line1A.x, 2) + powf(line1B.y - line1A.y, 2));
		float line2Mag = sqrtf(powf(line2B.x - line2A.x, 2) + powf(line2B.y - line2A.y, 2));

		float line1IntersectMag = sqrtf(powf(xIntersect - line1A.x, 2) + powf(yIntersect - line1A.y, 2));
		float line2IntersectMag = sqrtf(powf(xIntersect - line2A.x, 2) + powf(yIntersect - line2A.y, 2));

		if (vectorCheck && ((line1IntersectMag > line1Mag) || (line2IntersectMag > line2Mag))) {
			return nullptr;
		}

		return new Vec2(xIntersect, yIntersect);
	}
}
static Vec2 Normalise(Vec2 vector) {
	float mag = sqrt(powf(vector.x, 2.0f) + powf(vector.y, 2.0f));
	return vector / mag;
}
static const float DotProduct(Vec2 &a, Vec2 &b) {
	return (a.x * b.x) + (a.y * b.y);
}
struct AABB {
	Vec2 centre;
	Vec2 halfDimensions;
	AABB(Vec2 centre, Vec2 halfDimensions) : centre(centre), halfDimensions(halfDimensions) {}

	float getWidth() {
		return halfDimensions.x * 2.0f;
	}
	float getHeight() {
		return halfDimensions.y * 2.0f;
	}

	float getLeft() {
		return centre.x - halfDimensions.x;
	}
	float getRight() {
		return centre.x + halfDimensions.x;
	}
	float getBottom() {
		return centre.y - halfDimensions.y;
	}
	float getTop() {
		return centre.y + halfDimensions.y;
	}

	static bool Intersect(AABB a, AABB b) {
		if (a.getRight() < b.getLeft() || a.getLeft() > b.getRight()) return false;
		if (a.getTop() < b.getBottom() || a.getBottom() > b.getTop()) return false;
		return true;
	}
};

/*GENERAL*/

//Linear Interpolation.
template <typename T>
static T Lerp(T start, T end, float percent) {
	return (1.0f - percent) * start + percent * end;
}

template <typename T>
static void ChangeToValue(T &val, T end, T deltaTime) {
	val =
		val > end ? Max(val - deltaTime, end) :
		val < end ? Min(val + deltaTime, end) :
		end;
}

template <typename T>
static void ReduceToZero(T &val, T deltaTime) {
	ChangeToValue(val, (T)0, deltaTime);
}

template <typename T>
static T Max(T a, T b) {
	return a > b ? a : b;
}
template <typename T>
static T Min(T a, T b) {
	return a < b ? a : b;
}


#endif
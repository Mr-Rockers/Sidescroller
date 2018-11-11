#ifndef EXTRAMATHS_HELPER_H
#define EXTRAMATHS_HELPER_H

/*GENERAL*/
template <typename T>
static T Lerp(T start, T end, float percent) {
	return (start + end * percent*(end - start));
}

/*GEOMETRY*/
struct Vec2 {
	float x;
	float y;
	Vec2(float x, float y) {}
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
	Vec2 &operator= (const float& rhs) {
		*this = Vec2(rhs, rhs);
		return *this;
	}
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
static const float DotProduct(Vec2 &a, Vec2 &b) {
	return (a.x * b.x) + (a.y * b.y);
}

#endif
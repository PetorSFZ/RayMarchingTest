#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "sfz/math/Vector.hpp"

#include <unordered_map>
#include <type_traits>

TEST_CASE("Vector<T,2> specialization", "[sfz::Vector]")
{
	sfz::Vector<int,2> v;
	SECTION("Data") {
		REQUIRE(sizeof(sfz::Vector<int,2>) == sizeof(int)*2);
		REQUIRE(sizeof(v.elements) == sizeof(int)*2);
		v.elements[0] = 1;
		v.elements[1] = 2;
		REQUIRE(v.x == 1);
		REQUIRE(v.y == 2);
	}
	SECTION("Array pointer constructor") {
		int arr[] = {1, 2, 3};
		sfz::Vector<int,2> v1{arr};
		sfz::Vector<int,2> v2{arr+1};
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v2[0] == 2);
		REQUIRE(v2[1] == 3);
	}
	SECTION("Fill constructor") {
		sfz::Vector<int,2> v1{3};
		REQUIRE(v1.x == 3);
		REQUIRE(v1.y == 3);
	}
	SECTION("Constructor (x, y)") {
		sfz::Vector<int,2> v1{3, -1};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
	}
	SECTION("Access [] operator") {
		v[0] = 4;
		v[1] = -2;
		REQUIRE(v[0] == 4);
		REQUIRE(v[1] == -2);
	}
}

TEST_CASE("Vector<T,3> specialization", "[sfz::Vector]")
{
	sfz::Vector<int,3> v;
	SECTION("Data") {
		REQUIRE(sizeof(sfz::Vector<int,3>) == sizeof(int)*3);
		REQUIRE(sizeof(v.elements) == sizeof(int)*3);
		v.elements[0] = 1;
		v.elements[1] = 2;
		v.elements[2] = 3;
		REQUIRE(v.x == 1);
		REQUIRE(v.y == 2);
		REQUIRE(v.z == 3);
		REQUIRE(v.xy == (sfz::Vector<int,2>{1, 2}));
		REQUIRE(v.yz == (sfz::Vector<int,2>{2, 3}));
	}
	SECTION("Array pointer constructor") {
		int arr[] = {1, 2, 3, 4};
		sfz::Vector<int,3> v1{arr};
		sfz::Vector<int,3> v2{arr+1};
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 3);
		REQUIRE(v2[0] == 2);
		REQUIRE(v2[1] == 3);
		REQUIRE(v2[2] == 4);
	}
	SECTION("Fill constructor") {
		sfz::Vector<int,3> v1{3};
		REQUIRE(v1.x == 3);
		REQUIRE(v1.y == 3);
		REQUIRE(v1.z == 3);
	}
	SECTION("Constructor (x, y, z)") {
		sfz::Vector<int,3> v1{3, -1, -2};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
	}
	SECTION("Constructor (xy, z)") {
		sfz::Vector<int,3> v1{sfz::Vector<int,2>{3, -1}, -2};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
	}
	SECTION("Constructor (x, yz)") {
		sfz::Vector<int,3> v1{3, sfz::Vector<int,2>{-1, -2}};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
	}
	SECTION("Access [] operator") {
		v[0] = 4;
		v[1] = -2;
		v[2] = 1;
		REQUIRE(v[0] == 4);
		REQUIRE(v[1] == -2);
		REQUIRE(v[2] == 1);
	}
}

TEST_CASE("Vector<T,4> specialization", "[sfz::Vector]")
{
	sfz::Vector<int,4> v;
	SECTION("Data") {
		REQUIRE(sizeof(sfz::Vector<int,4>) == sizeof(int)*4);
		REQUIRE(sizeof(v.elements) == sizeof(int)*4);
		v.elements[0] = 1;
		v.elements[1] = 2;
		v.elements[2] = 3;
		v.elements[3] = 4;
		REQUIRE(v.x == 1);
		REQUIRE(v.y == 2);
		REQUIRE(v.z == 3);
		REQUIRE(v.w == 4);
		REQUIRE(v.xyz == (sfz::Vector<int,3>{1, 2, 3}));
		REQUIRE(v.yzw == (sfz::Vector<int,3>{2, 3, 4}));
		REQUIRE(v.xy == (sfz::Vector<int,2>{1, 2}));
		REQUIRE(v.zw == (sfz::Vector<int,2>{3, 4}));
		REQUIRE(v.yz == (sfz::Vector<int,2>{2, 3}));
	}
	SECTION("Array pointer constructor") {
		int arr[] = {1, 2, 3, 4, 5};
		sfz::Vector<int,4> v1{arr};
		sfz::Vector<int,4> v2{arr+1};
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 3);
		REQUIRE(v1[3] == 4);
		REQUIRE(v2[0] == 2);
		REQUIRE(v2[1] == 3);
		REQUIRE(v2[2] == 4);
		REQUIRE(v2[3] == 5);
	}
	SECTION("Fill constructor") {
		sfz::Vector<int,4> v1{3};
		REQUIRE(v1.x == 3);
		REQUIRE(v1.y == 3);
		REQUIRE(v1.z == 3);
		REQUIRE(v1.w == 3);
	}
	SECTION("Constructor (x, y, z, w)") {
		sfz::Vector<int,4> v1{3, -1, -2, 9};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
		REQUIRE(v1[3] == 9);
	}
	SECTION("Constructor (xyz, w)") {
		sfz::Vector<int,4> v1{sfz::Vector<int,3>{3, -1, -2}, 9};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
		REQUIRE(v1[3] == 9);
	}
	SECTION("Constructor (x, yzw)") {
		sfz::Vector<int,4> v1{3, sfz::Vector<int,3>{-1, -2, 9}};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
		REQUIRE(v1[3] == 9);
	}
	SECTION("Constructor (xy, zw)") {
		sfz::Vector<int,4> v1{sfz::Vector<int,2>{3, -1}, sfz::Vector<int,2>{-2, 9}};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
		REQUIRE(v1[3] == 9);
	}
	SECTION("Constructor (xy, z, w)") {
		sfz::Vector<int,4> v1{sfz::Vector<int,2>{3, -1}, -2, 9};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
		REQUIRE(v1[3] == 9);
	}
	SECTION("Constructor (x, yz, w)") {
		sfz::Vector<int,4> v1{3, sfz::Vector<int,2>{-1, -2}, 9};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
		REQUIRE(v1[3] == 9);
	}
	SECTION("Constructor (x, y, zw)") {
		sfz::Vector<int,4> v1{3, -1, sfz::Vector<int,2>{-2, 9}};
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -1);
		REQUIRE(v1[2] == -2);
		REQUIRE(v1[3] == 9);
	}
	SECTION("Access [] operator") {
		v[0] = 4;
		v[1] = -2;
		v[2] = 1;
		v[3] = 9;
		REQUIRE(v[0] == 4);
		REQUIRE(v[1] == -2);
		REQUIRE(v[2] == 1);
		REQUIRE(v[3] == 9);
	}
}

TEST_CASE("Vector<T,N> general definition", "[sfz::Vector]")
{
	sfz::Vector<int,5> v;
	SECTION("Data") {
		REQUIRE(sizeof(sfz::Vector<int,5>) == sizeof(int)*5);
		REQUIRE(sizeof(v.elements) == sizeof(int)*5);
	}
	SECTION("Array pointer constructor") {
		int arr[] = {1, 2, 3, 4, 5, 6};
		sfz::Vector<int,5> v1{arr};
		sfz::Vector<int,5> v2{arr+1};
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 3);
		REQUIRE(v1[3] == 4);
		REQUIRE(v1[4] == 5);
		REQUIRE(v2[0] == 2);
		REQUIRE(v2[1] == 3);
		REQUIRE(v2[2] == 4);
		REQUIRE(v2[3] == 5);
		REQUIRE(v2[4] == 6);
	}
	SECTION("Access [] operator") {
		v[0] = 4;
		v[1] = -2;
		v[2] = 1;
		v[3] = 27;
		v[4] = -9;
		REQUIRE(v[0] == 4);
		REQUIRE(v[1] == -2);
		REQUIRE(v[2] == 1);
		REQUIRE(v[3] == 27);
		REQUIRE(v[4] == -9);
	}
}

TEST_CASE("Arithmetic operators", "[sfz::Vector]")
{
	sfz::Vector<int, 3> v1{1, -2, 5};
	sfz::Vector<int, 3> v2{0, -2, 1};

	// Integrity check of base vectors
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == -2);
	REQUIRE(v1[2] == 5);
	REQUIRE(v2[0] == 0);
	REQUIRE(v2[1] == -2);
	REQUIRE(v2[2] == 1);

	SECTION("Addition") {
		auto v3 = v1 + v2;
		REQUIRE(v3[0] == 1);
		REQUIRE(v3[1] == -4);
		REQUIRE(v3[2] == 6);
		// Integrity check of base vectors
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == -2);
		REQUIRE(v1[2] == 5);
		REQUIRE(v2[0] == 0);
		REQUIRE(v2[1] == -2);
		REQUIRE(v2[2] == 1);
	}
	SECTION("Subtraction") {
		auto v3 = v1 - v2;
		REQUIRE(v3[0] == 1);
		REQUIRE(v3[1] == 0);
		REQUIRE(v3[2] == 4);
		auto v4 = v2 - v1;
		REQUIRE(v4[0] == -1);
		REQUIRE(v4[1] == 0);
		REQUIRE(v4[2] == -4);
		// Integrity check of base vectors
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == -2);
		REQUIRE(v1[2] == 5);
		REQUIRE(v2[0] == 0);
		REQUIRE(v2[1] == -2);
		REQUIRE(v2[2] == 1);
	}
	SECTION("Negating (-x)") {
		auto v3 = -v1;
		REQUIRE(v3[0] == -1);
		REQUIRE(v3[1] == 2);
		REQUIRE(v3[2] == -5);
		// Integrity check of base vectors
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == -2);
		REQUIRE(v1[2] == 5);
		REQUIRE(v2[0] == 0);
		REQUIRE(v2[1] == -2);
		REQUIRE(v2[2] == 1);
	}
	SECTION("Multiplication by number") {
		auto v3 = v1*3;
		REQUIRE(v3[0] == 3);
		REQUIRE(v3[1] == -6);
		REQUIRE(v3[2] == 15);
		auto v4 = -3*v2;
		REQUIRE(v4[0] == 0);
		REQUIRE(v4[1] == 6);
		REQUIRE(v4[2] == -3);
		// Integrity check of base vectors
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == -2);
		REQUIRE(v1[2] == 5);
		REQUIRE(v2[0] == 0);
		REQUIRE(v2[1] == -2);
		REQUIRE(v2[2] == 1);
	}
	SECTION("Element-wise multiplication") {
		auto v3 = v1*v2;
		REQUIRE(v3[0] == 0);
		REQUIRE(v3[1] == 4);
		REQUIRE(v3[2] == 5);
		// Integrity check of base vectors
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == -2);
		REQUIRE(v1[2] == 5);
		REQUIRE(v2[0] == 0);
		REQUIRE(v2[1] == -2);
		REQUIRE(v2[2] == 1);
	}
	SECTION("Division by number") {
		auto v1 = sfz::Vector<int,2>{2, -2}/2;
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == -1);
	}
	SECTION("Element-wise division") {
		auto v3 = v1 / v1;
		REQUIRE(v3[0] == 1);
		REQUIRE(v3[1] == 1);
		REQUIRE(v3[2] == 1);
	}
	SECTION("Addition assignment") {
		v1 += v2;
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == -4);
		REQUIRE(v1[2] == 6);
	}
	SECTION("Subtraction assignment") {
		v1 -= v2;
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 0);
		REQUIRE(v1[2] == 4);
	}
	SECTION("Multiplication by number assignment") {
		v1 *= 3;
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == -6);
		REQUIRE(v1[2] == 15);
	}
	SECTION("Element-wise multiplication assignment") {
		v1 *= v2;
		REQUIRE(v1[0] == 0);
		REQUIRE(v1[1] == 4);
		REQUIRE(v1[2] == 5);
	}
	SECTION("Division by number assignment") {
		sfz::Vector<int, 2> v3{2, -2};
		v3 /= 2;
		REQUIRE(v3[0] == 1);
		REQUIRE(v3[1] == -1);
	}
	SECTION("Element-wise division assignment") {
		v1 /= v1;
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 1);
		REQUIRE(v1[2] == 1);
	}
}

TEST_CASE("Length of vector", "[sfz::Vector]")
{
	using sfz::squaredLength;
	using sfz::length;
	sfz::Vector<int, 2> v1{2, 0};
	int v2Arr[] = {-2, 2, 2, -2, 3};
	sfz::Vector<int, 5> v2{v2Arr};

	SECTION("squaredLength()") {
		REQUIRE(squaredLength(v1) == 4);
		REQUIRE(squaredLength(v2) == 25);
	}
	SECTION("length()") {
		REQUIRE(length(v1) == 2);
		REQUIRE(length(v2) == 5);
	}
	SECTION("Rounding down") {
		sfz::Vector<int, 2> v3{2,1};
		REQUIRE(squaredLength(v3) == 5);
		REQUIRE(length(v3) == 2);
	}
}

TEST_CASE("Normalizing (making unit vector) vector", "[sfz::Vector]")
{
	sfz::Vector<float, 4> v1 = normalize(sfz::Vector<float, 4>{-2.f, 2.f, -2.f, 2.f});
	const float delta = 1e-3f;

	SECTION("Correct answer") {
		const float posLower = 0.5f - delta;
		const float posHigher = 0.5f + delta;
		const float negLower = -0.5f - delta;
		const float negHigher = -0.5f + delta;		

		REQUIRE(negLower <= v1[0]);
		REQUIRE(v1[0] <= negHigher);

		REQUIRE(posLower <= v1[1]);
		REQUIRE(v1[1] <= posHigher);

		REQUIRE(negLower <= v1[2]);
		REQUIRE(v1[2] <= negHigher);

		REQUIRE(posLower <= v1[3]);
		REQUIRE(v1[3] <= posHigher);
	}
}

TEST_CASE("Comparison operators", "[sfz::Vector]")
{
	sfz::Vector<int, 3> v1{-4, 0, 0};
	sfz::Vector<int, 3> v2{0, 2, 0};
	sfz::Vector<int, 3> v3{0, 2, 0};

	SECTION("== and !=") {
		REQUIRE(v1 == v1);
		REQUIRE(v2 == v2);
		REQUIRE(v3 == v3);	
		REQUIRE(v2 == v3);
		REQUIRE(v3 == v2);
		REQUIRE(v1 != v2);
		REQUIRE(v2 != v1);
	}
}

TEST_CASE("Dot (scalar) product", "[sfz::Vector]")
{
	using sfz::dot;
	SECTION("Correctness test") {
		sfz::Vector<int, 3> v1{1, 0, -2};
		sfz::Vector<int, 3> v2{6, 2, 2};
		int scalarProduct = dot(v1, v2);
		
		REQUIRE(scalarProduct == 2);
		
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 0);
		REQUIRE(v1[2] == -2);
		REQUIRE(v2[0] == 6);
		REQUIRE(v2[1] == 2);
		REQUIRE(v2[2] == 2);
	}
	SECTION("Using same vector twice") {	
		sfz::Vector<int, 2> v1{-3, 2};
		int scalarProduct = dot(v1, v1);
		
		REQUIRE(scalarProduct == 13);
		
		REQUIRE(v1[0] == -3);
		REQUIRE(v1[1] == 2);	
	}
}

TEST_CASE("Cross product", "[sfz::Vector]")
{
	sfz::Vector<int, 3> v1{-1, 4, 0};
	sfz::Vector<int, 3> v2{1, -2, 3};

	SECTION("Correctness test") {
		auto res = sfz::cross(v1, v2);

		REQUIRE(res[0] == 12);
		REQUIRE(res[1] == 3);
		REQUIRE(res[2] == -2);
	}
	SECTION("2nd correctness test") {
		auto res = sfz::cross(v2, v1);

		REQUIRE(res[0] == -12);
		REQUIRE(res[1] == -3);
		REQUIRE(res[2] == 2);
	}
	SECTION("A x A = 0") {
		auto res1 = sfz::cross(v1, v1);
		REQUIRE(res1[0] == 0);
		REQUIRE(res1[1] == 0);
		REQUIRE(res1[2] == 0);

		auto res2 = sfz::cross(v2, v2);
		REQUIRE(res2[0] == 0);
		REQUIRE(res2[1] == 0);
		REQUIRE(res2[2] == 0);
	}
}

TEST_CASE("Sum of vector", "[sfz::Vector]")
{
	using sfz::sum;
	sfz::Vector<int, 4> v1{1, 2, -4, 9};
	REQUIRE(sum(v1) == 8);
}

TEST_CASE("Angle of vectors", "[sfz::VectorSupport]")
{
	sfz::Vector<float, 2> vRight{1, 0};
	sfz::Vector<float, 2> vUp{0, 1};
	sfz::Vector<float, 2> vDown{0, -1};

	SECTION("(2D) Angle between y and (implicit) x-axis") {
		auto angle = sfz::angle(vUp);
		REQUIRE((3.1415f/2.f) <= angle);
		REQUIRE(angle <= (3.1416f/2.f));
	}
	SECTION("Angle between y and (explicit) x-axis") {
		auto angle = sfz::angle(vRight, vUp);
		REQUIRE((3.1415f/2.f) <= angle);
		REQUIRE(angle <= (3.1416f/2.f));
	}
	SECTION("Angle between same vectors") {
		auto angle1 = angle(vRight);
		REQUIRE(angle1 == 0);

		auto angle2 = angle(vUp, vUp);
		REQUIRE(angle2 == 0);
	}
	SECTION("(2D) Angle with implicit x-axis should never give negative angles") {
		auto angle = sfz::angle(vDown);
		REQUIRE((3.f*3.1415f/2.f) <= angle);
		REQUIRE(angle <= (3.f*3.1416f/2.f));
	}
}

TEST_CASE("Rotating vectors", "[sfz::VectorSupport]")
{
	sfz::Vector<float, 2> vRight{1, 0};
	sfz::Vector<float, 2> vUp{0, 1};
	
	SECTION("Rotates in positive direction") {
		auto res = sfz::rotate(vRight, 3.1415926f);
		REQUIRE(-1.01f <= res[0]);
		REQUIRE(res[0] <= -0.99f);
		REQUIRE(-0.01f <= res[1]);
		REQUIRE(res[1] <= 0.01f);

		auto angleOrg = angle(vRight);
		auto angleRes = angle(res);
		REQUIRE((angleOrg + 3.1415f) <= angleRes);
		REQUIRE(angleRes <= (angleOrg + 3.1416f));
	}
	SECTION("Rotates in negative direction") {
		auto res = sfz::rotate(vUp, -3.1415926f);
		REQUIRE(-0.01f <= res[0]);
		REQUIRE(res[0] <= 0.01f);
		REQUIRE(-1.01f <= res[1]);
		REQUIRE(res[1] <= -0.99f);

		auto angleOrg = angle(vUp);
		auto angleRes = angle(res);
		// "+" in this case since angle() returns positive angle from x-axis.
		REQUIRE((angleOrg + 3.1415f) <= angleRes);
		REQUIRE(angleRes <= (angleOrg + 3.1416f));
	}
	SECTION("Nothing happens when rotating with 0") {
		auto resRight = sfz::rotate(vRight, 0.f);
		REQUIRE(resRight == vRight);
		auto resUp = sfz::rotate(vUp, 0.f);
		REQUIRE(resUp == vUp);
	}
}

TEST_CASE("Converting to string", "[sfz::Vector]")
{
	using sfz::to_string;
	sfz::Vector<int, 3> v{-1, 2, 10};
	REQUIRE(to_string(v) == "[-1, 2, 10]");
}

TEST_CASE("Hashing", "[sfz::Vector]")
{
	sfz::Vector<int, 3> v1{2, 100, 32};
	sfz::Vector<int, 3> v2{-1, 0, -10};
	sfz::Vector<int, 3> v3{0, -9, 14};

	// This test checks if unordered_map works as it should. Not a very good test, but the best I
	// can come up with to test if hashing works as it should at the moment.
	std::unordered_map<sfz::Vector<int, 3>, int> hashMap;
	hashMap[v1] = 1;
	hashMap[v2] = 2;
	hashMap[v3] = 3;
	REQUIRE(hashMap[v1] == 1);
	REQUIRE(hashMap[v2] == 2);
	REQUIRE(hashMap[v3] == 3);

	REQUIRE(sfz::hash(v1) != sfz::hash(v2));
	REQUIRE(sfz::hash(v2) != sfz::hash(v3));
}

TEST_CASE("Is proper POD", "[sfz::Vector]")
{
	REQUIRE(std::is_trivially_default_constructible<sfz::vec2>::value);
	REQUIRE(std::is_trivially_default_constructible<sfz::vec2i>::value);
	REQUIRE(std::is_trivially_default_constructible<sfz::vec3>::value);
	REQUIRE(std::is_trivially_default_constructible<sfz::vec3i>::value);

	REQUIRE(std::is_trivially_copyable<sfz::vec2>::value);
	REQUIRE(std::is_trivially_copyable<sfz::vec2i>::value);
	REQUIRE(std::is_trivially_copyable<sfz::vec3>::value);
	REQUIRE(std::is_trivially_copyable<sfz::vec3i>::value);

	REQUIRE(std::is_trivial<sfz::vec2>::value);
	REQUIRE(std::is_trivial<sfz::vec2i>::value);
	REQUIRE(std::is_trivial<sfz::vec3>::value);
	REQUIRE(std::is_trivial<sfz::vec3i>::value);

	REQUIRE(std::is_standard_layout<sfz::vec2>::value);
	REQUIRE(std::is_standard_layout<sfz::vec2i>::value);
	REQUIRE(std::is_standard_layout<sfz::vec3>::value);
	REQUIRE(std::is_standard_layout<sfz::vec3i>::value);

#ifndef _MSC_VER
	REQUIRE(std::is_pod<sfz::vec2>::value);
	REQUIRE(std::is_pod<sfz::vec2i>::value);
	REQUIRE(std::is_pod<sfz::vec3>::value);
	REQUIRE(std::is_pod<sfz::vec3i>::value);
#endif

	REQUIRE(std::is_literal_type<sfz::vec2>::value);
	REQUIRE(std::is_literal_type<sfz::vec2i>::value);
	REQUIRE(std::is_literal_type<sfz::vec3>::value);
	REQUIRE(std::is_literal_type<sfz::vec3i>::value);
}
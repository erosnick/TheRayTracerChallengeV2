#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include <matrix.h>

#include "utils.h"

SCENARIO("Constructing and inspecting a 4x4 matrix", "[matrix]")
{
	GIVEN("the following 4x4 matrix M:"
		"| 1.0f	 | 2.0f  | 3.0f  | 4.0f  |"
		"| 5.5f  | 6.5f  | 7.5f  | 8.5f  |"
		"| 9.0f	 | 10.0f | 11.0f | 12.0f |"
		"| 13.5f | 14.5f | 15.5f | 16.5f |")
	{
		auto m = matrix4(1.0f, 2.0f, 3.0f, 4.0f,
			5.5f, 6.5f, 7.5f, 8.5f,
			9.0f, 10.0f, 11.0f, 12.0f,
			13.5f, 14.5f, 15.5f, 16.5f);

		THEN("Check elements")
		{
			REQUIRE(m(0, 0) == 1.0f);
			REQUIRE(m(0, 3) == 4.0f);
			REQUIRE(m(1, 0) == 5.5f);
			REQUIRE(m(1, 2) == 7.5f);
			REQUIRE(m(2, 2) == 11.0f);
			REQUIRE(m(3, 0) == 13.5f);
			REQUIRE(m(3, 2) == 15.5f);
		}
	}
}

SCENARIO("A 2x2 matrix ought to be representable", "[matrix]")
{
	GIVEN("the following 4x4 matrix M:"
		"| -3.0f | 5.0f  |"
		"|  1.0f | 2.0f  |")
	{
		auto m = matrix2(-3.0f, 5.0f, 1.0f, -2.0f);

		THEN("Check elements")
		{
			REQUIRE(m(0, 0) == -3.0f);
			REQUIRE(m(0, 1) == 5.0f);
			REQUIRE(m(1, 0) == 1.0f);
			REQUIRE(m(1, 1) == -2.0f);
		}
	}
}

SCENARIO("A 3x3 matrix ought to be representable", "[matrix]")
{
	GIVEN("the following 4x4 matrix M:"
		"| -3.0f |  5.0f  |  0.0f |"
		"|  1.0f | -2.0f  | -7.0f |"
		"|  0.0f |  1.0f  |  1.0f |")
	{
		auto m = matrix3(-3.0f, 5.0f, 0.0f,
			1.0f, -2.0f, -7.0f,
			0.0f, 1.0f, 1.0f);

		THEN("Check elements")
		{
			REQUIRE(m(0, 0) == -3.0f);
			REQUIRE(m(0, 1) == 5.0f);
			REQUIRE(m(0, 2) == 0.0f);

			REQUIRE(m(1, 0) == 1.0f);
			REQUIRE(m(1, 1) == -2.0f);
			REQUIRE(m(1, 2) == -7.0f);

			REQUIRE(m(2, 0) == 0.0f);
			REQUIRE(m(2, 1) == 1.0f);
			REQUIRE(m(2, 2) == 1.0f);
		}
	}
}

SCENARIO("Matrix equality with identical matrices", "[matrix]")
{
	GIVEN("the following matrix A:"
		"| 1.0f | 2.0f | 3.0f | 4.0f |"
		"| 5.0f | 6.0f | 7.0f | 8.0f |"
		"| 9.0f | 8.0f | 7.0f | 6.0f |"
		"| 5.0f | 4.0f | 3.0f | 2.0f |"
		"And the following matrix B :"
		"| 1.0f | 2.0f | 3.0f | 4.0f |"
		"| 5.0f | 6.0f | 7.0f | 8.0f |"
		"| 9.0f | 8.0f | 7.0f | 6.0f |"
		"| 5.0f | 4.0f | 3.0f | 2.0f | ")
	{
		auto m1 = matrix4(1.0f, 2.0f, 3.0f, 4.0f,
			5.0f, 6.0f, 7.0f, 8.0f,
			9.0f, 8.0f, 7.0f, 6.0f,
			5.0f, 4.0f, 3.0f, 2.0f);

		auto m2 = matrix4(1.0f, 2.0f, 3.0f, 4.0f,
			5.0f, 6.0f, 7.0f, 8.0f,
			9.0f, 8.0f, 7.0f, 6.0f,
			5.0f, 4.0f, 3.0f, 2.0f);

		THEN("m1 == m2")
		{
			REQUIRE(m1 == m2);
		}
	}
}

SCENARIO("Matrix equality with different matrices", "[matrix]")
{
	GIVEN("the following matrix A:"
		"| 1.0f | 2.0f | 3.0f | 4.0f |"
		"| 5.0f | 6.0f | 7.0f | 8.0f |"
		"| 9.0f | 8.0f | 7.0f | 6.0f |"
		"| 5.0f | 4.0f | 3.0f | 2.0f |"
		"And the following matrix B :"
		"| 5.0f | 6.0f | 7.0f | 8.0f |"
		"| 1.0f | 2.0f | 3.0f | 4.0f |"
		"| 9.0f | 8.0f | 7.0f | 6.0f |"
		"| 5.0f | 4.0f | 3.0f | 2.0f | ")
	{
		auto m1 = matrix4(
			5.0f, 6.0f, 7.0f, 8.0f,
			1.0f, 2.0f, 3.0f, 4.0f,
			9.0f, 8.0f, 7.0f, 6.0f,
			5.0f, 4.0f, 3.0f, 2.0f);

		auto m2 = matrix4(
			1.0f, 2.0f, 3.0f, 4.0f,
			5.0f, 6.0f, 7.0f, 8.0f,
			9.0f, 8.0f, 7.0f, 6.0f,
			5.0f, 4.0f, 3.0f, 2.0f);

		THEN("m1 != m2")
		{
			REQUIRE(m1 != m2);
		}
	}
}

SCENARIO("Multiplying two matrices", "[matrix]")
{
	GIVEN("the following matrix A:"
		"| 1.0f | 2.0f | 3.0f | 4.0f |"
		"| 5.0f | 6.0f | 7.0f | 8.0f |"
		"| 9.0f | 8.0f | 7.0f | 6.0f |"
		"| 5.0f | 4.0f | 3.0f | 2.0f |"
		"And the following matrix B :"
		"| -2.0f | 1.0f | 2.0f |  3.0f |"
		"|  3.0f | 2.0f | 1.0f | -1.0f |"
		"|  4.0f | 3.0f | 6.0f |  5.0f |"
		"|  1.0f | 2.0f | 7.0f |  8.0f |")
	{
		auto A = matrix4(
			1.0f, 2.0f, 3.0f, 4.0f,
			5.0f, 6.0f, 7.0f, 8.0f,
			9.0f, 8.0f, 7.0f, 6.0f,
			5.0f, 4.0f, 3.0f, 2.0f);

		auto B = matrix4(
			-2.0f, 1.0f, 2.0f, 3.0f,
			3.0f, 2.0f, 1.0f, -1.0f,
			4.0f, 3.0f, 6.0f, 5.0f,
			1.0f, 2.0f, 7.0f, 8.0f
		);

		THEN("A * B is the following 4x4 matrix:")
		{
			auto C = matrix4(
				20.0f, 22.0f, 50.0f, 48.0f,
				44.0f, 54.0f, 114.0f, 108.0f,
				40.0f, 58.0f, 110.0f, 102.0f,
				16.0f, 26.0f, 46.0f, 42.0f
			);

			REQUIRE(A * B == C);
		}
	}
}

SCENARIO("A matrix multiplied by a tuple", "[matrix]")
{
	GIVEN("the following matrix A:"
		"| 1.0f | 2.0f | 3.0f | 4.0f |"
		"| 2.0f | 4.0f | 4.0f | 2.0f |"
		"| 8.0f | 6.0f | 4.0f | 1.0f |"
		"| 0.0f | 0.0f | 0.0f | 1.0f |"
		"And b = tuple(1.0f, 2.0f, 3.0f, 1.0f)")
	{
		auto A = matrix4(
			1.0f, 2.0f, 3.0f, 4.0f,
			2.0f, 4.0f, 4.0f, 2.0f,
			8.0f, 6.0f, 4.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		auto b = tuple(1.0f, 2.0f, 3.0f, 1.0f);

		THEN("A * b == tuple(18, 24, 33, 1)")
		{
			REQUIRE(A * b == tuple(18.0f, 24.0f, 33.0f, 1.0f));
		}
	}
}

SCENARIO("A matrix multiplied by a the identity matrix", "[matrix]")
{
	GIVEN("the following matrix A:"
		"| 0 | 1 |  2 |  4 |"
		"| 1 | 2 |  4 |  8 |"
		"| 2 | 4 |  8 | 16 |"
		"| 4 | 8 | 16 | 32 |")
	{
		auto A = matrix4(
			0.0f, 1.0f, 2.0f, 4,
			1.0f, 2.0f, 4.0f, 8,
			2.0f, 4.0f, 8.0f, 16,
			4.0f, 8.0f, 16.0f, 32
		);

		auto identityMatrix = matrix4(1.0f);

		THEN("A * identityMatrix = A")
		{
			REQUIRE(A * identityMatrix == A);
		}
	}
}

SCENARIO("Transposing a matrix", "[matrix]")
{
	GIVEN("the following matrix A:"
		"| 0.0f | 9.0f | 3.0f | 0.0f |"
		"| 9.0f | 8.0f | 0.0f | 8.0f |"
		"| 1.0f | 8.0f | 5.0f | 3.0f |"
		"| 0.0f | 0.0f | 5.0f | 8.0f |")
	{
		auto A = matrix4(
		   0.0f, 9.0f, 3.0f, 0.0f,
		   9.0f, 8.0f, 0.0f, 8.0f,
		   1.0f, 8.0f, 5.0f, 3.0f,
		   0.0f, 0.0f, 5.0f, 8.0f
		);

		THEN("transpose(A) is the following matrix:"
			"| 0.0f | 9.0f | 1.0f | 0.0f |"
			"| 9.0f | 8.0f | 8.0f | 0.0f |"
			"| 3.0f | 0.0f | 5.0f | 5.0f |"
			"| 0.0f | 8.0f | 3.0f | 8.0f |")
		{
			auto B = matrix4(
				0.0f, 9.0f, 1.0f, 0.0f,
				9.0f, 8.0f, 8.0f, 0.0f,
				3.0f, 0.0f, 5.0f, 5.0f,
				0.0f, 8.0f, 3.0f, 8.0f
			);

			REQUIRE(transpose(A) == B);
		}
	}
}

SCENARIO("Transposing the identity matrix", "[matrix]")
{
	GIVEN("A = transpose(identityMatrix)")
	{
		auto A = transpose(matrix4(1.0f));

		THEN("A = identityMatrix")
		{
			REQUIRE(A == matrix4(1.0f));
		}
	}
}

SCENARIO("Calculating the determinant of a 2x2 matrix", "[matrix]")
{
	GIVEN("Calculating the determinant of a 2x2 matrix"
		"|  1.0f | 5.0f |"
		"| -3.0f | 2.0f |")
	{
		auto A = matrix2(1.0f, 5.0f,
							   -3.0f, 2.0f);

		THEN("determinant(A) == 17")
		{
			REQUIRE(determinant(A) == 17.0f);
		}
	}
}

SCENARIO("A submatrix of a 3x3 matrix is a 2x2 matrix", "[matrix]")
{
	GIVEN("the following 3x3 matrix A:"
		"|  1.0f | 5.0f |  0.0f |"
		"| -3.0f | 2.0f |  7.0f |"
		"|  0.0f | 6.0f | -3.0f |")
	{
		auto A = matrix3(
			 1.0f, 5.0f,  0.0f,
			-3.0f, 2.0f,  7.0f,
			 0.0f, 6.0f, -3.0f
		);

		THEN("submatrix(A, 0, 2) is the following 2x2 matrix:"
			"| -3.0f | 2.0f |"
			"|  0.0f | 6.0f |")
		{
			auto C = matrix2(
				-3.0f, 2.0f,
				 0.0f, 6.0f
			);

			auto B = subMatrix(A, 0, 2);
			REQUIRE(B == C);
		}
	}
}

SCENARIO("A submatrix of a 4x4 matrix is a 3x3 matrix", "[matrix]")
{
	GIVEN("the following 4x4 matrix A:"
		"| -6.0f | 1.0f |  1.0f | 6.0f |"
		"| -8.0f | 5.0f |  8.0f | 6.0f |"
		"| -1.0f | 0.0f |  8.0f | 2.0f |"
		"| -7.0f | 1.0f | -1.0f | 1.0f |")
	{
		auto A = matrix4(
			-6.0f, 1.0f,  1.0f, 6.0f,
			-8.0f, 5.0f,  8.0f, 6.0f,
			-1.0f, 0.0f,  8.0f, 2.0f,
			-7.0f, 1.0f, -1.0f, 1.0f
		);

		THEN("submatrix(A, 2, 1) is the following 3x3 matrix:"
			"| -6.0f |  1.0f | 6.0f |"
			"| -8.0f |  8.0f | 6.0f |"
			"| -7.0f | -1.0f | 1.0f |")
		{
			auto B = matrix3(
				-6.0f,  1.0f, 6.0f,
				-8.0f,  8.0f, 6.0f,
				-7.0f, -1.0f, 1.0f
			);

			REQUIRE(subMatrix(A, 2, 1) == B);
		}
	}
}

SCENARIO("Calculating a minor of a 3x3 matrix", "[matrix]")
{
	GIVEN("the following 3x3 matrix A:"
		"| 3.0f |  5.0f |  0.0f |"
		"| 2.0f | -1.0f | -7.0f |"
		"| 6.0f | -1.0f |  5.0f |")
	{
		auto A = matrix3(
			3.0f,  5.0f,  0.0f,
			2.0f, -1.0f, -7.0f,
			6.0f, -1.0f,  5.0f
		);

		auto B = subMatrix(A, 1, 0);

		THEN("determinant(B) = 25 And minor(A, 1, 0) = 25")
		{
			REQUIRE(determinant(B) == 25.0f);
			REQUIRE(minor(A, 1, 0) == 25.0f);
		}
	}
}

SCENARIO("Calculating a cofactor of a 3x3 matrix", "[matrix]")
{
	GIVEN("the following 3x3 matrix A:"
		"| 3 |  5 |  0 |"
		"| 2 | -1 | -7 |"
		"| 6 | -1 |  5 |")
	{
		auto A = matrix3(
			3.0f,  5.0f,  0.0f,
			2.0f, -1.0f, -7.0f,
			6.0f, -1.0f,  5.0f
		);

		THEN("Check equality")
		{
			REQUIRE(minor(A, 0, 0) == -12.0f);
			REQUIRE(cofactor(A, 0, 0) == -12.0f);
			REQUIRE(minor(A, 1, 0) == 25.0f);
			REQUIRE(cofactor(A, 1, 0) == -25.0f);
		}
	}
}

SCENARIO("Calculating the determinant of a 3x3 matrix", "[matrix]")
{
	GIVEN("the following 3x3 matrix A:"
		"|  1 | 2 |  6 |"
		"| -5 | 8 | -4 |"
		"|  2 | 6 |  4 |")
	{
		auto A = matrix3(
			  1.0f, 2.0f,  6.0f,
			 -5.0f, 8.0f, -4.0f,
			  2.0f, 6.0f,  4.0f
		);

		THEN("Check equality")
		{
			REQUIRE(cofactor(A, 0, 0) == 56.0f);
			REQUIRE(cofactor(A, 0, 1) == 12.0f);
			REQUIRE(cofactor(A, 0, 2) == -46.0f);
			REQUIRE(determinant(A) == -196.0f);
		}
	}
}

SCENARIO("Testing an invertible matrix for invertibility", "[matrix]")
{
	GIVEN("the following 4x4 matrix A:"
		"| 6.0f |  4.0f | 4.0f |  4.0f |"
		"| 5.0f |  5.0f | 7.0f |  6.0f |"
		"| 4.0f | -9.0f | 3.0f | -7.0f |"
		"| 9.0f |  1.0f | 7.0f | -6.0f |")
	{
		auto A = matrix4(
			6.0f,  4.0f, 4.0f,  4.0f,
			5.0f,  5.0f, 7.0f,  6.0f,
			4.0f, -9.0f, 3.0f, -7.0f,
			9.0f,  1.0f, 7.0f, -6.0f
		);

		THEN("determinant(A) = -2120 And A is invertible")
		{
			REQUIRE(determinant(A) == -2120.0f);
		}
	}
}

SCENARIO("Calculating the inverse of a matri", "[matrix]")
{
	GIVEN("the following 4x4 matrix A:"
		"| -5.0f |  2.0f |  6.0f | -8.0f |"
		"|  1.0f | -5.0f |  1.0f |  8.0f |"
		"|  7.0f |  7.0f | -6.0f | -7.0f |"
		"|  1.0f | -3.0f |  7.0f |  4.0f |")
	{
		auto A = matrix4(
		   -5.0f,  2.0f,  6.0f,  -8.0f,
			1.0f, -5.0f,  1.0f,   8.0f,
			7.0f,  7.0f, -6.0f,  -7.0f,
			1.0f, -3.0f,  7.0f,   4.0f
		);

		auto B = inverse(A);

		THEN("Check equality")
		{
			REQUIRE(determinant(A) == 532.0f);
			REQUIRE(cofactor(A, 2, 3) == -160.0f);
			REQUIRE(equal(B(3, 2), -160.0f / 532.0f));
			REQUIRE(cofactor(A, 3, 2) == 105.0f);
			REQUIRE(equal(B(2, 3), 105.0f / 532.0f));
			
			auto inversedA = matrix4(
				 0.21805f,  0.45113f,  0.24060f, -0.04511f,
				-0.80827f, -1.45677f, -0.44361f,  0.52068f,
				-0.07895f, -0.22368f, -0.05263f,  0.19737f,
				-0.52256f, -0.81391f, -0.30075f,  0.30639f
			);

			REQUIRE(B == inversedA);
		}
	}
}

SCENARIO("Calculating the inverse of another matrix", "[matrix]")
{
	GIVEN("the following 4x4 matrix A:"
		"|  8.0f | -5.0f |  9.0f |  2.0f |"
		"|  7.0f |  5.0f |  6.0f |  1.0f |"
		"| -6.0f |  0.0f |  9.0f |  6.0f |"
		"| -3.0f |  0.0f | -9.0f | -4.0f |")
	{
		auto A = matrix4(
			8.0f, -5.0f,  9.0f,  2.0f,
			7.0f,  5.0f,  6.0f,  1.0f,
		   -6.0f,  0.0f,  9.0f,  6.0f,
		   -3.0f,  0.0f, -9.0f, -4.0f
		);

		THEN("inverse(A) is the following 4x4 matrix:"
			"| -0.15385f | -0.15385f | -0.28205f | -0.53846f |"
			"| -0.07692f |  0.12308f |  0.02564f |  0.03077f |"
			"|  0.35897f |  0.35897f |  0.43590f |  0.92308f |"
			"| -0.69231f | -0.69231f | -0.76923f | -1.92308f |")
		{
			auto inversedA = inverse(A);

			auto B = matrix4(
				-0.15385f, -0.15385f, -0.28205f, -0.53846f,
				-0.07692f,  0.12308f,  0.02564f,  0.03077f,
				 0.35897f,  0.35897f,  0.43590f,  0.92308f,
				-0.69231f, -0.69231f, -0.76923f, -1.92308f
			);
			
			REQUIRE(B == inversedA);
		}
	}
}

SCENARIO("Calculating the inverse of the third matrix", "[matrix]")
{
	GIVEN("the following 4x4 matrix A:"
		"|  9.0f |  3.0f |  0.0f |  9.0f |"
		"| -5.0f | -2.0f | -6.0f | -3.0f |"
		"| -4.0f |  9.0f |  6.0f |  4.0f |"
		"| -7.0f |  6.0f |  6.0f |  2.0f |")
	{
		auto A = matrix4(
			 9.0f,  3.0f,  0.0f,  9.0f,
			-5.0f, -2.0f, -6.0f, -3.0f,
			-4.0f,  9.0f,  6.0f,  4.0f,
			-7.0f,  6.0f,  6.0f,  2.0f
		);

		THEN("inverse(A) is the following 4x4 matrix:"
			"| -0.04074f | -0.07778f |  0.14444f | -0.22222f |"
			"| -0.07778f |  0.03333f |  0.36667f | -0.33333f |"
			"| -0.02901f | -0.14630f | -0.10926f |  0.12963f |"
			"|  0.17778f |  0.06667f | -0.26667f |  0.33333f |")
		{
			auto inversedA = inverse(A);

			auto B = matrix4(
				-0.04074f, -0.07778f,  0.14444f, -0.22222f,
				-0.07778f,  0.03333f,  0.36667f, -0.33333f,
				-0.02901f, -0.14630f, -0.10926f,  0.12963f,
				 0.17778f  ,0.06667f, -0.26667f,  0.33333f
			);

			REQUIRE(B == inversedA);
		}
	}
}

SCENARIO("Multiplying a product by its inverse", "[matrix]")
{
	GIVEN("the following 4x4 matrix A:"
		"|  3.0f | -9.0f |  7.0f |  3.0f |"
		"|  3.0f | -8.0f |  2.0f | -9.0f |"
		"| -4.0f |  4.0f |  4.0f |  1.0f |"
		"| -6.0f |  5.0f | -1.0f |  1.0f |"
		"And the following 4x4 matrix B :"
		"| 8.0f |  2.0f | 2.0f | 2.0f |"
		"| 3.0f | -1.0f | 7.0f | 0.0f |"
		"| 7.0f |  0.0f | 5.0f | 4.0f |"
		"| 6.0f | -2.0f | 0.0f | 5.0f |")
	{
		auto A = matrix4(
			 3.0f, -9.0f,  7.0f,  3.0f,
			 3.0f, -8.0f,  2.0f, -9.0f,
			-4.0f,  4.0f,  4.0f,  1.0f,
			-6.0f,  5.0f, -1.0f,  1.0f
		);

		auto B = matrix4(
			8.0f,  2.0f, 2.0f, 2.0f,
			3.0f, -1.0f, 7.0f, 0.0f,
			7.0f,  0.0f, 5.0f, 4.0f,
			6.0f, -2.0f, 0.0f, 5.0f
		);

		auto C = A * B;

		THEN("C * inverse(B) == A")
		{
			REQUIRE(C * inverse(B) == A);
		}
	}
}
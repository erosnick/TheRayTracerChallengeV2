#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include "matrix.h"
#include "transforms.h"

#include "utils.h"
#include "colors.h"
#include <canvas.h>

// Chapter 3 Matrices

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

// Chapter 4 Matrix Transformations

SCENARIO("Multiplying by a translation matrix", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2) And p = point(-3, 4, 5)")
	{
		auto transform = translate(5.0f, -3.0f, 2.0f);
		auto p = point(-3.0f, 4.0f, 5.0f);

		THEN("transform * p == point(2, 1, 7)")
		{
			REQUIRE(transform * p == point(2.0f, 1.0f, 7.0f));
		}
	}
}

SCENARIO("Multiplying by the inverse of a translation matrix", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2)"
			"And inv = inverse(transform)"
			"And p = point(-3, 4, 5)")
	{
		auto transform = translate(5.0f, -3.0f, 2.0f);
		auto inv = inverse(transform);
		auto p = point(-3.0f, 4.0f, 5.0f);

		THEN("transform * p == point(2, 1, 7)")
		{
			REQUIRE(inv * p == point(-8.0f, 7.0f, 3.0f));
		}
	}
}

SCENARIO("Translation does not affect vectors", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2) And v = vector(-3, 4, 5)")
	{
		auto transform = translate(5.0f, -3.0f, 2.0f);
		auto v = vector(-3.0f, 4.0f, 5.0f);

		THEN("transform * p == point(2, 1, 7)")
		{
			REQUIRE(transform * v == v);
		}
	}
}

SCENARIO("A scaling matrix applied to a point", "[matrix]")
{
	GIVEN("transform = translation(5, -3, 2) And v = vector(-3, 4, 5)")
	{
		auto transform = scale(2.0f, 3.0f, 4.0f);
		auto p = point(-4.0f, 6.0f, 8.0f);

		THEN("transform * p == point(-8.0f, 18.0f, 32.0f)")
		{
			REQUIRE(transform * p == point(-8.0f, 18.0f, 32.0f));
		}
	}
}

SCENARIO("A scaling matrix applied to a vector", "[matrix]")
{
	GIVEN("transform = translation(2.0f, 3.0f, 4.0f) And v = vector(-4.0f, 6.0f, 8.0f)")
	{
		auto transform = scale(2.0f, 3.0f, 4.0f);
		auto v = vector(-4.0f, 6.0f, 8.0f);

		THEN("transform * p == point(-8.0f, 18.0f, 32.0f)")
		{
			REQUIRE(transform * v == vector(-8.0f, 18.0f, 32.0f));
		}
	}
}

SCENARIO("Multiplying by the inverse of a scaling matrix", "[matrix]")
{
	GIVEN("transform = scaling(2.0f, 3.0f, 4.0f)"
		"And inv = inverse(transform)"
		"And v = vector(-4.0f, 6.0f, 8.0f)")
	{
		auto transform = scale(2.0f, 3.0f, 4.0f);
		auto inv = inverse(transform);
		auto v = vector(-4.0f, 6.0f, 8.0f);

		THEN("inv * v = vector(-2.0f, 2.0f, 2.0f)")
		{
			REQUIRE(inv * v == vector(-2.0f, 2.0f, 2.0f));
		}
	}
}

SCENARIO("Reflection is scaling by a negative value", "[matrix]")
{
	GIVEN("transform = scaling(-1.0f, 1.0f, 1.0f) And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = scale(-1.0f, 1.0f, 1.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(-2.0f, 3.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(-2.0f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("Rotating a point around the x axis", "[matrix].0f")
{
	GIVEN("p = point(0.0f, 1.0f, 0.0f)"
		"And halfQuarter = rotateX(¦Ð / 4.0f)"
		"And fullQuarter = rotateX(¦Ð / 2.0f)")
	{
		auto p = point(0.0f, 1.0f, 0.0f);
		auto halfQuarter = rotateX(PI / 4.0f);
		auto fullQuarter = rotateX(PI / 2.0f);
		
		THEN("halfQuarter * p == point(0.0f, ¡Ì2/2, ¡Ì2/2)"
		 "And fullQuarter * p == point(0.0f, 0.0f, 1.0f)")
		{
			REQUIRE(halfQuarter * p == point(0.0f, SQRT2 / 2.0f, SQRT2 / 2.0f));
			REQUIRE(fullQuarter * p == point(0.0f, 0.0f, 1.0f));
		}
	}
}

SCENARIO("The inverse of an x-rotation rotates in the opposite direction", "[matrix].0f")
{
	GIVEN("p = point(0, 1, 0)"
		"And half_quarter = rotation_x(¦Ð / 4)"
		"And inv = inverse(half_quarter)")
	{
		auto p = point(0.0f, 1.0f, 0.0f);
		auto halfQuarter = rotateX(PI / 4.0f);
		auto inv = inverse(halfQuarter);

		THEN("inv * p = point(0, ¡Ì2/2, -¡Ì2/2)")
		{
			REQUIRE(inv * p == point(0.0f, SQRT2 / 2.0f, -SQRT2 / 2.0f));
		}
	}
}

SCENARIO("Rotating a point around the y axis", "[matrix].0f")
{
	GIVEN("p = point(0.0f, 0.0f, 1.0f)"
		"And halfQuarter = rotateY(¦Ð / 4.0f)"
		"And fullQuarter = rotateY(¦Ð / 2.0f)")
	{
		auto p = point(0.0f, 0.0f, 1.0f);
		auto halfQuarter = rotateY(PI / 4.0f);
		auto fullQuarter = rotateY(PI / 2.0f);

		THEN("halfQuarter * p == point(¡Ì2/2, 0.0f, ¡Ì2/2)"
			"And fullQuarter * p == point(1.0f, 0.0f, 0.0f)")
		{
			REQUIRE(halfQuarter * p == point(SQRT2 / 2.0f, 0.0f, SQRT2 / 2.0f));
			REQUIRE(fullQuarter * p == point(1.0f, 0.0f, 0.0f));
		}
	}
}

SCENARIO("Rotating a point around the z axis", "[matrix].0f")
{
	GIVEN("p = point(0.0f, 1.0f, 0.0f)"
		"And halfQuarter = rotateZ(¦Ð / 4.0f)"
		"And fullQuarter = rotateZ(¦Ð / 2.0f)")
	{
		auto p = point(0.0f, 1.0f, 0.0f);
		auto halfQuarter = rotateZ(PI / 4.0f);
		auto fullQuarter = rotateZ(PI / 2.0f);

		THEN("halfQuarter * p == point(¡Ì2/2, 0.0f, ¡Ì2/2)"
			"And fullQuarter * p == point(1.0f, 0.0f, 0.0f)")
		{
			REQUIRE(halfQuarter * p == point(-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f));
			REQUIRE(fullQuarter * p == point(-1.0f, 0.0f, 0.0f));
		}
	}
}

SCENARIO("A shearing transformation moves x in proportion to y", "[matrix]")
{
	GIVEN("transform = shearing(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(5.0f, 3.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(5.0f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves x in proportion to z", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(6.0f, 3.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(6.0f, 3.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves y in proportion to x", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(2.0f, 5.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(2.0f, 5.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves y in proportion to z", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(2.0f, 5.0f, 4.0f)")
		{
			REQUIRE(transform * p == point(2.0f, 7.0f, 4.0f));
		}
	}
}

SCENARIO("A shearing transformation moves z in proportion to y", "[matrix]")
{
	GIVEN("transform = shearing(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f)"
		"And p = point(2.0f, 3.0f, 4.0f)")
	{
		auto transform = shearing(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		auto p = point(2.0f, 3.0f, 4.0f);

		THEN("transform * p == point(2.0f, 3.0f, 7.0f)")
		{
			REQUIRE(transform * p == point(2.0f, 3.0f, 7.0f));
		}
	}
}

SCENARIO("Individual transformations are applied in sequence", "[matrix]")
{
	GIVEN("p1 = point(1.0f, 0.0f, 1.0f)"
		"And A = rotateX(¦Ð / 2.0f)"
		"And B = scale(5.0f, 5.0f, 5.0f)"
		"And C = translate(10.0f, 5.0f, 7.0f)")
	{
		auto A = rotateX(PI / 2.0f);
		auto B = scale(5.0f, 5.0f, 5.0f);
		auto C = translate(10.0f, 5.0f, 7.0f);

		auto p1 = point(1.0f, 0.0f, 1.0f);

		WHEN("p2 = A * p1"
			 "p3 = B * p2"
			 "p4 = C * p3")
		{
			auto p2 = A * p1;

			THEN("p2 == point(1.0f, -1.0f, 0.0f)")
			{
				REQUIRE(p2 == point(1.0f, -1.0f, 0.0f));
			}

			auto p3 = B * p2;

			THEN("p3 == point(5.0f, -5.0f, 0.0f)")
			{
				REQUIRE(p3 == point(5.0f, -5.0f, 0.0f));
			}

			auto p4 = C * p3;

			THEN("p4 == point(15.0f, 0.0f, 7.0f)")
			{
				REQUIRE(p4 == point(15.0f, 0.0f, 7.0f));
			}
		}
	}
}

SCENARIO("Chained transformations must be applied in reverse order", "[matrix]")
{
	GIVEN("p = point(1.0f, 0.0f, 1.0f)"
		"And A = rotateX(¦Ð / 2.0f)"
		"And B = scale(5.0f, 5.0f, 5.0f)"
		"And C = translate(10.0f, 5.0f, 7.0f)")
	{
		auto p = point(1.0f, 0.0f, 1.0f);
		auto A = rotateX(PI / 2.0f);
		auto B = scale(5.0f, 5.0f, 5.0f);
		auto C = translate(10.0f, 5.0f, 7.0f);

		WHEN("T = C * B * A")
		{
			auto T = C * B * A;

			THEN("T * p == point(15, 0, 7)")
			{
				REQUIRE(T * p == point(15, 0, 7));
			}
		}
	}
}

SCENARIO("Draw a clock face", "[matrix]")
{
	GIVEN("")
	{
		THEN("")
		{
			auto c = Canvas(200, 200);

			auto p = point(0.0f, 90.0f, 0.0f);

			for (int32_t i = 0; i < 12; i++)
			{
				auto rotation = rotateZ(PI / 6.0f);
				p = rotation * p;

				c.writePixel(static_cast<int32_t>(p.x + 100.0f), static_cast<int32_t>(p.y + 100.0f), Color::White);
			}

			c.writeToPPM("clock.ppm");

			REQUIRE(true);
		}
	}
}
#include "../include/quat.h"

#include <gtest/gtest.h>

#include <cmath>
#include <cstring>

TEST(ctor, zeros)
{
	constexpr float expected[4]{ 0, 0, 0, 0 };
	EXPECT_EQ(0, std::memcmp(Quat(0, 0, 0, 0).data().data(), expected, 4 * sizeof(float)));
}

TEST(ctor, defaultCtor)
{
	const Quat q;
	constexpr float expected[4]{ 0, 0, 0, 0 };
	EXPECT_EQ(0, std::memcmp(q.data().data(), expected, 4 * sizeof(float)));
}

TEST(ctor, paramCtor)
{
	const Quat q(1.0f, 2.0f, 3.0f, 4.0f);
	const std::vector< float > result = q.data();
	const std::vector< float > expected = { 2.0f, 3.0f, 4.0f, 1.0f };
	EXPECT_EQ(result.size(), expected.size());
	for (size_t i = 0; i < expected.size(); ++i)
	{
		EXPECT_FLOAT_EQ(result[i], expected[i]) << "Mismatch at index " << i;
	}
}

TEST(ctor, angleAxisDegrees)
{
	const vector3_t axis(1.0f, 0.0f, 0.0f);
	const Quat q(90.0f, false, axis);
	const std::vector< float > res = q.data();
	const float sq2_2 = std::sqrt(2.0f) / 2.0f;
	const std::vector< float > expected = { sq2_2, 0.0f, 0.0f, sq2_2 };

	for (size_t i = 0; i < expected.size(); ++i)
	{
		EXPECT_NEAR(res[i], expected[i], 1e-5) << "Mismatch at index " << i;
	}
}

TEST(ctor, angleAxisRadians)
{
	const vector3_t axis(0.0f, 1.0f, 0.0f);
	const Quat q(static_cast< float >(M_PI / 2.0), true, axis);
	const float sq2_2 = std::sqrt(2.0f) / 2.0f;
	const std::vector< float > expected = { 0.0f, sq2_2, 0.0f, sq2_2 };
	const auto res = q.data();

	for (size_t i = 0; i < expected.size(); ++i)
	{
		EXPECT_NEAR(res[i], expected[i], 1e-5) << "Mismatch at index " << i;
	}
}

TEST(operators, copyCtor)
{
	const Quat original(1.0f, 2.0f, 3.0f, 4.0f);
	const Quat &copy(original);

	EXPECT_TRUE(copy == original);
}

TEST(operators, add)
{
	const Quat q1(1.0f, 2.0f, 3.0f, 4.0f);
	const Quat q2(4.0f, 3.0f, 2.0f, 1.0f);
	const Quat sum = q1 + q2;
	const auto res = sum.data();
	constexpr float expected[4] = { 5.0f, 5.0f, 5.0f, 5.0f };
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_FLOAT_EQ(res[i], expected[i]);
	}
}

TEST(operators, sub)
{
	const Quat q1(1.0f, 2.0f, 3.0f, 4.0f);
	const Quat q2(0.5f, 1.5f, 2.5f, 3.5f);
	const Quat diff = q1 - q2;
	const auto res = diff.data();
	constexpr float expected[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_FLOAT_EQ(res[i], expected[i]);
	}
}

TEST(operators, mulQuat)
{
	const Quat qx(90.0f, false, vector3_t(1, 0, 0));
	const Quat qy(90.0f, false, vector3_t(0, 1, 0));
	const Quat mul = qx * qy;
	EXPECT_FALSE(mul == Quat(0, 0, 0, 0));
	EXPECT_FALSE(mul == Quat(1, 0, 0, 0));
}

TEST(operators, mulScalar)
{
	const Quat q(1.0f, 2.0f, 3.0f, 4.0f);
	const Quat mul = q * 2.0f;

	constexpr float expected[4] = { 4.0f, 6.0f, 8.0f, 2.0f };
	const auto res = mul.data();
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_FLOAT_EQ(res[i], expected[i]);
	}
}

TEST(operators, conjugate)
{
	const Quat q(1.0f, 2.0f, 3.0f, 4.0f);
	const Quat conj = ~q;

	constexpr float expected[4] = { -2.0f, -3.0f, -4.0f, 1.0f };
	const auto res = conj.data();
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_FLOAT_EQ(res[i], expected[i]);
	}
}

TEST(operators, normFloatConversion)
{
	const Quat q(3.0f, 4.0f, 0.0f, 0.0f);
	const auto len = static_cast< float >(q);
	EXPECT_FLOAT_EQ(len, 5.0f);
}

TEST(operators, mulVector)
{
	const Quat q(90.0f, false, vector3_t(0, 0, 1));
	const vector3_t v(1.0f, 0.0f, 0.0f);
	const Quat rotated = q * v;
	const std::vector< float > res = rotated.data();
	EXPECT_NEAR(res[0], 0.0f, 1e-5);
	EXPECT_NEAR(res[1], 1.0f, 1e-5);
	EXPECT_NEAR(res[2], 0.0f, 1e-5);
}

TEST(methods, matrixCheck)
{
	const Quat q(1.0f, 0.0f, 0.0f, 0.0f);
	const auto mat = q.matrix();
	const std::vector< float > expected = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

	ASSERT_EQ(mat.size(), expected.size());
	for (size_t i = 0; i < expected.size(); ++i)
	{
		EXPECT_FLOAT_EQ(mat[i], expected[i]) << "Mismatch at index " << i;
	}
}

TEST(methods, rotationMatrixCheck)
{
	const Quat q(1.0f, 0.0f, 0.0f, 0.0f);
	const auto rmat = q.rotation_matrix();
	const std::vector< float > expected = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

	ASSERT_EQ(rmat.size(), expected.size());
	for (size_t i = 0; i < expected.size(); ++i)
	{
		EXPECT_FLOAT_EQ(rmat[i], expected[i]) << "Mismatch at index " << i;
	}
}

TEST(negative, tryCatchAngleAxisZeroAxis)
{
	const vector3_t zero_axis(0.0f, 0.0f, 0.0f);
	try
	{
		const Quat q(45.0f, false, zero_axis);
		const std::vector< float > expected = { 0.0f, 0.0f, 0.0f, 0.0f };
		const std::vector< float > result = q.data();
		EXPECT_EQ(result.size(), expected.size());
		for (size_t i = 0; i < expected.size(); ++i)
		{
			EXPECT_FLOAT_EQ(result[i], expected[i]) << "Mismatch at index " << i;
		}
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown in angle-axis constructor with zero axis: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown in angle-axis constructor with zero axis.";
	}
}

TEST(negative, tryCatchCopyAssignment)
{
	try
	{
		const Quat original(1.0f, 2.0f, 3.0f, 4.0f);
		const Quat &copy = original;
		EXPECT_TRUE(copy == original);
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during copy assignment: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during copy assignment.";
	}
}

TEST(operators, associativity)
{
	try
	{
		const Quat q1(1.0f, 2.0f, 3.0f, 4.0f);
		const Quat q2(4.0f, 3.0f, 2.0f, 1.0f);
		const Quat q3(1.5f, 2.5f, 3.5f, 4.5f);
		const Quat res1 = (q1 * q2) * q3;
		const Quat res2 = q1 * (q2 * q3);
		const std::vector< float > data1 = res1.data();
		const std::vector< float > data2 = res2.data();
		EXPECT_EQ(data1.size(), data2.size());
		for (size_t i = 0; i < data1.size(); ++i)
		{
			EXPECT_NEAR(data1[i], data2[i], 1e-5) << "Mismatch at index " << i;
		}
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during associativity test: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during associativity test.";
	}
}

TEST(operators, conjugateProperties)
{
	try
	{
		const Quat q(1.0f, 2.0f, 3.0f, 4.0f);
		const Quat q_conj = ~q;
		const Quat product = q * q_conj;
		const float norm_sq = static_cast< float >(q) * static_cast< float >(q);
		const std::vector< float > prod = product.data();
		EXPECT_NEAR(prod[0], 0.0f, 1e-5);
		EXPECT_NEAR(prod[1], 0.0f, 1e-5);
		EXPECT_NEAR(prod[2], 0.0f, 1e-5);
		EXPECT_NEAR(prod[3], norm_sq, 1e-5);
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during conjugate property test: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during conjugate property test.";
	}
}

TEST(operators, negativeScalarMultiplication)
{
	try
	{
		const Quat q(1.0f, -2.0f, 3.0f, -4.0f);
		const Quat result = q * (-3.0f);
		const std::vector< float > res = result.data();
		constexpr float expected[4] = { 6.0f, -9.0f, 12.0f, -3.0f };
		for (int i = 0; i < 4; ++i)
		{
			EXPECT_FLOAT_EQ(res[i], expected[i]) << "Mismatch at index " << i;
		}
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during negative scalar multiplication test: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during negative scalar multiplication test.";
	}
}

TEST(operators, vectorRotationTryCatch)
{
	try
	{
		const Quat q(90.0f, false, vector3_t(0, 0, 1));
		const vector3_t v(1.0f, 0.0f, 0.0f);
		const Quat rotated = q * v;
		const std::vector< float > res = rotated.data();
		EXPECT_NEAR(res[0], 0.0f, 1e-5);
		EXPECT_NEAR(res[1], 1.0f, 1e-5);
		EXPECT_NEAR(res[2], 0.0f, 1e-5);
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during vector rotation test: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during vector rotation test.";
	}
}

TEST(methods, dataLength)
{
	try
	{
		const Quat q(1.0f, 2.0f, 3.0f, 4.0f);
		const std::vector< float > data = q.data();
		EXPECT_EQ(data.size(), 4);
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during data length test: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during data length test.";
	}
}

TEST(methods, rotationMatrixLength)
{
	try
	{
		const Quat q(1.0f, 0.0f, 0.0f, 0.0f);
		const std::vector< float > rmat = q.rotation_matrix();
		EXPECT_EQ(rmat.size(), 9);
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during rotation matrix length test: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during rotation matrix length test.";
	}
}

TEST(methods, matrixLength)
{
	try
	{
		const Quat q(1.0f, 0.0f, 0.0f, 0.0f);
		const std::vector< float > mat = q.matrix();
		EXPECT_EQ(mat.size(), 16);
	} catch (const std::exception &e)
	{
		FAIL() << "Exception thrown during matrix length test: " << e.what();
	} catch (...)
	{
		FAIL() << "Unknown exception thrown during matrix length test.";
	}
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

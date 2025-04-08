#include "../include/example.hpp"

#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

string writeTempFile(const string &filename, const string &content)
{
	std::fstream ofs(filename, std::fstream::out);
	ofs << content;
	ofs.close();
	return filename;
}

string readFileContent(const string &filename)
{
	std::fstream ifs(filename, std::fstream::in);
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}

TEST(SrtSubtitlesTest, ReadSingleBlock)
{
	string filename = "tmp_test1.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"1st block of Taito district, Shin-Ueno line.\n"
		"\n";
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();

	string outFilename = "tmp_test1_out.txt";
	std::fstream ofs(outFilename, std::fstream::out);
	subtitles.write(ofs, 2);
	ofs.close();

	string output = readFileContent(outFilename);
	EXPECT_NE(output.find("<SAMI>"), string::npos);
	EXPECT_NE(output.find("<BODY>"), string::npos);

	std::remove(filename.c_str());
	std::remove(outFilename.c_str());
}

TEST(SrtSubtitlesTest, ReadMultipleBlocks)
{
	string filename = "tmp_test2.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"Block one\n"
		"\n"
		"2\n"
		"00:00:40,000 --> 00:00:44,000\n"
		"Block two\n"
		"\n";
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();

	string outFilename = "tmp_test2_out.txt";
	std::fstream ofs(outFilename, std::fstream::out);
	subtitles.write(ofs, 2);
	ofs.close();

	string output = readFileContent(outFilename);
	EXPECT_NE(output.find("Block one"), string::npos);
	EXPECT_NE(output.find("Block two"), string::npos);

	std::remove(filename.c_str());
	std::remove(outFilename.c_str());
}

TEST(SrtSubtitlesTest, WriteSAMIFormat)
{
	string filename = "tmp_test3.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"Test text\n"
		"\n";
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();

	string outFilename = "tmp_test3_out.txt";
	std::fstream ofs(outFilename, std::fstream::out);
	subtitles.write(ofs, 2);
	ofs.close();

	string output = readFileContent(outFilename);
	EXPECT_NE(output.find("<SAMI>"), string::npos);
	EXPECT_NE(output.find("<BODY>"), string::npos);
	EXPECT_NE(output.find("Test text"), string::npos);

	std::remove(filename.c_str());
	std::remove(outFilename.c_str());
}

TEST(SrtSubtitlesTest, ChangeDeltaFormat1)
{
	string filename = "tmp_test4.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"Delta test\n"
		"\n";
	writeTempFile(filename, content);
	{
		std::fstream file(filename, std::fstream::in);
		SrtSubtitles subtitles;
		subtitles.read(file);
		file.close();
		std::fstream n_file;
		subtitles.change_delta(1, "00:00:00,100");

		std::fstream ofs(filename, std::fstream::out | std::fstream::trunc);
		subtitles.write(ofs, 1);
		ofs.close();
	}

	string modified = readFileContent(filename);
	EXPECT_NE(modified.find("00:00:33,040"), string::npos);
	EXPECT_NE(modified.find("00:00:36,560"), string::npos);

	std::remove(filename.c_str());
}

TEST(SrtSubtitlesTest, ChangeDeltaFormat2)
{
	string filename = "tmp_test5.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"Delta start only\n"
		"\n";
	writeTempFile(filename, content);

	{
		std::fstream file(filename, std::fstream::in);
		SrtSubtitles subtitles;
		subtitles.read(file);
		file.close();
		std::fstream n_file;
		subtitles.change_delta(2, "00:00:00,100");

		std::fstream ofs(filename, std::fstream::out | std::fstream::trunc);
		subtitles.write(ofs, 1);
		ofs.close();
	}

	string modified = readFileContent(filename);
	EXPECT_NE(modified.find("00:00:33,040"), string::npos);
	EXPECT_NE(modified.find("00:00:36,460"), string::npos);

	std::remove(filename.c_str());
}

TEST(SrtSubtitlesTest, ChangeDeltaFormat3)
{
	string filename = "tmp_test6.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"Delta end only\n"
		"\n";
	writeTempFile(filename, content);

	{
		std::fstream file(filename, std::fstream::in);
		SrtSubtitles subtitles;
		subtitles.read(file);
		file.close();
		std::fstream n_file;
		subtitles.change_delta(3, "00:00:00,100");

		std::fstream ofs(filename, std::fstream::out | std::fstream::trunc);
		subtitles.write(ofs, 1);
		ofs.close();
	}

	string modified = readFileContent(filename);
	EXPECT_NE(modified.find("00:00:32,940"), string::npos);
	EXPECT_NE(modified.find("00:00:36,560"), string::npos);

	std::remove(filename.c_str());
}

TEST(SrtSubtitlesTest, FindCollisionsNoCollision)
{
	string filename = "tmp_test7.txt";
	string content =
		"1\n"
		"00:00:10,000 --> 00:00:15,000\n"
		"Block 1\n"
		"\n"
		"2\n"
		"00:00:16,000 --> 00:00:20,000\n"
		"Block 2\n"
		"\n";
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();

	Subtitles *collisionSubs = subtitles.find_collisions();

	string outFilename = "tmp_test7_out.txt";
	std::fstream ofs(outFilename, std::fstream::out);
	collisionSubs->write(ofs, 2);
	ofs.close();

	string collisionsStr = readFileContent(outFilename);
	EXPECT_EQ(collisionsStr.find("<SYNC"), string::npos);

	delete collisionSubs;
	std::remove(filename.c_str());
	std::remove(outFilename.c_str());
}

TEST(SrtSubtitlesTest, FindCollisionsWithCollision)
{
	string filename = "tmp_test8.txt";
	string content =
		"1\n"
		"00:00:10,000 --> 00:00:20,000\n"
		"Block 1\n"
		"\n"
		"2\n"
		"00:00:15,000 --> 00:00:25,000\n"
		"Block 2\n"
		"\n";
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();

	Subtitles *collisionSubs = subtitles.find_collisions();

	string outFilename = "tmp_test8_out.txt";
	std::fstream ofs(outFilename, std::fstream::out);
	collisionSubs->write(ofs, 2);
	ofs.close();

	string collisions = readFileContent(outFilename);
	EXPECT_NE(collisions.find("Block 1"), string::npos);
	EXPECT_NE(collisions.find("Block 2"), string::npos);

	delete collisionSubs;
	std::remove(filename.c_str());
	std::remove(outFilename.c_str());
}

TEST(SrtSubtitlesTest, ReadEmptyFile)
{
	string filename = "tmp_test9.txt";
	string content;
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();

	string outFilename = "tmp_test9_out.txt";
	std::fstream ofs(outFilename, std::fstream::out);
	subtitles.write(ofs, 2);
	ofs.close();

	string output = readFileContent(outFilename);
	EXPECT_NE(output.find("<SAMI>"), string::npos);

	std::remove(filename.c_str());
	std::remove(outFilename.c_str());
}

TEST(SrtSubtitlesTest, ChangeDeltaInvalidDelta)
{
	string filename = "tmp_test10.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"Invalid delta test\n"
		"\n";
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in | std::fstream::out);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();

	file.open(filename, std::fstream::in | std::fstream::out);
	subtitles.change_delta(1, "00:00:00:100");
	file.close();

	string modified = readFileContent(filename);
	EXPECT_NE(modified.find("00:00:32,940"), string::npos);
	EXPECT_NE(modified.find("00:00:36,460"), string::npos);

	std::remove(filename.c_str());
}

TEST(SrtSubtitlesTest, ChangeDeltaExpectedShift)
{
	string filename = "tmp_test12.txt";
	string content =
		"1\n"
		"00:00:50,000 --> 00:00:55,000\n"
		"Shift test\n"
		"\n";
	writeTempFile(filename, content);

	std::fstream file(filename, std::fstream::in);
	SrtSubtitles subtitles;
	subtitles.read(file);
	file.close();
	std::fstream n_file;
	subtitles.change_delta(1, "00:00:01,000");

	string outFilename = "tmp_test12_out.txt";
	std::fstream ofs(outFilename, std::fstream::out);
	subtitles.write(ofs, 1);
	ofs.close();

	string modified = readFileContent(outFilename);
	EXPECT_NE(modified.find("00:00:51,000"), string::npos);
	EXPECT_NE(modified.find("00:00:56,000"), string::npos);

	std::remove(filename.c_str());
	std::remove(outFilename.c_str());
}

TEST(SmiSubtitlesTest, RemoveFormatting)
{
	string filename = "tmp_smi_format.txt";
	string content =
		"<SAMI>\n"
		"<BODY>\n"
		"<SYNC Start=1000 End=2000><P><b>X</b> Y</P></SYNC>\n"
		"</BODY>\n"
		"</SAMI>";
	writeTempFile(filename, content);
	SmiSubtitles subs;
	{
		std::fstream fs(filename, std::fstream::in | std::fstream::out);
		subs.removeFormatting(fs);
	}
	string output = readFileContent(filename);
	EXPECT_EQ(output.find("<b>"), string::npos);
	remove(filename.c_str());
}

TEST(AssSubtitlesTest, RemoveFormatting)
{
	string filename = "tmp_ass_format.txt";
	string content =
		"Style: Default,Arial,48,16777215,255,0,0,1,0,1,2,2,2,10,10,10,0,1\n"
		"Dialogue: Marked=0,0:00:01.00,0:00:02.00,Default,,0,0,0,,{\\b1}Hello{\\b0}";
	writeTempFile(filename, content);
	AssSubtitles subs;
	{
		std::fstream fs(filename, std::fstream::in | std::fstream::out);
		subs.removeFormatting(fs);
	}
	string output = readFileContent(filename);
	EXPECT_EQ(output.find("{\\b1}"), string::npos);
	remove(filename.c_str());
}

TEST(EncoreSubtitlesTest, RemoveFormatting)
{
	string filename = "tmp_enc_format.txt";
	string content = "1 00:00:01:01 00:00:02:01 <i>X</i>";
	writeTempFile(filename, content);
	EncoreSubtitles subs;
	{
		std::fstream fs(filename, std::fstream::in | std::fstream::out);
		subs.removeFormatting(fs);
	}
	string output = readFileContent(filename);
	EXPECT_EQ(output.find("<i>"), string::npos);
	remove(filename.c_str());
}

TEST(SrtSubtitlesTest, SetStyle)
{
	string filename = "tmp_style.txt";
	string content =
		"1\n"
		"00:00:32,940 --> 00:00:36,460\n"
		"Style test\n"
		"\n";
	writeTempFile(filename, content);
	SrtSubtitles subs;
	subs.set_style();
	std::fstream fs(filename, std::fstream::in);
	subs.read(fs);
	fs.close();
	string outFilename = "tmp_style_out.txt";
	{
		std::fstream ofs(outFilename, std::fstream::out);
		subs.write(ofs, 3);
	}
	string output = readFileContent(outFilename);
	EXPECT_NE(output.find("Style:"), string::npos);
	remove(filename.c_str());
	remove(outFilename.c_str());
}

TEST(AssSubtitlesTest, ChangeDeltaInvalid)
{
	string filename = "tmp_ass_invalid.txt";
	string content = "Dialogue: Marked=0,0:00:01.00,0:00:02.00,Default,,0,0,0,,Test";
	writeTempFile(filename, content);
	AssSubtitles subs;
	std::fstream fs(filename, std::fstream::in | std::fstream::out);
	subs.read(fs);
	subs.change_delta(1, "00:00:00:100");
	fs.close();
	string output = readFileContent(filename);
	EXPECT_NE(output.find("0:00:01.00"), string::npos);
	remove(filename.c_str());
}

TEST(EncoreSubtitlesTest, ChangeDeltaExpected)
{
	string filename = "tmp_enc_delta.txt";
	string content = "1 00:00:01:01 00:00:02:01 Test";
	writeTempFile(filename, content);
	EncoreSubtitles subs;
	std::fstream fs(filename, std::fstream::in | std::fstream::out);
	subs.read(fs);
	subs.change_delta(1, "00:00:01,000");
	fs.close();
	string output = readFileContent(filename);
	EXPECT_NE(output.find("00:00:02:"), string::npos);
	remove(filename.c_str());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

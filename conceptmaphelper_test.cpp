#include "conceptmaphelper.h"

#include <fstream>
#include <vector>
#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(test_cmap_wordwrap_and_unwordwrap_must_be_symmetrical)
{
  const auto v = {
    "",
    "1",
    "12",
    "1234",
    "12345678",
    "1234567890123456",
    "12345678901234567890123456789012",
    "123456789012345678901234567890123",
    "1234567890123456789012345678901234",
    "12345678901234567890123456789012345",
    "123456789012345678901234567890123456",
    "1234567890123456789012345678901234567",
    "12345678901234567890123456789012345678",
    "123456789012345678901234567890123456789",
    "1234567890123456789012345678901234567890",
    "1 1",
    "12 12",
    "123 123",
    "1234 1234",
    "12345 12345",
    "123456 123456",
    "1234567 1234567",
    "12345678 8",
    "123456789 9",
    "1234567890 0",
    "1234567890 1234567890",
    "1234567890 1234567890 1234567890",
    "1234567890 1234567890 1234567890 1234567890",
    "1234567890 1234567890 1234567890 1234567890 1234567890",
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890",
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890", //!OCLINT
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890", //!OCLINT
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890", //!OCLINT
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890", //!OCLINT
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890", //!OCLINT
    "1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890", //!OCLINT
    " 1",
    "  1",
    "  1 ",
    "  1  ",
    "  1 2 ",
    "  1 23 ",
    "  12 34  ",
    "  12  34  ",
    "   12   34   ",
    "   12   34   5",
    "   12   34   5 ",
    "   12   34   5 6",
    "0   12   34   5 6",
    "0   12   34   5 6  ",
    "                    ",
    "                      ",
    "                        ",
    "                            ",
    "                                    ",
    "                                                    ",
    "                                                                                     "
  };
  for (int len=1; len!=200; ++len)
  {
    for (const std::string& s: v)
    {
      const std::vector<std::string> w = Wordwrap(s,len);
      BOOST_CHECK(Unwordwrap(w) == s);
    }
  }
}

BOOST_AUTO_TEST_CASE(test_cmap_cannot_wordwrap_on_negative_lengths)
{
  const std::string text{"irrelevant"};
  BOOST_CHECK_THROW(
    Wordwrap(text, 0),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(test_cmap_create_tally)
{
  {
    const std::vector<std::string> v = {};
    const std::map<std::string, int> expected = {};
    const auto result = CreateTally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A" };
    const std::map<std::string, int> expected = { {"A", 1} };
    const auto result = CreateTally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A", "A" };
    const std::map<std::string, int> expected = { {"A", 2} };
    const auto result = CreateTally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A", "B" };
    const std::map<std::string, int> expected = { {"A", 1}, { "B", 1} };
    const auto result = CreateTally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A", "A", "B" };
    const std::map<std::string, int> expected = { {"A", 2}, {"B", 1} };
    const auto result = CreateTally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "B", "A", "A" };
    const std::map<std::string, int> expected = { {"A", 2}, {"B", 1} };
    const auto result = CreateTally(v);
    BOOST_CHECK(result == expected);
  }
}

BOOST_AUTO_TEST_CASE(test_cmap_is_on_travis)
{
  if (OnTravis())
  {
    BOOST_CHECK(OnTravis());
  }
}

BOOST_AUTO_TEST_CASE(test_cmap_save_file_to_vector_on_absent_file)
{
  BOOST_CHECK_THROW(
    SafeFileToVector("abs.ent"),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(test_cmap_save_file_to_vector_on_empty_file)
{
  std::string filename{"tmp"};
  //Create empty file
  {
    std::ofstream f(filename);
    f.close();
  }
  BOOST_CHECK(
    SafeFileToVector(filename).empty()
  );
  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(test_cmap_save_file_to_vector_on_file_with_one_line)
{
  std::string filename{"tmp"};
  //Create empty file
  {
    std::ofstream f(filename);
    f << "first and only line";
    f.close();
  }
  BOOST_CHECK_EQUAL(
    1, SafeFileToVector(filename).size()
  );
  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(test_cmap_save_file_to_vector_on_file_with_two_lines)
{
  std::string filename{"tmp"};
  //Create empty file
  {
    std::ofstream f(filename);
    f << "first line\n";
    f << "second and last line";
    f.close();
  }
  BOOST_CHECK_EQUAL(
    2, SafeFileToVector(filename).size()
  );
  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(test_cmap_save_file_to_vector_throws_on_absent_files)
{
  BOOST_CHECK_THROW(
    SafeFileToVector("abs.ent"),
    std::invalid_argument
  );
}

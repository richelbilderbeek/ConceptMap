#include "conceptmaphelper.h"

#include <fstream>
#include <vector>
#include <boost/test/unit_test.hpp>

using namespace ribi::cmap;

BOOST_AUTO_TEST_CASE(test_cmap_get_combinations_number_of_elements_must_be_correct)
{
  BOOST_CHECK(GetCombinations(std::vector<int>( {         } ) ).size() ==  1);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1        } ) ).size() ==  2);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2      } ) ).size() ==  4);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2,3    } ) ).size() ==  8);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2,3,4  } ) ).size() == 16);
  BOOST_CHECK(GetCombinations(std::vector<int>( {1,2,3,4,5} ) ).size() == 32);
}

BOOST_AUTO_TEST_CASE(test_cmap_get_combinations_elements_must_be_correct)
{
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
  }
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1,2 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    const std::vector<int> expected_2 = {2};
    const std::vector<int> expected_3 = {1,2};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_2));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_3));
  }
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1,2,3 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    const std::vector<int> expected_2 = {2};
    const std::vector<int> expected_3 = {3};
    const std::vector<int> expected_4 = {1,2};
    const std::vector<int> expected_5 = {1,3};
    const std::vector<int> expected_6 = {2,3};
    const std::vector<int> expected_7 = {1,2,3};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_2));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_3));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_4));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_5));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_6));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_7));
  }
  {
    const std::vector<std::vector<int> > v = GetCombinations(std::vector<int>( { 1,2,3,4 } ) );
    const std::vector<int> expected_0 = {};
    const std::vector<int> expected_1 = {1};
    const std::vector<int> expected_2 = {2};
    const std::vector<int> expected_3 = {3};
    const std::vector<int> expected_4 = {4};
    const std::vector<int> expected_5 = {1,2};
    const std::vector<int> expected_6 = {1,3};
    const std::vector<int> expected_7 = {1,4};
    const std::vector<int> expected_8 = {2,3};
    const std::vector<int> expected_9 = {2,4};
    const std::vector<int> expected_10 = {3,4};
    const std::vector<int> expected_11 = {1,2,3};
    const std::vector<int> expected_12 = {1,2,4};
    const std::vector<int> expected_13 = {1,3,4};
    const std::vector<int> expected_14 = {2,3,4};
    const std::vector<int> expected_15 = {1,2,3,4};
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_0));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_1));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_2));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_3));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_4));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_5));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_6));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_7));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_8));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_9));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_10));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_11));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_12));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_13));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_14));
    BOOST_CHECK(std::count(v.begin(),v.end(),expected_15));
  }
}

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

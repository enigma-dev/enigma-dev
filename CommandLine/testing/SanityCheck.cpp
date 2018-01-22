#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TestHarness.hpp"

namespace {


using std::map;
using std::string;
using std::vector;
typedef map<string, string> NameMap;

const char *const kSimpleTestDirectory = "CommandLine/testing/SimpleTests";
const char *const kDrivenTestDirectory = "CommandLine/testing/Tests";

void read_files(string directory,
                NameMap *games, NameMap *sources, NameMap *others) {
  boost::filesystem::path targetDir(directory);
  boost::filesystem::directory_iterator iter(targetDir), eod;
  BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)) {
    string fullname = i.filename().string(), filename = fullname, ext;
    size_t dot = filename.find_last_of('.');
    if (dot != 0 && dot != string::npos) {
      ext = filename.substr(dot + 1);
      for (size_t i = 0; i < ext.size(); ++i) {
        if (ext[i] >= 'A' && ext[i] <= 'Z') ext += 'a' - 'A';
      }
      filename = filename.substr(0, dot);
    }
    if (!games && others) games = others;
    if (!sources && others) sources = others;
    if (games && ext == "sog") {
      games->insert(NameMap::value_type(fullname, filename));
    } else if (sources && (ext == "cpp" || ext == "cc")) {
      sources->insert(NameMap::value_type(fullname, filename));
    } else if (others) {
      others->insert(NameMap::value_type(fullname, filename));
    }
  }
}

using std::cout;
using std::endl;

TEST(GTestHarness, SanityCheck) {
  NameMap games, sources, others;
  read_files(kDrivenTestDirectory, &games, &sources, &others);

  for (auto &game : games) {
    auto match = sources.find(game.second + ".cpp");
    if (match == sources.end()) match = sources.find(game.second + ".cc");
    if (match == sources.end()) {
      FAIL() << "Game \"" << game.first
             << "\" does not appear to have a matching test driver.";
    } else {
      sources.erase(match);
    }
  }
  if (sources.size()) {
    if (sources.size() == 1) {
      FAIL() << "Extra source file \"" << sources.begin()->first << '"';
    } else {
      FAIL() << "Found " << sources.size() << " extra source files, including "
             << '"' << sources.begin()->first << '"';
    }
  }
  if (others.size()) {
    if (others.size() == 1) {
      FAIL() << "Junk file \"" << others.begin()->first << '"';
    } else {
      FAIL() << "Found " << others.size() << " junk files, including "
             << '"' << others.begin()->first << '"';
    }
  }
}

void bitch_about_junk_files(const NameMap &junk) {
  if (junk.size()) {
    if (junk.size() == 1) {
      FAIL() << "Junk file \"" << junk.begin()->first << '"';
    } else {
      FAIL() << "Found " << junk.size() << " junk files, including "
             << '"' << junk.begin()->first << '"';
    }
  }
}

vector<string> enumerate_simple_games() {
  NameMap games, others;
  read_files(kSimpleTestDirectory, &games, nullptr, &others);
  bitch_about_junk_files(others);
  vector<string> result;
  for (auto &kv : games) {
    result.push_back(string(kSimpleTestDirectory) + "/" + kv.first);
  }
  return result;
}

class SimpleTestHarness : public testing::TestWithParam<string> {};

TEST_P(SimpleTestHarness, SimpleTestRunner) {
  string game = GetParam();
  TestConfig tc;
  tc.extensions = "Paths,GTest";
  int ret = TestHarness::run_to_completion(game, tc);
  if (!ret) return;
  switch (ret) {
    case TestHarness::ErrorCodes::BUILD_FAILED:
      FAIL() << "Building game \"" << game << "\" failed!";
    case TestHarness::ErrorCodes::LAUNCH_FAILED:
      FAIL() << "Could not launch game \"" << game << "\"! Linkage error? "
                "Shift in the spacetime continuum?";
    case TestHarness::ErrorCodes::SYSCALL_FAILED:
      FAIL() << "Universe collapsed while running game \"" << game << "\"! "
                "Job pre-empted? PCR? Shift in the spacetime continuum?";
    case TestHarness::ErrorCodes::GAME_CRASHED:
      FAIL() << "Game \"" << game << "\" did not exit normally! "
                "Access violation?";
    case TestHarness::ErrorCodes::TIMED_OUT:
      FAIL() << "Game \"" << game << "\" did not finish running in the time "
                "allotted, and has therefore been killed.";
    default:
      FAIL() << "Game \"" << game << "\" returned " << ret << ". "
                "Check log for other errors (possibly gTest-flavored).";
  }
}

INSTANTIATE_TEST_CASE_P(SimpleTests, SimpleTestHarness,
                        testing::ValuesIn(enumerate_simple_games()));


}  // namespace

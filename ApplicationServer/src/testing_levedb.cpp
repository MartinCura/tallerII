#include "../src/leveldb/include/leveldb/db.h"
#include <string>
#include "gtest/gtest.h"
#include <cassert>
#include <iostream>


TEST(GTest, WorkingTest) {

  EXPECT_EQ(1,1);

}

int main (int argc, char *argv[]) {
  printf ("Running main() from testing_levedb.cpp\n");
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
/*
int main(int argc, char *argv[]) {
	
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
  if (status.ok()) {
  	printf("%s\n", "hola milena" );
  }
  std::string str1 ("green");

  status = db->Put(leveldb::WriteOptions(), "Key", str1);
  std::string result;
  db->Get(leveldb::ReadOptions(), "Key", &result);
  std::cout << result;
  assert(1+1==2);
  printf("%d\n", str1.compare(result));
  assert(str1.compare(result)==0);
  return 0;
}
*/
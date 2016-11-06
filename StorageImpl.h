#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include "Storage.h"

using namespace std;

class StorageImpl : public Storage {
public:
  StorageImpl();
  ~StorageImpl();
  vector<string> getAllEntries();

private:
  string filepath;
};

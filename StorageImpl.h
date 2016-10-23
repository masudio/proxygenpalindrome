#include<string>
#include<vector>
#include<iostream>
#include<fstream>

using namespace std;

class StorageImpl {
public:
  StorageImpl();
  ~StorageImpl();
  vector<string> getAllEntries();

private:
  string filepath;
};

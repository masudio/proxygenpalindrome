#include "StorageImpl.h"


StorageImpl::StorageImpl() {
  filepath = "db/links.txt";
}

StorageImpl::~StorageImpl() {
}

vector<string> StorageImpl::getAllEntries() {
  ifstream file(filepath);
  vector<string> links;
  string line;
  if(file.is_open()) {
    while(getline(file, line)) {
      links.push_back(line);
    }
    file.close();
  }

  return links;
}

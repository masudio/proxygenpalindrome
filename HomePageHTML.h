
#include "StorageImpl.h"

class HomePageHTML {
public:
  HomePageHTML();
  virtual ~HomePageHTML();
  string getPage();

private:
  Storage* storage;
};


#include "HomePageHTML.h"

HomePageHTML::HomePageHTML() : storage{new StorageImpl} {}

HomePageHTML::~HomePageHTML() {
  delete storage;
}

string HomePageHTML::getPage() {
  vector<string> links = storage->getAllEntries();
  string html("<!doctype html><html><head><title>Masudio's Website</title></head><body>");
  for(auto s : links) {
    html = html + s + "<br />";
  }
  html = html + "</body></html>";

  return html;
}

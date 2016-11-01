
#include "HomePageHTML.h"

string HomePageHTML::getPage() {
  vector<string> links = storage.getAllEntries();
  string html("<!doctype html><html><head><title>Masudio's Website</title></head><body>");
  for(auto s : links) {
    html = html + s + "<br />";
  }
  html = html + "</body></html>";

  return html;
}

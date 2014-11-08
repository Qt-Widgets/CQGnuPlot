#ifndef CUNIX_FILE_H
#define CUNIX_FILE_H

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>

class CUnixFile {
 public:
  explicit CUnixFile(const char *filename="") :
   filename_(filename), fp_(0) {
  }

  explicit CUnixFile(const std::string &filename) :
   filename_(filename), fp_(0) {
  }

 ~CUnixFile() {
    close();
  }

  bool isValid() const { return (fp_ != 0); }

  bool open() {
    close();

    fp_ = fopen(filename_.c_str(), "r");

    if (! fp_) {
      std::cerr << "Failed to open'" << filename_ << "'" << std::endl;
      return false;
    }

    return true;
  }

  void close() {
    if (! fp_) return;

    fclose(fp_);

    fp_ = 0;
  }

  bool readLine(std::string &line) {
    if (! fp_ || feof(fp_))
      return false;

    line = "";

    char c;

    while (fp_ && ((c = fgetc(fp_)) != EOF)) {
      line += c;

      if (c == '\n') break;
    }

    return true;
  }

 private:
  std::string  filename_;
  FILE        *fp_;
};

#endif

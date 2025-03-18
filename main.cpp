#include "utfcpp-4.0.6/source/utf8/checked.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <tuple>
#include <unistd.h>
#include <vector>

using namespace std;

#ifdef __APPLE__
#include <sys/sysctl.h>
#endif

vector<string> split(string a, char delim) {
  vector<string> result;
  string left = a;

  while (left.find(delim) != std::string::npos) {
    int index = left.find_first_of(delim);
    result.push_back(left.substr(0, index));
    left = left.substr(index + 1, left.length());
  }

  result.push_back(left);
  return result;
}

string getOSIcon() {
#ifdef __APPLE__
  return "\033[37m \033[0m"; // Icono de macOS en blanco
#else
  ifstream os_release_file("/etc/os-release");
  if (!os_release_file.is_open()) {
    return "\033[33m \033[0m"; // Icono genérico si no se encuentra el archivo
  }

  string os_release_line;
  while (getline(os_release_file, os_release_line)) {
    if (split(os_release_line, '=')[0] == "ID") {
      string id = split(os_release_line, '=')[1];
      if (id == "arch") return "\033[34m \033[0m";
      if (id == "debian") return "\033[31m \033[0m";
      if (id == "ubuntu") return "\033[33m󰕈 \033[0m";
      if (id == "fedora") return "\033[34m \033[0m";
      if (id == "nixos") return "\033[34m \033[0m";
    }
  }
  return "\033[33m \033[0m";
#endif
}

void quit() {
  cout << "\033[2J\033[1;1H";
  exit(0);
}

int main() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  string ascii_art =
      "██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n"
      "██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n"
      "██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗  \n"
      "██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝  \n"
      "╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n"
      " ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝";

  string username = getenv("USER");
  string pwd = std::filesystem::current_path().string();
  string os_icon = getOSIcon();
  string hostname;

#ifdef __APPLE__
  char hostname_buf[256];
  size_t size = sizeof(hostname_buf);
  sysctlbyname("kern.hostname", hostname_buf, &size, NULL, 0);
  hostname = string(hostname_buf);
#else
  ifstream hostname_file("/etc/hostname");
  getline(hostname_file, hostname);
#endif

  pwd.replace(0, strlen(getenv("HOME")), "~");
  string subtitle = fmt::format("  {}     {}  {}{}", username, pwd, os_icon, hostname);

  cout << "\033[32m";
  string screen = fmt::format("{}\n{}\n", ascii_art, subtitle);
  cout << screen;
  cout << "\033[0m";

  return 0;
}

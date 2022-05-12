#pragma once

#include <sys/ioctl.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>


#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

#define DIR_COLOR 3
#define FILE_COLOR 9
#define SELECTED_COLOR 1
#define HOVER_COLOR 4
#define HINT_COLOR 6

#define HEADER_HEIGHT 10
#define PRINTABLE_HEIGHT 30
#define COLUMN_MAX_WIDTH 35

enum SelectionType{
  sel_file,
  sel_folder,
  sel_all,
};

enum Style{
  NORMAL=0,
  BOLD=1,
};


class Browse {
public:
  Browse();
  vector<string> Start();

  void SetStartPath(string path);
  void SetMaxSelections(int num=-1);
  void SetExtension(string extension);
  void SetSelectionType(SelectionType type);

private:
  void GetWinsize();
  void Move(int, int);
  void ClearScreen();
  void Print(int, int, string);
  void PrintHeader();

  void Select();
  bool IsSelected(string path);
  int GetSelectedIndex(string path);

  void UpdateDirs();
  void RemoveHidden();

  void PrintFile(string fname, int column=0);
  void PrintDirs(int column=0);
  string GetColored(string text, int color, int style=0);

  string HumanReadable(uintmax_t size);

  vector<directory_entry> all_dirs;

  int index;
  vector<string> selected_paths;
  bool hide_hidden_files;

  int count;
  int prev_count;

  int width;
  int height;

  int cursor_x;
  int cursor_y;

  string hint="";
  string extension="*";
  int max_selections=1;
  SelectionType selection_type;

  string stat_fname = ".browse_cpp";
  string start_path;
  string path;
};

#include "Browse.h"

Browse::Browse(){
  GetWinsize();

  index = 0;
  hide_hidden_files = true;

  selection_type = SelectionType::sel_file;

  cursor_x = 0;
  cursor_y = 0;

  stat_fname = string(getenv("HOME")) + "/" + stat_fname;
  if(exists(boost::filesystem::path(stat_fname))){
    FILE* f = fopen(stat_fname.c_str(), "r");
    char* line = NULL;
    size_t size = 0;
    getline(&line, &size, f);
    fclose(f);
    if(exists(boost::filesystem::path(string(line))) &&
    is_directory(boost::filesystem::path(string(line)))){
      start_path = string(line);
    }
    else{
      start_path = getenv("HOME");
    }
  }
  else{
    start_path = getenv("HOME");
  }

  max_selections = 1;
  extension = "*";
  selection_type = sel_all;
}

void Browse::SetStartPath(string path){
  this->start_path = path;
}
void Browse::SetExtension(string extension){
  this->extension = extension;
}
void Browse::SetSelectionType(SelectionType type){
  this->selection_type = type;
}
void Browse::SetMaxSelections(int num){
  this->max_selections = num;
}

vector<string> Browse::Start(){
  path = start_path;
  char command;

  char prev_commands[2];
  bool jump_clear = false;

  system("stty raw");

  while(true){
    GetWinsize();

    UpdateDirs();

    if(!jump_clear)
      ClearScreen();

    PrintDirs(0);
    PrintHeader();
    if(is_directory(all_dirs[index])){
      int prev_idx = index;
      string prev_path = path;
      path = all_dirs[index].path().string();
      index = -1;
      UpdateDirs();
      PrintDirs(1);
      index = prev_idx;
      path = prev_path;
      UpdateDirs();
    }else{
      PrintFile(all_dirs[index].path().string(), 1);
    }

    jump_clear = false;

    Move(0, height -1);
    cout << GetColored(hint, HINT_COLOR) << flush;
    hint = "";

    command = getchar();
    cout << "\r    \r" << flush;

    if(command == 'q'){
      selected_paths.clear();
      break;
    }
    if(command == 13){
      break;
    }

    // Remap if are arrows
    // arrow codes are 27 then 91 then 65~68
    if(prev_commands[0] == 27  && prev_commands[1] == 91){
      if(command >= 65 && command <= 68){
        if(command == 65)
          command = 'w';
        if(command == 66)
          command = 's';
        if(command == 67)
          command = 'd';
        if(command == 68)
          command = 'a';
      }
    }

    switch (command) {
      case 3:   //CTRL-C
        cout << "use q to quit" << endl;
      break;
      case 'a':
        if(path != "/"){
          string prev_path = path;

          path = boost::filesystem::path(path).parent_path().string();
          UpdateDirs();

          vector<directory_entry>::iterator pos;
          pos = find(all_dirs.begin(), all_dirs.end(), directory_entry(prev_path));
          if(pos != all_dirs.end()){
            index = pos - all_dirs.begin();
          }
          else{
            index = 0;
          }
        }
        else{
          hint = "You are in root, can't go back";
        }
      break;
      case 'd':
        if(is_directory(all_dirs[index].path())){
          path = all_dirs[index].path().string();
          index = 0;
        }
        else{
          hint = "It is a file. Use space to select";
        }
      break;
      case 's':
        index = (index + 1) % count;
      break;
      case 'w':
        index = (index - 1) % count;
        if(index < 0)
          index = count-1;
      break;
      case ' ':
        Select();
      break;
      case 'h':
        if(hide_hidden_files == false)
          hide_hidden_files = true;
        else
          hide_hidden_files = false;
      break;
    }

    prev_commands[0] = prev_commands[1];
    prev_commands[1] = command;
  }
  ClearScreen();
  system("stty cooked");

  if(selected_paths.size() > 0){
    std::ofstream stat(stat_fname);
    stat << boost::filesystem::path(selected_paths.at(0)).parent_path().string();;
    stat.close();
  }

  return selected_paths;
}

void Browse::ClearScreen(){
  Move(0,0);
  for(int i = 0; i < height; i++)
    cout << "\033[K" << "\r\n";
  Move(0,0);
}

void Browse::Move(int x, int y){
  cout << "\033[" << y << ";" << x << "H  " << flush;
}

void Browse::Print(int x, int y, string text){
  Move(x, y);
  cout << text << flush;
}

void Browse::PrintDirs(int column){
  int x = column * (COLUMN_MAX_WIDTH+2);
  if(x > width)
    return;

  Move(0, HEADER_HEIGHT);

  int color = DIR_COLOR;
  int style = NORMAL;

  string prepend = "";

  int scroll = index - PRINTABLE_HEIGHT/2;
  if(scroll < 0)
    scroll = 0;

  for(int i = scroll; i < (int)all_dirs.size() && i < PRINTABLE_HEIGHT + scroll; i++){
    color = DIR_COLOR;
    style = NORMAL;
    prepend = "";

    string fname = all_dirs[i].path().filename().string();

    if(is_regular_file(all_dirs[i].path()))
      color = FILE_COLOR;

    if(i == index){
      color = HOVER_COLOR;
      style = BOLD;

      prepend = "-> ";
    }
    if(IsSelected(all_dirs[i].path().string())){
      color = SELECTED_COLOR;
      style = BOLD;
      prepend = "--> ";
    }
    fname = prepend + fname;

    if(int(x + fname.size()) > width-1){
      fname.erase(fname.begin() + width - x-1, fname.end());
    }
    if(fname.size() > COLUMN_MAX_WIDTH){
      fname.erase(fname.end()+COLUMN_MAX_WIDTH-fname.size(), fname.end());
    }

    Move(x, HEADER_HEIGHT + i - scroll);
    cout << GetColored(fname, color, style);
  }
  cout << flush;
  Move(0, HEADER_HEIGHT + index - scroll);
}

string Browse::GetColored(string text, int color, int style){
  return  "\e[" + to_string(style) + ";3" + to_string(color) + "m" + text + "\e[0m";
}

void Browse::RemoveHidden(){
  int removed = 0;
  for(size_t i = all_dirs.size()-1; i > 0 ; i--){
    if(all_dirs[i].path().filename().string()[0] == '.'){
      all_dirs.erase(all_dirs.begin() + i);
      removed ++;
    }
  }
}

void Browse::Select(){
  if(selection_type == sel_all)
  {
    int idx = GetSelectedIndex(all_dirs[index].path().string());
    if(idx == -1)
      selected_paths.push_back(all_dirs[index].path().string());
    else
      selected_paths.erase(selected_paths.begin() + idx);
  }
  else if(selection_type == sel_folder && is_directory(all_dirs[index].path())){
    int idx = GetSelectedIndex(all_dirs[index].path().string());
    if(idx == -1)
      selected_paths.push_back(all_dirs[index].path().string());
    else
      selected_paths.erase(selected_paths.begin() + idx);
  }
  else if(selection_type == sel_file && is_regular_file(all_dirs[index])){
    if(extension == "*" || all_dirs[index].path().extension() == extension){
      int idx = GetSelectedIndex(all_dirs[index].path().string());
      if(idx == -1)
        selected_paths.push_back(all_dirs[index].path().string());
      else
        selected_paths.erase(selected_paths.begin() + idx);
    }else{
      hint = "The file does not match extension: " + extension;
    }
  }

  if(selection_type != -1)
    while((int)selected_paths.size() > max_selections)
      selected_paths.erase(selected_paths.begin());
}

bool Browse::IsSelected(string path){
  vector<string>::iterator pos;
  pos = find(selected_paths.begin(), selected_paths.end(), path);
  if(pos != selected_paths.end())
    return true;
  return false;
}

int Browse::GetSelectedIndex(string path){
  vector<string>::iterator pos;
  pos = find(selected_paths.begin(), selected_paths.end(), path);
  if(pos != selected_paths.end())
    return pos - selected_paths.begin();
  return -1;
}

void Browse::UpdateDirs(){
    all_dirs.clear();
    try{
      copy(directory_iterator(path), directory_iterator(), back_inserter(all_dirs));
    }catch(exception e){
      cout << "Error" << endl;
    }
    if(hide_hidden_files){
      RemoveHidden();
    }
    sort(all_dirs.begin(), all_dirs.end());
    count = all_dirs.size();
}

void Browse::GetWinsize(){
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  width = w.ws_col;
  height = w.ws_row;
}


void Browse::PrintHeader()
{
  int x = 1;
  int y = 1;
  int color = 3;
  int style = 1;

  string head = "Configuration: ";
  Print(x, y, GetColored(head, color, style));
  string select[] = {"File", "Folder", "All"};
  head = "Selection mode: " + select[selection_type];
  Print(x+3, y+1, GetColored(head, color, style));
  head = "Accepted extension: " + extension;
  Print(x+3, y+2, GetColored(head, color, style));
  head = "Max selections: " + to_string(max_selections);
  Print(x+3, y+3, GetColored(head, color, style));

  try{
      color = 6;
      style = 0;
      head  = "Current file size: ";
    if(!is_directory(all_dirs[index].path())){
      head += HumanReadable(file_size(all_dirs[index].path()));
    }
    else{
      head += "-";
    }
    Print(x, y+4, GetColored(head, color, style));
  }catch(exception e){
    cout << e.what() << endl;
  }
}

string Browse::HumanReadable(uintmax_t size){
  auto mantissa = size;
  int i{};
  for (; mantissa >= 1024.; mantissa /= 1024., ++i) { }
  mantissa = std::ceil(mantissa * 10.) / 10.;
  return to_string(mantissa) + "BKMGTPE"[i];
}

void Browse::PrintFile(string fname, int column){
  if(!boost::filesystem::is_regular_file(fname))
    return;
  if(!exists(fname))
    return;

  FILE* f = fopen(fname.c_str(), "r");
  char* line = NULL;
  size_t size = 0;
  int x = column * (COLUMN_MAX_WIDTH + 2);
  int count = 0;
  while(getline(&line, &size, f) != -1){
    if(count > PRINTABLE_HEIGHT)
      break;
    for(size_t i = 0; i < size; i++){
      if(line[i] == '\r')
        line[i] = ' ';
    }

    string line_str = string(line);
    if(line_str.size() > COLUMN_MAX_WIDTH){
      line_str.resize(COLUMN_MAX_WIDTH);
    }

    Move(x, HEADER_HEIGHT + count);
    cout << line_str;
    count ++;
  }

  fclose(f);
}

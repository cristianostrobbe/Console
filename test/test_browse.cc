#include "Browse.h"

int main(){
  Browse b;
  b.SetSelectionType(SelectionType::sel_all);
  b.Start();

  b.SetMaxSelections(1);
  b.SetExtension(".log");
  b.SetSelectionType(SelectionType::sel_file);

  b.Start();
}

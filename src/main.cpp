#include <iostream>
#include <string>
#include <sqlite3.h>

#include "mother_window.h"

int main()
{
    auto ui = MotherWindow::create();

    ui->run();
    return 0;
}

#include <iostream>
#include <iomanip>
#include "Table.h"

using namespace std;

int main()
{
    Table table(mysql_property);
    table << "姓名"  << "成绩" << "其他" << endl;
    table << "章三"  << std::right << 100 << std::left << "" << endl;
    table << "李四先生名字很长"  << std::right << 100000 << std::left << "" << endl;
    table << "王" << std::right << 1 << std::left << "" << endl;
    table << "六 六 | 六" << std::right << 3.1415926 << std::left << "123" << endl;
    cout << table << endl;

    table.set_property(default_property);
    cout << table << endl;

    const Table::Property cross_property
        = { "+-", "-+-", "-+", "+-", "-+-", "-+", "+-", "-+-", "-+",
            "| ", " | ", " |",'-',
            Table::upper_border | Table::lower_border | Table::horizon_line };
    table.set_property(cross_property);
    cout << table << endl;

    const Table::Property star_property
        =   { "* ", " * ", " *", "* ", " * ", " *", "* ", " * ", " *",
              "* ", " * ", " *", '*',
              Table::upper_border | Table::lower_border };
    table.set_property(star_property);
    cout << table << endl;

     const Table::Property hori_property
        = { "", "-", "", "", "-", "", "", "-", "", "", " ", "", '-',
            Table::upper_border | Table::lower_border | Table::horizon_line };
    table.set_property(hori_property);
    cout << table << endl;

    const Table::Property vert_property
        = { "", "", "", "", "", "", "", "", "", "", " | ", "", '\0', 0UL };
    table.set_property(vert_property);
    cout << table << endl;

    return 0;
}

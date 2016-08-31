#ifndef TABLE_H
#define TABLE_H

#include "basic_Table.h"

extern template class basic_Table<char>;
extern template class basic_Table<wchar_t>;

using Table = basic_Table<char>;
using wTable = basic_Table<wchar_t>;

const Table::Property default_property
    = { "", "", "", "", "", "", "", "", "", "", " ", "", '\0', 0UL };

const wTable::Property wdefault_property
    = { L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L" ", L"", L'\0', 0UL };

const Table::Property mysql_property
    = { "+-", "-+-", "-+", "+-", "-+-", "-+", "+-", "-+-", "-+",
        "| ", " | ", " |",'-',
        Table::upper_border | Table::lower_border | Table::header };

const wTable::Property wmysql_property
    = { L"+-", L"-+-", L"-+", L"+-", L"-+-", L"-+", L"+-", L"-+-", L"-+",
        L"| ", L" | ", L" |", L'-',
        Table::upper_border | Table::lower_border | Table::header };


#endif // TABLE_H

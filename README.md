# Table
A Tool to Format Cout to Pretty Tables

    If you want to cout student's scores:
    name	            score    others
    章三	            100
    李四先生名字很长	100000
    王	              1
    六六六	          3.1415926 123

    sample code:
    cout << "姓名"  << '\t' << "成绩" << '\t' << "其他" << endl;
    cout << "章三"  << '\t' << 100    << '\t' << "" << endl;
    cout << "李四先生名字很长"  << '\t' << 100000 << '\t' << "" << endl;
    cout << "王" << '\t' << 1 << '\t' << "" << endl;
    cout << "六 六 | 六" << '\t' << 3.1415926 << '\t' << "123" << endl;
    
![Image](https://github.com/apreak/Table/blob/master/pic1.png)

    untidy? use class 'Table' provided, you can print cout as a pretty. Here are some sample:
    sample code:
    Table table(mysql_property);
    table << "姓名"  << "成绩" << "其他" << endl;
    table << "章三"  << std::right << 100 << std::left << "" << endl;
    table << "李四先生名字很长"  << std::right << 100000 << std::left << "" << endl;
    table << "王" << std::right << 1 << std::left << "" << endl;
    table << "六 六 | 六" << std::right << 3.1415926 << std::left << "123" << endl;
    cout << table;

![Image](https://github.com/apreak/Table/blob/master/pic1.png)
![Image](https://github.com/apreak/Table/blob/master/pic1.png)

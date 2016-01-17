#include "Student.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void Menu()
{
    cout << "\t\t\t****************************\n";
    cout << "\t\t\t*   学生基本信息管理系统   *\n";
    cout << "\t\t\t*==========================*\n";
    cout << "\t\t\t*    1、录入学生信息       *\n";
    cout << "\t\t\t*    2、显示学生信息       *\n";
    cout << "\t\t\t*    3、查询学生信息       *\n";
    cout << "\t\t\t*    4、添加学生信息       *\n";
    cout << "\t\t\t*    5、统计学生信息       *\n";
    cout << "\t\t\t*    6、删除学生信息       *\n";
    cout << "\t\t\t*    7、修改学生信息       *\n";
    cout << "\t\t\t*    0、退出管理系统       *\n";
    cout << "\t\t\t****************************\n";
    cout << "\n\t\t\t请选择:";
}

int main()
{
    CStudent iStudent;
    int nChoose = 0;
    do
    {
        Menu();
        cin >> nChoose;
        switch (nChoose)
        {
            case 1:
                iStudent.Entering();
                break;
            case 2:
                iStudent.Show();
                break;
            case 3:
                iStudent.Query();
                break;
            case 4:
                iStudent.Entering();
                break;
            case 5:
                iStudent.Count();
                break;
            case 6:
                iStudent.Erase();
            case 7:
                iStudent.Modify();
            default:
                cout << "\t\t\t选择错误，请重新选择" << endl;
                break;

        }
    } while (nChoose != 0);
    return 0;
}

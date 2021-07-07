#include "Employee.h"

int getPositionIndex(QString postion){
    if(postion == "admin")
        return 0;
    if(postion == "manager")
        return 1;
    if(postion == "seller")
        return 2;

    return -1;
}

QString getPostionName(int position){
    if(position == 0)
        return "admin";
    if(position == 1)
        return "manager";
    if(position == 2)
        return "seller";

    return "";
}

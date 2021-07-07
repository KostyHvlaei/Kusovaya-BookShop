#include "Validating.h"

#include <QChar>

bool validate_password(QString Password){
    if (Password.length() > 30 || Password.length() < 7)
        return false;

    bool has_lower = false,
        has_upper = false,
        has_digit = false;

    for (int i = 0; i < Password.length(); ++i) {
        if (Password[i].isLower())
            has_lower = true;
        if (Password[i].isUpper())
            has_upper = true;
        if (Password[i].isDigit())
            has_digit = true;
    }

    return has_digit && has_lower && has_upper;
}

bool validate_phoneNumber(QString Phone){
    if (Phone.length() < 7)
        return false;

    for (int i = Phone[0] == '+' ? 1 : 0; i < Phone.length(); ++i) {
        if (!Phone[i].isDigit())
            return false;
    }
    return true;
}

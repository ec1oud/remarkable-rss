#ifndef UTIL_H
#define UTIL_H

class QString;

class Util
{
public:
    Util();

    static void show_error(const QString & title, const QString & message);
};

#endif // UTIL_H

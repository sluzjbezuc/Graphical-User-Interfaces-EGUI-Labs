#ifndef ABSTRACTDB_HPP
#define ABSTRACTDB_HPP

#include <QString>

class AbstractDB
{
protected:
    AbstractDB(void);

public:
    virtual void load(const QString& json) = 0;
    virtual void save(const QString& json) const = 0;
};

#endif // ABSTRACTDB_HPP

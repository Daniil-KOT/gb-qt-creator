#ifndef SPECSYMS_H
#define SPECSYMS_H

#include <QString>
#include <QMap>

class SpecSyms
{
public:
    SpecSyms();

    bool changed(QString text);

    QString getText();

private:
    QString modifiedText_;
    QString lastText_;
    const QMap<QString, QString> specSyms_ =
    {
      {"RUB", "₽"},
      {"EUR", "€"},
      {"PROM", "‰"},
      {"CPR", "©"},  // CPR - copyright
      {"RTM", "®"}   // RTM - registered trademark
    };
    qint32 startPos_;
};

#endif // SPECSYMS_H

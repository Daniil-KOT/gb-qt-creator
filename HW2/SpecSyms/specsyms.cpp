#include "specsyms.h"

#include <QMessageBox>

SpecSyms::SpecSyms()
{
    startPos_ = 0;
    modifiedText_ = "";
    lastText_ = "";
}

bool SpecSyms::changed(QString& text)
{
    modifiedText_ = text;

    quint32 length = qMin(qMin(text.length(), lastText_.length()), startPos_);

    for (quint32 i = 0; i < length; ++i)
    {
        if (lastText_[i] != text[i])
        {
            startPos_ = i;
            break;
        }
    }

    bool changed = false;

    while (true)
    {
        qint32 fnd = modifiedText_.indexOf("#@", startPos_);

        if (fnd == -1)
            break;

        modifiedText_.remove(fnd, 2);

        qint32 endPos_ = modifiedText_.indexOf("@", fnd);

        if (endPos_ == -1)
            break;

        if (modifiedText_[endPos_ - 1] == '#')
            continue;

        qint16 charNum = endPos_ - fnd;

        modifiedText_.remove(endPos_, 1);

        if (specSyms_.contains(modifiedText_.mid(fnd, charNum)))
        {
           QString sym = specSyms_[modifiedText_.mid(fnd, charNum)];
           modifiedText_.remove(fnd + 1, charNum - 1);
           modifiedText_.replace(fnd, 1, sym);
           startPos_ = fnd;
           changed = true;
        }
    }

    return changed;
}

QString SpecSyms::getText()
{
    lastText_ = modifiedText_;
    return lastText_;
}

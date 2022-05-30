#include "specsyms.h"

#include <QMessageBox>
#include <QDebug>

SpecSyms::SpecSyms()
{
    startPos_ = 0;
    modifiedText_ = "";
    lastText_ = "";
}

bool SpecSyms::changed(QString text)
{
    quint32 length = qMin(qMin(text.length(), lastText_.length()), startPos_);
    modifiedText_ = "";

    for (quint32 i = 0; i < length; ++i)
    {
        if (lastText_[i] != text[i])
        {
            startPos_ = i;
            break;
        }
    }

    bool changed = false;
    qint32 lp = -1;

    const QChar* tmpText = text.constData();

    for (qint32 fnd = text.indexOf("#@", startPos_);
         fnd != -1 && startPos_ != lp;
         fnd = text.indexOf("#@", startPos_))
    {
        modifiedText_.insert(modifiedText_.length(), &tmpText[0], fnd);

        lp = startPos_;
        startPos_ = fnd;

        qint32 endPos = text.indexOf('@', fnd + 2);

        if (-1 == endPos)
        {
            break;
        }
        else if ('#' == text[endPos - 1])
        {
            modifiedText_.insert(modifiedText_.length(), &tmpText[fnd], fnd + 2);
            startPos_ += 2;
            continue;
        }

        qint16 charNum = endPos - (fnd + 2);

        if (specSyms_.contains(text.mid(fnd + 2, charNum)))
        {
           QString sym = specSyms_[text.mid(fnd + 2, charNum)];

           modifiedText_.insert(modifiedText_.length(), &tmpText[fnd + 2], charNum - 1);
           modifiedText_.remove(fnd + 1, charNum - 1);
           modifiedText_.replace(fnd, 1, sym);
           startPos_ = endPos + 1;
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

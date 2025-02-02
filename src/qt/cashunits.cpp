// Copyright (c) 2016-2021 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2021 The Dash Core Developers
// Copyright (c) 2009-2021 The Bitcoin Developers
// Copyright (c) 2009-2021 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "cashunits.h"

#include "chainparams.h"
#include "primitives/transaction.h"

#include <QSettings>
#include <QStringList>

CashUnits::CashUnits(QObject* parent) : QAbstractListModel(parent),
                                              unitlist(availableUnits())
{
}

QList<CashUnits::Unit> CashUnits::availableUnits()
{
    QList<CashUnits::Unit> unitlist;
    unitlist.append(ODYNC);
    unitlist.append(mODYNC);
    unitlist.append(uODYNC);
    unitlist.append(satoshis);
    return unitlist;
}

bool CashUnits::valid(int unit)
{
    switch (unit) {
    case ODYNC:
    case mODYNC:
    case uODYNC:
    case satoshis:
        return true;
    default:
        return false;
    }
}

QString CashUnits::id(int unit)
{
    switch (unit) {
    case ODYNC:
        return QString("0dync");
    case mODYNC:
        return QString("m0dync");
    case uODYNC:
        return QString("u0dync");
    case satoshis:
        return QString("satoshis");
    default:
        return QString("???");
    }
}

QString CashUnits::name(int unit)
{
    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
        switch (unit) {
        case ODYNC:
            return QString("0DYNC");
        case mODYNC:
            return QString("m0DYNC");
        case uODYNC:
            return QString::fromUtf8("μ0DYNC");
        case satoshis:
            return QString("satoshis");
        default:
            return QString("???");
        }
    } else {
        switch (unit) {
        case ODYNC:
            return QString("t0DYNC");
        case mODYNC:
            return QString("mt0DYNC");
        case uODYNC:
            return QString::fromUtf8("μt0DYNC");
        case satoshis:
            return QString("tsatoshis");
        default:
            return QString("???");
        }
    }
}

QString CashUnits::symbol(int unit)
{
    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {    
        switch (unit) {
        case ODYNC:
            return QString("κ");
        case mODYNC:
            return QString("mκ");
        case uODYNC:
            return QString("uκ");
        case satoshis:
            return QString("satoshis");
        default:
            return QString("???");
        }
    } else {
        switch (unit) {
        case ODYNC:
            return QString("tκ");
        case mODYNC:
            return QString("mtκ");
        case uODYNC:
            return QString::fromUtf8("μtκ");
        case satoshis:
            return QString("tsatoshis");
        default:
            return QString("???");
        }
    }        
}

QString CashUnits::description(int unit)
{
    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
        switch (unit) {
        case ODYNC:
            return QString("Cash");
        case mODYNC:
            return QString("Milli-Cash (1 / 1" THIN_SP_UTF8 "000)");
        case uODYNC:
            return QString("Micro-Cash (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
        case satoshis:
            return QString("Ten Nano-Cash (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
        default:
            return QString("???");
        }
    } else {
        switch (unit) {
        case ODYNC:
            return QString("TestCash");
        case mODYNC:
            return QString("Milli-TestCash (1 / 1" THIN_SP_UTF8 "000)");
        case uODYNC:
            return QString("Micro-TestCash (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
        case satoshis:
            return QString("Ten Nano-TestCash (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
        default:
            return QString("???");
        }
    }
}

qint64 CashUnits::factor(int unit)
{
    switch (unit) {
    case ODYNC:
        return 100000000;
    case mODYNC:
        return 100000;
    case uODYNC:
        return 100;
    case satoshis:
        return 1;
    default:
        return 100000000;
    }
}

int CashUnits::decimals(int unit)
{
    switch (unit) {
    case ODYNC:
        return 8;
    case mODYNC:
        return 5;
    case uODYNC:
        return 2;
    case satoshis:
        return 0;
    default:
        return 0;
    }
}

QString CashUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if (!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (num_decimals <= 0)
        return quotient_str;

    return quotient_str + QString(".") + remainder_str;
}


// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString CashUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return symbol(unit) + QString(" ") + format(unit, amount, plussign, separators);
}

QString CashUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));

 return QString("<span style='white-space: nowrap;font-family: Arial;'>%1</span>").arg(str);
}

QString CashUnits::floorWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QSettings settings;
    int digits = settings.value("digits").toInt();

    QString result = format(unit, amount, plussign, separators);
    if (decimals(unit) > digits)
        result.chop(decimals(unit) - digits);

    return symbol(unit) + QString(" ") + result;
}

QString CashUnits::floorHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(floorWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));

    return QString("<span style='white-space: nowrap;font-family: Arial;'>%1</span>").arg(str);
}

bool CashUnits::parse(int unit, const QString& value, CAmount* val_out)
{
    if (!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if (parts.size() > 2) {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if (parts.size() > 1) {
        decimals = parts[1];
    }
    if (decimals.size() > num_decimals) {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if (str.size() > 18) {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if (val_out) {
        *val_out = retvalue;
    }
    return ok;
}

QString CashUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (CashUnits::valid(unit)) {
        amountTitle += " (" + CashUnits::symbol(unit) + ")";
    }
    return amountTitle;
}

int CashUnits::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant CashUnits::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < unitlist.size()) {
        Unit unit = unitlist.at(row);
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount CashUnits::maxMoney()
{
    return MAX_MONEY;
}

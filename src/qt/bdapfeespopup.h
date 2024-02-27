// Copyright (c) 2019-2021 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ODYNCASH_QT_BDAPFEESPOPUP_H
#define ODYNCASH_QT_BDAPFEESPOPUP_H

#include "bdap/fees.h"
#include "bdappage.h"
#include "odyncashunits.h"

#include <QDialog>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QTranslator>

bool bdapFeesPopup(QWidget *parentDialog, const opcodetype& opCodeAction, const opcodetype& opCodeObject, BDAP::ObjectType inputAccountType, int unit, int32_t regMonths = DEFAULT_REGISTRATION_MONTHS);

#endif // ODYNCASH_QT_BDAPFEESPOPUP_H
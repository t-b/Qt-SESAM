/*

    Copyright (c) 2015 Oliver Lau <ola@ct.de>, Heise Medien GmbH & Co. KG

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "global.h"
#include <QDebug>
#include <QSysInfo>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QSettings>
#include <QProcess>

const QString AppCompanyName = "ct";
const QString AppCompanyDomain = "http://www.ct.de/";
const QString AppName = "QtSESAM";
const QString AppVersion = QTSESAM_VERSION;
const QString AppURL = "https://github.com/ola-ct/Qt-SESAM";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "ola@ct.de";
#if QT_VERSION >= 0x050400
const QString AppUserAgent = QString("%1/%2 (+%3) Qt/%4 (%5; %6, %7)")
    .arg(AppName)
    .arg(AppVersion)
    .arg(AppURL)
    .arg(qVersion())
    .arg(QSysInfo::prettyProductName())
    .arg(QSysInfo::currentCpuArchitecture())
    .arg(QSysInfo::buildCpuArchitecture());
#else
const QString AppUserAgent = QString("%1/%2 (+%3) Qt/%4")
    .arg(AppName)
    .arg(AppVersion)
    .arg(AppURL)
    .arg(qVersion());
#endif
const QStringList BackupFilenameFilters = { QString("*-%1-backup.txt").arg(AppName) };

static const QString PortableFlagFile = "PORTABLE";
static const QString ReadMeFile = "README";
static const QString SettingsSubDir = "settings";
static bool gPortable = false;
bool isPortable(void) { return gPortable; }
void checkPortable(void) {
  const QString &cwd = QDir::currentPath();
  QFileInfo cwdfi(cwd);
  QFileInfo pfffi(cwd + "/" + PortableFlagFile);
  if (pfffi.isFile() && pfffi.isReadable() && cwdfi.isWritable()) {
    QDir(cwd).mkdir(SettingsSubDir);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, cwd + "/" + SettingsSubDir);
    QFile hintFile(cwd + "/" + SettingsSubDir + "/" + ReadMeFile);
    hintFile.open(QIODevice::WriteOnly);
    hintFile.write(QObject::tr("Deleting this directory with all its contents will erase all your application and domain settings. So be careful!").toUtf8());
    hintFile.close();
    gPortable = true;
  }
}


#ifdef Q_OS_WIN
bool isRunning(qint64 pid) {
  QProcess tasklistProcess;
  QStringList param = QStringList() << "/NH" << "/FO" << "CSV" << "/FI" << QString("PID eq %1").arg(pid);
  tasklistProcess.start("tasklist", param);
  tasklistProcess.waitForFinished();
  const QString &output = tasklistProcess.readAllStandardOutput();
  const QStringList &d = output.split(",");
  if (d.count() < 2)
    return false;
  QString pidStr = d.at(1);
  pidStr.replace("\"", "");
  return pidStr.toInt() == pid;
}
#else
bool isRunning(qint64 pid) {
  Q_UNUSED(pid);
  return false;
}
#endif

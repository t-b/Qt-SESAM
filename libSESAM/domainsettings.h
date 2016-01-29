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


#ifndef __DOMAINSETTINGS_H_
#define __DOMAINSETTINGS_H_

#include <QDebug>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QDateTime>
#include <QByteArray>
#include <QJsonDocument>

#include "securestring.h"

class DomainSettings {
public:
  DomainSettings(void);
  DomainSettings(const DomainSettings &);

  bool expired(void) const;
  QVariantMap toVariantMap(void) const;
  QJsonDocument toJsonDocument(void) const;
  QByteArray toJson(void) const;
  bool isEmpty(void) const;
  void clear(void);

  // returns the unique name for the domain settings
  QString getUniqueName(void) const;
  void replaceGroupName(QString oldName, QString newName);

  static DomainSettings fromVariantMap(const QVariantMap &);
  static DomainSettings fromJson(const QByteArray &);

  static const QByteArray DefaultSalt;
  static const QByteArray DefaultSalt_base64;
  static const int DefaultIterations;
  static const int DefaultPasswordLength;
  static const int DefaultSaltLength;

  static const QString DOMAIN_NAME;
  QString domainName;

  static const QString URL;
  QString url;

  static const QString USER_NAME;
  QString userName;

  static const QString LEGACY_PASSWORD;
  SecureString legacyPassword;

  static const QString NOTES;
  QString notes;

  static const QString SALT;
  QString salt_base64;

  static const QString ITERATIONS;
  int iterations;

  static const QString PASSWORD_LENGTH;
  int passwordLength;

  static const QString USED_CHARACTERS;
  QString usedCharacters;

  static const QString CDATE;
  QDateTime createdDate;

  static const QString MDATE;
  QDateTime modifiedDate;

  static const QString DELETED;
  bool deleted;

  // v3 settings

  static const QString EXTRA_CHARACTERS;
  QString extraCharacters;

  static const QString PASSWORD_TEMPLATE;
  QByteArray passwordTemplate;

  static const QString GROUP;
  QStringList groupHierarchy;

  static const QString EXPIRY_DATE;
  QDateTime expiryDate;

  static const QString TAGS;
  QStringList tags;

  static const QChar TagSeparator;
  static const QChar GroupSeparator;

private:

};


extern QDebug operator<<(QDebug debug, const DomainSettings &ds);


#endif // __DOMAINSETTINGS_H_

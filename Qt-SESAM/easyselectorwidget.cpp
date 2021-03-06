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

#include <limits>
#include "easyselectorwidget.h"
#include "util.h"
#include "password.h"
#include <QDebug>
#include <QSizePolicy>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPoint>
#include <QPixmap>
#include <QToolTip>
#include <QCryptographicHash>
#include <QThread>
#include <QtConcurrent>
#include <QElapsedTimer>
#include <QTime>

const int EasySelectorWidget::DefaultMinLength = 4;
const int EasySelectorWidget::DefaultMaxLength = 36;


class EasySelectorWidgetPrivate {
public:
  EasySelectorWidgetPrivate(void)
    : buttonDown(false)
    , length((EasySelectorWidget::DefaultMaxLength - EasySelectorWidget::DefaultMinLength) / 2)
    , oldLength(length)
    , complexity(Password::DefaultComplexityValue)
    , oldComplexity(Password::NoComplexityValue)
    , minLength(EasySelectorWidget::DefaultMinLength)
    , maxLength(EasySelectorWidget::DefaultMaxLength)
    , extraCharCount(Password::ExtraChars.count())
    , doAbortSpeedTest(false)
    , hashesPerSec(-1)
  { /* ... */ }
  ~EasySelectorWidgetPrivate()
  { /* ... */ }
  bool buttonDown;
  int length;
  int oldLength;
  int complexity;
  int oldComplexity;
  int minLength;
  int maxLength;
  int extraCharCount;
  QString passwordTemplate;
  QPixmap bgPixmap;
  bool doAbortSpeedTest;
  QFuture<void> speedTestFuture;
  qreal hashesPerSec;
};


EasySelectorWidget::EasySelectorWidget(QWidget *parent)
  : QWidget(parent)
  , d_ptr(new EasySelectorWidgetPrivate)
{
  QObject::connect(this, SIGNAL(speedTestFinished(qreal)), SLOT(onSpeedTestEnd(qreal)), Qt::QueuedConnection);
  QTimer::singleShot(500, this, SLOT(onSpeedTestBegin()));
  qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}


EasySelectorWidget::~EasySelectorWidget()
{
  Q_D(EasySelectorWidget);
  onSpeedTestAbort();
  d->speedTestFuture.waitForFinished();
}


QSize EasySelectorWidget::minimumSizeHint(void) const
{
  return QSize(200, 100);
}


void EasySelectorWidget::setMousePos(const QPoint &pos)
{
  Q_D(EasySelectorWidget);
  if (!d->bgPixmap.isNull()) {
    const int nX = d->maxLength - d->minLength + 1;
    const int nY = Password::MaxComplexityValue + 1;
    const int xs = d->bgPixmap.width() / nX;
    const int ys = d->bgPixmap.height() / nY;
    const int clampedX = clamp(pos.x(), 0, d->bgPixmap.width() - xs);
    const int clampedY = clamp(pos.y(), 0, d->bgPixmap.height() - ys);
    d->length = d->minLength + clampedX / xs;
    d->complexity = Password::MaxComplexityValue - clampedY / ys;
    update();
  }
}


void EasySelectorWidget::setExtraCharacters(const QString &extraChars)
{
  d_ptr->extraCharCount = extraChars.count();
  redrawBackground();
  update();
}


void EasySelectorWidget::setLength(int length)
{
  Q_D(EasySelectorWidget);
  d->length = length;
  if (d->length != d->oldLength) {
    emit valuesChanged(d->length, d->complexity);
    d->oldLength = d->length;
  }
  if (d->length > d->maxLength) {
    setMaxLength(d->length);
  }
  update();
}


void EasySelectorWidget::setComplexityValue(int complexityValue)
{
  Q_D(EasySelectorWidget);
  d->complexity = complexityValue;
  update();
}


int EasySelectorWidget::length(void) const
{
  return d_ptr->length;
}


int EasySelectorWidget::complexityValue(void) const
{
  return d_ptr->complexity;
}


void EasySelectorWidget::mouseMoveEvent(QMouseEvent *e)
{
  Q_D(EasySelectorWidget);
  if (d->buttonDown) {
    setMousePos(e->pos());
    if (d->length != d->oldLength || d->complexity != d->oldComplexity) {
      emit valuesChanged(d->length, d->complexity);
      d->oldLength = d->length;
      d->oldComplexity = d->complexity;
    }
  }
}


void EasySelectorWidget::mousePressEvent(QMouseEvent *e)
{
  Q_D(EasySelectorWidget);
  d->buttonDown = e->button() == Qt::LeftButton;
  if (d->buttonDown) {
    setMousePos(e->pos());
    if (d->length != d->oldLength || d->complexity != d->oldComplexity) {
      emit valuesChanged(d->length, d->complexity);
      d->oldLength = d->length;
      d->oldComplexity = d->complexity;
    }
  }
}


void EasySelectorWidget::mouseReleaseEvent(QMouseEvent *e)
{
  Q_D(EasySelectorWidget);
  if (e->button() == Qt::LeftButton) {
    d->buttonDown = false;
    if (d->length != d->oldLength || d->complexity != d->oldComplexity) {
      emit valuesChanged(d->length, d->complexity);
      d->oldLength = d->length;
      d->oldComplexity = d->complexity;
    }
  }
}


void EasySelectorWidget::paintEvent(QPaintEvent *)
{
  Q_D(EasySelectorWidget);
  if (!d->bgPixmap.isNull()) {
    const int nX = d->maxLength - d->minLength + 1;
    const int nY = Password::MaxComplexityValue + 1;
    const int xs = d->bgPixmap.width() / nX;
    const int ys = d->bgPixmap.height() / nY;
    QPainter p(this);
    p.drawPixmap(QPoint(0, 0), d->bgPixmap);
    p.setBrush(QColor(255, 255, 255, 208));
    p.setPen(Qt::transparent);
    p.drawRect(QRect(QPoint(xs * (d->length - d->minLength) + 1,
                            d->bgPixmap.height() - ys * (d->complexity + 1)),
                     QSize(xs - 1, ys - 1)));
  }
}


void EasySelectorWidget::resizeEvent(QResizeEvent *)
{
  redrawBackground();
}


bool EasySelectorWidget::event(QEvent *e)
{
  if (e->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
    QString helpText;
    if (tooltipTextAt(helpEvent->pos(), helpText)) {
      QToolTip::showText(helpEvent->globalPos(), helpText);
    }
    else {
      QToolTip::hideText();
    }
    return true;
  }
  return QWidget::event(e);;
}


static QColor red2yellow2green(qreal x)
{
  QColor color;
  x = clamp(x, 0.0, 1.0);
  if (x <= 0.5) {
    x *= 2;
    color.setRed(255);
    color.setGreen(255 * x);
  }
  else {
    x = 2 * x - 1;
    color.setRed(255 - 255 * x);
    color.setGreen(255);
  }
  return color;
}


void EasySelectorWidget::redrawBackground(void)
{
  Q_D(EasySelectorWidget);
  if (width() == 0 || height() == 0) {
    d->bgPixmap = QPixmap();
    return;
  }
  const int nX = d->maxLength - d->minLength + 1;
  const int nY = Password::MaxComplexityValue + 1;
  const int xs = (width() - 2) / nX;
  const int ys = (height() - 2) / nY;
  d->bgPixmap = QPixmap(QSize(xs * nX + 1, ys * nY + 1));
  QPainter p(&d->bgPixmap);
  for (int y = 0; y < nY; ++y) {
    for (int x = 0; x < nX; ++x) {
      p.fillRect(QRect(x * xs, d->bgPixmap.height() - y * ys - ys, xs, ys),
                 red2yellow2green(qLn(passwordStrength(x + d->minLength, y)) / 40).darker(168));
    }
  }
  p.setBrush(Qt::transparent);
  p.setPen(QPen(QBrush(QColor(0, 0, 0, 128)), 1));
  for (int x = 0; x <= nX; ++x) {
    p.drawLine(xs * x, 0, xs * x, d->bgPixmap.height());
  }
  for (int y = 0; y <= nY; ++y) {
    p.drawLine(0, ys * y, d->bgPixmap.width(), ys * y);
  }
}


static QString makeCrackDuration(qreal secs)
{
  QString crackDuration;
  if (secs < 1e-6) {
    crackDuration = QObject::tr("< 1 microsecond");
  }
  else if (secs < 1e-3) {
    crackDuration = QObject::tr("< 1 millisecond");
  }
  else if (secs < 1) {
    crackDuration = QObject::tr("~ %1 milliseconds").arg(qRound(1e3 * secs));
  }
  else if (secs < 60) {
    crackDuration = QObject::tr("~ %1 seconds").arg(qRound(secs));
  }
  else if (secs < 60 * 60) {
    crackDuration = QObject::tr("~ %1 minutes").arg(qRound(secs / 60));
  }
  else if (secs < 60 * 60 * 24) {
    crackDuration = QObject::tr("~ %1 hours").arg(qRound(secs / 60 / 60));
  }
  else if (secs < 60 * 60 * 24 * 365.24) {
    crackDuration = QObject::tr("~ %1 days").arg(qRound(secs / 60 / 60 / 24));
  }
  else {
    crackDuration = QObject::tr("~ %1 years").arg(secs / 60 / 60 / 24 / 365.2425, 0, 'g', 2);
  }
  return crackDuration;
}


bool EasySelectorWidget::tooltipTextAt(const QPoint &pos, QString &helpText) const
{
  if (d_ptr->bgPixmap.isNull())
    return false;
  const int xs = d_ptr->bgPixmap.width() / (d_ptr->maxLength - d_ptr->minLength + 1);
  const int ys = d_ptr->bgPixmap.height() / (Password::MaxComplexityValue + 1);
  const int length = pos.x() / xs + d_ptr->minLength;
  const int complexity = Password::MaxComplexityValue - pos.y() / ys;
  QString crackDuration = makeCrackDuration(tianhe2Secs(length, complexity));
  QString myCrackDuration = makeCrackDuration(mySecs(length, complexity));
  helpText = tr("%1 characters,\n"
                "est. crack time w/ Tianhe-2: %2,\n"
                "on your computer: %3")
      .arg(length)
      .arg(crackDuration)
      .arg(d_ptr->hashesPerSec < 0 ? tr("calculating ...") : myCrackDuration);
  return (d_ptr->minLength <= length) && (length <= d_ptr->maxLength);
}


qreal EasySelectorWidget::sha1Secs(int length, int complexityValue, qreal sha1PerSec) const
{
  int charCount = 0;
  if (qFuzzyIsNull(sha1PerSec)) {
    return std::numeric_limits<qreal>::infinity();
  }
  Password::Complexity complexity = Password::Complexity::fromValue(complexityValue);
  if (complexity.digits) {
    charCount += Password::Digits.count();
  }
  if (complexity.lowercase) {
    charCount += Password::LowerChars.count();
  }
  if (complexity.uppercase) {
    charCount += Password::UpperChars.count();
  }
  if (complexity.extra) {
    charCount += d_ptr->extraCharCount;
  }
  static const int Iterations = 1;
  const qreal perms = qPow(charCount, length);
  const qreal t2secs = perms * Iterations / sha1PerSec;
  return .5 * t2secs;

}


qreal EasySelectorWidget::tianhe2Secs(int length, int complexityValue) const
{
  return sha1Secs(length, complexityValue, 767896613647.0);
}


qreal EasySelectorWidget::mySecs(int length, int complexityValue) const
{
  return sha1Secs(length, complexityValue, d_ptr->hashesPerSec);
}


qreal EasySelectorWidget::passwordStrength(int length, int complexityValue) const
{
  const qreal t2y = tianhe2Secs(length, complexityValue) / 60 / 60 / 24 / 365.25;
  return t2y * 1e8;
}


void EasySelectorWidget::setMinLength(int minLength)
{
  Q_D(EasySelectorWidget);
  d->minLength = minLength;
  if (d->maxLength < d->minLength) {
    setMaxLength(d->minLength);
  }
  else {
    redrawBackground();
    update();
  }
}

void EasySelectorWidget::setMaxLength(int maxLength)
{
  Q_D(EasySelectorWidget);
  d->maxLength = maxLength;
  redrawBackground();
  update();
}


void EasySelectorWidget::onSpeedTestBegin(void)
{
  Q_D(EasySelectorWidget);
  QTimer::singleShot(3000, this, SLOT(onSpeedTestAbort()));
  d->speedTestFuture = QtConcurrent::run(this, &EasySelectorWidget::speedTest);
}


void EasySelectorWidget::onSpeedTestEnd(qreal hashesPerSec)
{
  Q_D(EasySelectorWidget);
  d->hashesPerSec = hashesPerSec;
}


void EasySelectorWidget::onSpeedTestAbort(void)
{
  Q_D(EasySelectorWidget);
  d->doAbortSpeedTest = true;
}


void EasySelectorWidget::speedTest(void)
{
  Q_D(EasySelectorWidget);
  static const int BufSize = 512 / 8;
  QByteArray data(BufSize, static_cast<char>(0));
  QCryptographicHash hash(QCryptographicHash::Sha1);
  QElapsedTimer t;
  t.start();
  qint64 n = 0;
  while (!d->doAbortSpeedTest && t.elapsed() < 5000) {
    for (auto d = data.begin(); d != data.end(); ++d) {
      *d = qrand() & 0xff;
    }
    hash.addData(data);
    hash.result();
    hash.reset();
    ++n;
  }
  const int coreCount = QThread::idealThreadCount() > 0 ? QThread::idealThreadCount() : 1;
  const qint64 nsecsElapsed = t.nsecsElapsed();
  if (nsecsElapsed > 0) {
    const qreal hashesPerSec = n * coreCount * 1e9 / nsecsElapsed;
    emit speedTestFinished(hashesPerSec);
  }
}

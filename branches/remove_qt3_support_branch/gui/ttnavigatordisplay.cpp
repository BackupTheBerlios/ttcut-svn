#include "ttnavigatordisplay.h"

#include <QPainter>

TTNavigatorDisplay::TTNavigatorDisplay(QWidget* parent)
  :QFrame(parent)
{
  setupUi( this );
}


void TTNavigatorDisplay::controlEnabled(bool enabled)
{
}

void TTNavigatorDisplay::setMinValue(int val)
{
  minValue = val;
}

void TTNavigatorDisplay::setMaxValue(int val)
{
  maxValue = val;
}

void TTNavigatorDisplay::resizeEvent(QResizeEvent* event)
{
  scaleFactor = navigatorDisplay->geometry().width() / (double)(maxValue-minValue);
  qDebug("scale: %lf", scaleFactor);
  qDebug("width: %d", navigatorDisplay->geometry().width());
}

void TTNavigatorDisplay::paintEvent(QPaintEvent *event)
{
  minValue = 0;
  maxValue = 5800;
  
  QPainter paint(this);

  QRect clientRect = navigatorDisplay->geometry();
  paint.fillRect(clientRect, QBrush(Qt::green));

  int start = clientRect.x() + (int)2900 * scaleFactor;
  int ende  = clientRect.x() + (int)1400 * scaleFactor;
  qDebug("start/ende: %d/%d", start, ende);
  QRect cutRect;
  cutRect.setRect(start, clientRect.y(), ende, clientRect.height());
  paint.fillRect(cutRect, QBrush(Qt::red));
}



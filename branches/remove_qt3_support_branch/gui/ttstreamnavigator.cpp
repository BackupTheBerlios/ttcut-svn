#include "ttstreamnavigator.h"

TTStreamNavigator::TTStreamNavigator(QWidget* parent)
  :QWidget(parent)
{
  setupUi( this );

  connect(videoSlider, SIGNAL(valueChanged(int)), SLOT(onNewSliderValue(int)));
}

void TTStreamNavigator::setTitle(const QString& title)
{
}

void TTStreamNavigator::controlEnabled(bool enabled)
{
  videoSlider->setEnabled(enabled);
}

QSlider* TTStreamNavigator::slider()
{
  return videoSlider;
}

void TTStreamNavigator::onNewSliderValue(int val)
{
  emit sliderValueChanged(val);
}



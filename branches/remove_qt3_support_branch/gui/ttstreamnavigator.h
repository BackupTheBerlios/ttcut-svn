
#ifndef TTSTREAMNAVIGATOR_H
#define TTSTREAMNAVIGATOR_H

#include "ui_streamnavigationwidget.h"

class TTStreamNavigator : public QWidget, public Ui::TTStreamNavigatorWidget
{
  Q_OBJECT

  public:
    TTStreamNavigator(QWidget* parent);

    void setTitle(const QString& title);
    void controlEnabled(bool enabled);
    QSlider* slider();

    public slots:
      void onNewSliderValue(int value);

signals:
    void sliderValueChanged(int value);
};

#endif //TTSTREAMNAVIGATOR_H

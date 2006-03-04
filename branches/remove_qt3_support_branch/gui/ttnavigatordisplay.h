
#ifndef TTNAVIGATORDISPLAY_H
#define TTNAVIGATORDISPLAY_H


#include "ui_navigatordisplaywidget.h"

class TTNavigatorDisplay : public QFrame, public Ui::TTNavigatorDisplayWidget
{
  Q_OBJECT

  public:
    TTNavigatorDisplay(QWidget* parent);

    void controlEnabled(bool enabled);
    void setMinValue(int val);
    void setMaxValue(int val);

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent* event);

  private:
    int    minValue;
    int    maxValue;
    double scaleFactor;
};

#endif //TTNAVIGATORDISPLAY_H

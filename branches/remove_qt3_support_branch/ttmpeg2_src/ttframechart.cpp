
#include "ttframechart.h"


TTFrameChart::TTFrameChart( QWidget* parent, const char* name, int w, int h )
  :QWidget( parent, name )
{

  if ( w > 0 && h > 0 )
    {
      setFixedWidth( w );
      setFixedHeight( h );
    }
}


TTFrameChart::~TTFrameChart()
{

}

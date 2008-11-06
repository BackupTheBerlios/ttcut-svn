#include "../gui/ttvideofileinfo.h"
#include "videofileinfoplugin.h"

#include <QtPlugin>

  VideoFilePlugin::VideoFilePlugin(QObject *parent)
: QObject(parent)
{
  initialized = false;
}

void VideoFilePlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
  if (initialized)
    return;

  initialized = true;
}

bool VideoFilePlugin::isInitialized() const
{
  return initialized;
}

QWidget *VideoFilePlugin::createWidget(QWidget *parent)
{
  return new AnalogClock(parent);
}

QString VideoFilePlugin::name() const
{
  return "AnalogClock";
}

QString VideoFilePlugin::group() const
{
  return "Display Widgets [Examples]";
}

QIcon VideoFilePlugin::icon() const
{
  return QIcon();
}

QString VideoFilePlugin::toolTip() const
{
  return "";
}

QString VideoFilePlugin::whatsThis() const
{
  return "";
}

bool VideoFilePlugin::isContainer() const
{
  return false;
}

QString VideoFilePlugin::domXml() const
{
  return "<widget class=\"AnalogClock\" name=\"analogClock\">\n"
    " <property name=\"geometry\">\n"
    "  <rect>\n"
    "   <x>0</x>\n"
    "   <y>0</y>\n"
    "   <width>100</width>\n"
    "   <height>100</height>\n"
    "  </rect>\n"
    " </property>\n"
    " <property name=\"toolTip\" >\n"
    "  <string>The current time</string>\n"
    " </property>\n"
    " <property name=\"whatsThis\" >\n"
    "  <string>The analog clock widget displays "
    "the current time.</string>\n"
    " </property>\n"
    "</widget>\n";
}

QString VideoFilePlugin::includeFile() const
{
  return "analogclock.h";
}

QString VideoFilePlugin::codeTemplate() const
{
  return "";
}

Q_EXPORT_PLUGIN2(customwidgetplugin, VideoFilePlugin)

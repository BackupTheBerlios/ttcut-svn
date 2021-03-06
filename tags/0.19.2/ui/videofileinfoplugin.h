#ifndef VIDEOFILEINFOPLUGIN_H
#define VIDEOFILEINFOPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class VideoFileInfoPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

  public:
    VideoFileInfoPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString codeTemplate() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

  private:
    bool initialized;
};

#endif

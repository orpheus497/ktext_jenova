// ##Script function and purpose: Defines the main plugin class for the Kate AI Assistant.
#pragma once

#include <KTextEditor/Plugin>
#include <QObject>
#include <QVariant>

class AiPluginView;

// ##Class purpose: Core plugin class responsible for creating views and config pages.
class AiPlugin : public KTextEditor::Plugin
{
    Q_OBJECT

public:
    // ##Method purpose: Constructor for the plugin.
    explicit AiPlugin(QObject *parent = nullptr, const QVariantList & = QVariantList());
    
    // ##Method purpose: Destructor.
    ~AiPlugin() override;

    // ##Method purpose: Factory method to create a new plugin view for a main window.
    QObject *createView(KTextEditor::MainWindow *mainWindow) override;

    // ##Method purpose: Returns the number of configuration pages provided by this plugin.
    int configPages() const override;
    
    // ##Method purpose: Returns the configuration page widget for the given index.
    KTextEditor::ConfigPage *configPage(int number, QWidget *parent) override;

private:
    QList<AiPluginView *> m_views;
};

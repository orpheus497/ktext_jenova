// ##Script function and purpose: Implements the main plugin class and its lifecycle.
#include "AiPlugin.h"
#include "AiPluginView.h"
#include "../config/AiConfigPage.h"

#include <KPluginFactory>
#include <KTextEditor/MainWindow>

K_PLUGIN_FACTORY_WITH_JSON(AiPluginFactory, "../jenovaktext.json", registerPlugin<AiPlugin>();)

// ##Method purpose: Constructor implementation.
AiPlugin::AiPlugin(QObject *parent, const QVariantList &)
    : KTextEditor::Plugin(parent)
{
}

// ##Method purpose: Destructor implementation.
AiPlugin::~AiPlugin()
{
}

// ##Method purpose: Instantiates the AiPluginView and attaches it to the main window.
QObject *AiPlugin::createView(KTextEditor::MainWindow *mainWindow)
{
    auto *view = new AiPluginView(this, mainWindow);
    m_views.append(view);
    return view;
}

// ##Method purpose: Indicates this plugin provides 1 configuration page.
int AiPlugin::configPages() const
{
    return 1;
}

// ##Method purpose: Creates and returns the configuration page instance.
KTextEditor::ConfigPage *AiPlugin::configPage(int number, QWidget *parent)
{
    // ##Condition purpose: Ensure we only return a page for index 0.
    if (number != 0) {
        return nullptr;
    }
    return new AiConfigPage(parent);
}

#include "AiPlugin.moc"

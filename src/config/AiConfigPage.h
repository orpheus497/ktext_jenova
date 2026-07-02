// ##Script function and purpose: Defines the configuration page for setting the AI backend URL and model options.
#pragma once
#include <KTextEditor/ConfigPage>

// ##Class purpose: Provides a GUI for the user to configure plugin settings.
class AiConfigPage : public KTextEditor::ConfigPage {
    Q_OBJECT
public:
    // ##Method purpose: Constructor for the configuration page.
    explicit AiConfigPage(QWidget *parent = nullptr);
    
    // ##Method purpose: Destructor.
    ~AiConfigPage() override = default;
    
    // ##Method purpose: Returns the short name of the configuration page.
    QString name() const override { return QStringLiteral("Jenova K Text"); }
    
    // ##Method purpose: Returns the full name of the configuration page.
    QString fullName() const override { return QStringLiteral("Jenova K Text Settings"); }
    
    // ##Method purpose: Returns the icon used in the configuration dialog sidebar.
    QIcon icon() const override { return QIcon::fromTheme(QStringLiteral("preferences-system")); }
    
    // ##Method purpose: Applies the currently selected settings to the active configuration.
    void apply() override;
    
    // ##Method purpose: Resets the settings page to the currently saved configuration.
    void reset() override;
    
    // ##Method purpose: Resets the settings page to the hardcoded default values.
    void defaults() override;

private Q_SLOTS:
    void configChanged();

private:
    class QLineEdit *m_endpointEdit;
};

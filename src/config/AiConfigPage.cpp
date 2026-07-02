// ##Script function and purpose: Implements the configuration page logic (currently inline in header).
#include "AiConfigPage.h"
#include <QFormLayout>
#include <QLineEdit>
#include <KSharedConfig>
#include <KConfigGroup>

// ##Method purpose: Constructor for the configuration page.
AiConfigPage::AiConfigPage(QWidget *parent)
    : KTextEditor::ConfigPage(parent)
{
    auto *layout = new QFormLayout(this);
    m_endpointEdit = new QLineEdit(this);
    layout->addRow(QStringLiteral("Llama.cpp Endpoint URL:"), m_endpointEdit);
    
    connect(m_endpointEdit, &QLineEdit::textChanged, this, &AiConfigPage::configChanged);
    
    reset();
}

// ##Method purpose: Emit changed signal to enable the apply button.
void AiConfigPage::configChanged()
{
    Q_EMIT changed();
}

// ##Method purpose: Applies the currently selected settings to the active configuration.
void AiConfigPage::apply()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(QStringLiteral("JenovaKText"));
    group.writeEntry("EndpointUrl", m_endpointEdit->text());
    config->sync();
}

// ##Method purpose: Resets the settings page to the currently saved configuration.
void AiConfigPage::reset()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(QStringLiteral("JenovaKText"));
    m_endpointEdit->setText(group.readEntry("EndpointUrl", QStringLiteral("http://127.0.0.1:8080")));
}

// ##Method purpose: Resets the settings page to the hardcoded default values.
void AiConfigPage::defaults()
{
    m_endpointEdit->setText(QStringLiteral("http://127.0.0.1:8080"));
}

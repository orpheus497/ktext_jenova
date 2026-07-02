add_definitions(-DTRANSLATION_DOMAIN=\"jenovaktext\")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(jenovaktext_SRCS
    plugin/AiPlugin.cpp
    plugin/AiPluginView.cpp
    config/AiConfigPage.cpp
    network/LlamaClient.cpp
    context/ContextManager.cpp
    ui/AiChatWidget.cpp
    completion/AiCompletionModel.cpp
)

kcoreaddons_add_plugin(jenovaktext INSTALL_NAMESPACE "kf6/ktexteditor")
target_sources(jenovaktext PRIVATE ${jenovaktext_SRCS})

target_link_libraries(jenovaktext
    KF6::TextEditor
    KF6::I18n
    KF6::CoreAddons
    KF6::XmlGui
    KF6::SyntaxHighlighting
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

install(FILES jenovaktextui.rc DESTINATION ${KDE_INSTALL_KXMLGUIDIR}/jenovaktext)

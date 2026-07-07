// ##Script function and purpose: Defines the test suite for the AiChatWidget class.
#pragma once

#include <QObject>

// ##Class purpose: Tests the behavior of the AiChatWidget's sendMessage function.
class TestAiChatWidget : public QObject {
    Q_OBJECT

public:
    explicit TestAiChatWidget(QObject *parent = nullptr);
    int runTests();

private:
    // ##Method purpose: Tests sending an empty message does not modify state.
    bool testSendMessageEmpty();

    // ##Method purpose: Tests the first message sent adds the system prompt and the user message.
    bool testSendMessageFirstMessage();

    // ##Method purpose: Tests subsequent messages append to history and update system prompt if needed.
    bool testSendMessageSubsequentMessage();
};

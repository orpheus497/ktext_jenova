// ##Script function and purpose: Tests the AiCompletionModel functionality using QTest and Mock objects for KTextEditor.
#include <QtTest>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QModelIndex>
#include "../src/completion/AiCompletionModel.h"

// Forward declarations to satisfy compiler before Mock classes
namespace KTextEditor {
    class Document;
    class View;
}

#include <KTextEditor/Document>
#include <KTextEditor/View>
#include "MockLlamaClient.h"

// ##Class purpose: Mock for KTextEditor::Document to provide text retrieval capabilities for testing.
class MockDocument : public KTextEditor::Document {
public:
    explicit MockDocument() : m_lines(0) {}

    // Minimal mock methods needed by AiCompletionModel::completionInvoked
    int lines() const override { return m_lines; }
    int lineLength(int line) const override { return m_lineLengths.value(line, 0); }
    bool replaceText(const KTextEditor::Range &range, const QString &s) override { Q_UNUSED(range); Q_UNUSED(s); return true; }

    // We implement text() returning a string based on range
    QString text(const KTextEditor::Range &range) const override {
        Q_UNUSED(range);
        return m_textToReturn;
    }

    // Setters for tests
    void setLines(int lines) { m_lines = lines; }
    void setLineLength(int line, int length) { m_lineLengths[line] = length; }
    void setTextToReturn(const QString& text) { m_textToReturn = text; }

private:
    int m_lines;
    QMap<int, int> m_lineLengths;
    QString m_textToReturn;
};

// ##Class purpose: Mock for KTextEditor::View to provide a mock Document pointer.
class MockView : public KTextEditor::View {
public:
    explicit MockView(MockDocument *doc) : m_doc(doc) {}

    KTextEditor::Document* document() const override { return m_doc; }

private:
    MockDocument *m_doc;
};


// ##Class purpose: The main test class for AiCompletionModel using QTest.
class TestAiCompletionModel : public QObject {
    Q_OBJECT

private Q_SLOTS:
    // ##Method purpose: Setup before each test case
    void init() {
        m_model = new AiCompletionModel(this);
        // Replace real LlamaClient with our mock to prevent network calls
        delete m_model->m_client;
        m_mockClient = new MockLlamaClient(m_model);
        m_model->m_client = m_mockClient;
    }

    // ##Method purpose: Cleanup after each test case
    void cleanup() {
        delete m_model;
    }

    // ##Method purpose: Tests completionInvoked with a valid view and document.
    void testCompletionInvoked_ValidView() {
        MockDocument doc;
        doc.setLines(10);
        doc.setLineLength(0, 50);
        doc.setTextToReturn("some text");

        MockView view(&doc);
        KTextEditor::Range range(KTextEditor::Cursor(0,0), KTextEditor::Cursor(0,10));

        QCOMPARE(m_model->rowCount(), 0);

        m_model->completionInvoked(&view, range, KTextEditor::CodeCompletionModel::AutomaticInvocation);

        // Verify state changes - should set waiting state row count to 1
        QCOMPARE(m_model->rowCount(), 1);

        // Verify that the LlamaClient was correctly invoked for autocomplete
        QVERIFY(m_mockClient->requestCompletionCalled);
        QCOMPARE(m_mockClient->m_lastPrefix, QStringLiteral("some text"));
        QCOMPARE(m_mockClient->m_lastSuffix, QStringLiteral("some text"));
    }

    // ##Method purpose: Tests completionInvoked with a null view.
    void testCompletionInvoked_NullView() {
        KTextEditor::Range range(KTextEditor::Cursor(0,0), KTextEditor::Cursor(0,0));

        QCOMPARE(m_model->rowCount(), 0);

        m_model->completionInvoked(nullptr, range, KTextEditor::CodeCompletionModel::AutomaticInvocation);

        // It sets the loading message row even if view is null, but then aborts before LlamaClient request.
        QCOMPARE(m_model->rowCount(), 1);
    }

    // ##Method purpose: Tests completionInvoked with a view that has a null document.
    void testCompletionInvoked_NullDocument() {
        MockView view(nullptr); // Null doc
        KTextEditor::Range range(KTextEditor::Cursor(0,0), KTextEditor::Cursor(0,0));

        QCOMPARE(m_model->rowCount(), 0);

        m_model->completionInvoked(&view, range, KTextEditor::CodeCompletionModel::AutomaticInvocation);

        QCOMPARE(m_model->rowCount(), 1);
    }

private:
    AiCompletionModel *m_model;
    MockLlamaClient *m_mockClient;
};

QTEST_MAIN(TestAiCompletionModel)
#include "TestAiCompletionModel.moc"

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

class MockDocument : public KTextEditor::Document {
    QString m_text;
public:
    MockDocument() = default;
    void setText(const QString& text) { m_text = text; }
    QString text(const KTextEditor::Range& r) const override {
        int startCol = r.start().column();
        int endCol = r.end().column();
        return m_text.mid(startCol, qMax(0, endCol - startCol));
    }
    int lines() const override { return 1; }
    int lineLength(int) const override { return m_text.length(); }
    QUrl url() const override { return QUrl(); }
};

class MockView : public KTextEditor::View {
    MockDocument m_doc;
public:
    MockView() = default;
    KTextEditor::Document* document() const override { return const_cast<MockDocument*>(&m_doc); }
    MockDocument* mockDoc() { return &m_doc; }
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
        QObject::connect(m_mockClient, &LlamaClient::completionReceived, m_model, &AiCompletionModel::onCompletionReceived);
    }

    // ##Method purpose: Cleanup after each test case
    void cleanup() {
        delete m_model;
    }

    // ##Method purpose: Tests completionInvoked with a valid view and document.
    void testCompletionInvoked_ValidView() {
        MockView mockView;
        mockView.mockDoc()->setText(QStringLiteral("some text"));
        auto *view = &mockView;

        KTextEditor::Range range(KTextEditor::Cursor(0,0), KTextEditor::Cursor(0,9));

        QCOMPARE(m_model->rowCount(), 0);

        m_model->completionInvoked(view, range, KTextEditor::CodeCompletionModel::AutomaticInvocation);

        // Verify state changes - should set waiting state row count to 1
        QCOMPARE(m_model->rowCount(), 1);

        // Verify that the LlamaClient was correctly invoked for autocomplete
        QVERIFY(m_mockClient->requestCompletionCalled);
        QCOMPARE(m_mockClient->m_lastPrefix, QStringLiteral("some text"));
        QCOMPARE(m_mockClient->m_lastSuffix, QStringLiteral(""));
    }

    // ##Method purpose: Tests completionInvoked with a null view.
    void testCompletionInvoked_NullView() {
        KTextEditor::Range range(KTextEditor::Cursor(0,0), KTextEditor::Cursor(0,0));

        QCOMPARE(m_model->rowCount(), 0);

        m_model->completionInvoked(nullptr, range, KTextEditor::CodeCompletionModel::AutomaticInvocation);

        // It sets the loading message row even if view is null, but then aborts before LlamaClient request.
        QCOMPARE(m_model->rowCount(), 1);
    }

    // ##Method purpose: Tests completionInvoked with a mock view that has a null document.
    void testCompletionInvoked_NullDocument() {
        // Create a minimal Test Double for View returning a null document
        class NullDocView : public KTextEditor::View {
        public:
            NullDocView(QWidget* parent = nullptr) : KTextEditor::View(parent) {}
            KTextEditor::Document* document() const override { return nullptr; }

            // Minimal implementations for pure virtual functions not used in completionInvoked test
            void setContextMenu(QMenu*) override {}
            QMenu* contextMenu() const override { return nullptr; }
            QMenu* defaultContextMenu(QMenu*) const override { return nullptr; }
        };

        NullDocView view;
        KTextEditor::Range range(KTextEditor::Cursor(0,0), KTextEditor::Cursor(0,0));

        QCOMPARE(m_model->rowCount(), 0);

        m_model->completionInvoked(&view, range, KTextEditor::CodeCompletionModel::AutomaticInvocation);

        // Ensure state aborted safely despite being invoked
        QCOMPARE(m_model->rowCount(), 1);
    }

private:
    AiCompletionModel *m_model;
    MockLlamaClient *m_mockClient;
};

QTEST_MAIN(TestAiCompletionModel)
#include "TestAiCompletionModel.moc"

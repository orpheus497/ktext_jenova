# Decisions Log

**Timestamp**: 2026-07-02 15:14

## Architecture: Reference Replication
* **Decision**: Extract `CommandTextEdit` into `AiChatInputWidget` and implement context menu hooks instead of deleting original classes or force-swapping whole codebases.
* **Justification**: Dumping an external repo into our project breaks FOSS rules and introduces massive tech debt. Integrating their UI logic (e.g. `@file` mentions and right-click menus) into our native classes preserves our clean `QNetworkAccessManager` backend and guarantees separation of concerns.

## UI: Code Completion Crash Fixes
* **Decision**: Replace `beginResetModel()` with safe `beginInsertRows()` and `beginRemoveRows()` in `AiCompletionModel`. Implement `MatchRole`.
* **Justification**: Calling `beginResetModel()` during asynchronous LLM streams aggressively destroys KTextEditor's internal UI delegates. When typing quickly, this causes Kate to segfault because the popup view is actively attempting to redraw invalidated memory. Furthermore, KTextEditor requires precise data filtering via `MatchRole` to prevent internal crashes.

**Timestamp**: 2026-07-02 14:43

## Architecture: LSP and Interpretation
* **Decision**: Retain Kate's basic `CodeCompletionModel` instead of writing a full out-of-process LSP server. Retain basic context aggregation instead of full project AST parsing.
* **Justification**: A Qt Plugin is fundamentally restricted from acting as an LSP server. Deep parsing is beyond the scope of a fast inference client. We mitigate this by ensuring the plugin operates correctly within its intended scope.


**Timestamp**: 2026-07-02 13:33

## Architecture: KF6 & Qt6 Native Plugin
* **Decision**: Target KDE Frameworks 6 exclusively.
* **Justification**: Modern KDE applications (Kate, KDevelop) have migrated to Qt6/KF6.
* **Impact**: Requires `kf6-*` packages and modern C++17/20.

## Backend: Local Llama.cpp REST API
* **Decision**: Communicate via HTTP POST using `QNetworkAccessManager`.
* **Justification**: Llama.cpp provides standard `/completion` and `/chat/completions` endpoints.

## Network: Async QNetworkReply Handling
* **Decision**: Overwrite ongoing requests by calling `abort()` and `deleteLater()` on the old pointers, and use `sender()` in slots.
* **Justification**: Prevents crashes or signal cross-talk when multiple requests (e.g., chat vs refactor) overlap asynchronously in `LlamaClient`.

## User Interface: Chat Interactions
* **Decision**: Implement `QEventFilter` on `QTextEdit` to capture `Enter` for sending messages, and auto-populate a welcome guide on load.
* **Justification**: Empty chat panels feel like non-functional stubs. Intercepting `Enter` matches standard chat UX and greatly enhances usability.

## Architecture: Native Integration
* **Decision**: Remove custom `ComparisonView` popup. Pipe LLM edits directly into `KTextEditor::Document` via `replaceText`.
* **Justification**: Custom popups fight the host IDE. Editing the native buffer lets the user rely on the IDE's built-in Git Diff and Undo functionality, shifting from a "slap-on app" to a deeply integrated native plugin.

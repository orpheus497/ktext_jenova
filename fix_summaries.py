with open(".devdocs/SUMMARIES.md", "r") as f:
    lines = f.readlines()
with open(".devdocs/SUMMARIES.md", "w") as f:
    f.write("**Timestamp**: 2026-07-21 00:22\n* **2026-07-21 00:22**: Secured directory traversal logic against infinite loops in `ContextManager.cpp`.\n\n")
    for i in range(len(lines)):
        if "2026-07-21 00:22" in lines[i]:
            continue
        if "- Secured directory traversal logic against infinite loops" in lines[i]:
            continue
        f.write(lines[i])

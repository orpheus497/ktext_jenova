with open("src/context/ContextManager.cpp", "r") as f:
    lines = f.readlines()
with open("src/context/ContextManager.cpp", "w") as f:
    for line in lines:
        if "// ##Action purpose: Check if a marker was actually added below" in line:
            continue
        f.write(line)

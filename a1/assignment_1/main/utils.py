import re;

def edgeTranspose(t):
    return (t[1],t[0]);

def parser(line):
    ops = re.findall(r"^(\S+)",line);
    on = re.findall(r"(\".*\")",line);
    data = re.findall(r"(\(\s*-?\d+\s*,\s*-?\d+\s*\))",line);

    if on is not None:
        on = on.lower();

    return ops,on,data;

def strToTup(str):
    pattern = r"\s*(-?\d+)\s*";
    nums = re.findall(pattern,str);
    return (int(nums[0]),int(nums[1]));

def formattedPrinting(V,E):
    print("V = {");
    for key,val in V.items():
        print(f"  {key}:  {val}");
    print("}");

    print("E = {");
    for edge in E:
        print(f"  <{edge[0]},{edge[1]}>,");
    print("}");

def plot(V,E):
    pass
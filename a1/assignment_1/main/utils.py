import re;

def edgeTranspose(t):
    return (t[1],t[0]);

def parser(line):
    ops = re.findall(r"^(\S+)",line);
    on = re.findall(r"(\".*\")",line);
    data = re.findall(r"(\(\s*-?\d+\s*,\s*-?\d+\s*\))",line);

    on = [_.lower() for _ in on];

    return ops,on,data;

def strToTup(str):
    pattern = r"\s*(-?\d+)\s*";
    nums = re.findall(pattern,str);
    return (float(nums[0]),float(nums[1]));

def formattedPrinting(V,E):
    print("V = {");
    for key,val in V.items():
        print(f"  {key}:  ({val[0]:.2f},{val[1]:.2f})");
    print("}");

    print("E = {");
    for edge in E:
        print(f"  <{edge[0]},{edge[1]}>,");
    print("}");

def plot(V,E):
    pass
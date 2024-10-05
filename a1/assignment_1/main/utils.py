import re;
from assignment_1.main.errors import *;

def edgeTranspose(t):
    return (t[1],t[0]);

def parser(line):
    pattern = r"(?:add|mod|rm|gg)(?:\s*)(?:\".*\"(?:\s*))?(?:(?:\(\s*-?\d+\s*,\s*-?\d+\s*\)\s*)*)";
    match = re.match(pattern,line);

    if match is None: raise InvalidInput();
    else:
        st,en = match.span();
        if st != 0 or en - st != len(line):
            raise InvalidInput();

    ops = re.findall(r"^(\S+)",line);
    on = re.findall(r"(\".*\")",line);
    data = re.findall(r"(\(\s*-?\d+\s*,\s*-?\d+\s*\))",line);

    if ops in ["add","mod"] and (len(on) != 1 or len(data) < 2):
        raise InvalidInput();
    elif ops == "rm" and (len(on) != 1 or len(data) > 0):
        raise InvalidInput();
    elif ops == "gg" and (len(on) != 0 or len(data) > 0):
        raise InvalidInput();

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

    E = list(E);

    for edge in E[:-1]:
        print(f"  <{edge[0]},{edge[1]}>,");
    else:
        if len(E) > 0:
            print(f"  <{E[-1][0]},{E[-1][1]}>");
    print("}");

def plot(V,E):
    pass
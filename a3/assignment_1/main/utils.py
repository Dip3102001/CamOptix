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

def formattedPrinting(V,E,file):
    print(f"V {len(V)}",file=file,flush=True);
    

    E = list(E);
    
    print("E {",end="",file=file,flush=True);
    for edge in E[:-1]:
        print(f"<{edge[0]+1},{edge[1]+1}>,",end="",file=file,flush=True);
    else:
        if len(E) > 0:
            print(f"<{E[-1][0]+1},{E[-1][1]+1}>",end="",file=file,flush=True);
    print("}",file=file,flush=True);

def plot(V,E):
    pass
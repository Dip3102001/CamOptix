#!/usr/bin/env python3
import sys;

from assignment_1.main.utils import *;
from assignment_1.main.map import Map;
from assignment_1.main.errors import *;

def main():
    map = Map();
    while True:

        line = sys.stdin.readline();
        if line == "":
            break;
        
        try:
            ops,on,data = parser(line);
            if len(ops) == 0 or ops[0] not in ["gg","mod","add","rm"]:
                raise InvalidInput();
    
            if not ops[0] == "gg":
                if ops[0] == "rm" and len(on) == 0:
                    raise ExpectedStreetName();
                if ops[0] in ["mod","add"]:
                    if len(on) == 0 or len(data) == 0:
                        raise ExpectedCoordinate();
    
            ops = ops[0];
            if ops in ["add","mod","rm"]:
                on = on[0];
                if ops in ["add","mod"]:
                    data = [strToTup(_) for _ in data];

            if ops == "add":
                    map.add(on,data);
            elif ops == "mod": 
                map.modify(on,data);
            elif ops == "rm":
                map.remove(on);
            elif ops == "gg":
                V,E = map.gg();
                formattedPrinting(V,E);

        except InvalidInput as e:
            print(e,file=sys.stderr);
            exit(1);
        
        except (ExpectedStreetName,ExpectedCoordinate) as e:
            print(e,file=sys.stderr);
            continue;
    
        except (StreetNotFound,StreetAlreadyPresent) as e:
            print(e,file=sys.stderr);
            continue;
    
    sys.exit(0);

if __name__ == "__main__":
    main();
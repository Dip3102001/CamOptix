#!/usr/bin/env python3
import sys;
import re;

class LineSegment():
    def __init__(self,x,y):
        self.st = x;
        self.en = y;

        x1,y1 = self.st;
        x2,y2 = self.en;

        self.m = None if x1 == x2 else (y2 - y1) / (x2 - x1);
        self.c = None if x1 == x2 else y1 - ((y2 - y1) / (x2 - x1)) * x1;

    def isEqual(self,point):
        x,y = point;
        if self.m is not None:
            return y == self.m * x + self.c;
        else:
            x1,y1 = self.st;
            x2,y2 = self.en;

            if y2 < y1:
                y1,y2 = y2,y1;

            return x == x1 and x == x2 and y1 <= y and y <= y2;

    def isOnLineSegment(self,point):
        x,y = point;
        x1,y1 = self.st;
        x2,y2 = self.en;

        if x2 < x1:
            x1,x2 = x2,x1;
        if y2 < y1:
            y1,y2 = y2,y1;

        return x1 <= x and x <= x2 and y1 <= y and y <= y2 and self.isEqual(point);

    def inBetween(self,point):
        x,y = point;
        x1,y1 = self.st;
        x2,y2 = self.en;

        if x2 < x1:
            x1,x2 = x2,x1;
        if y2 < y1:
            y1,y2 = y2,y1;
        
        return ((x1 < x and x < x2 and y1 < y and y < y2) or 
                (x1 == x2 and x1 == x and y1 < y and y < y2) or 
                (x1 < x and x < x2 and y1 == y2 and y1 == y)) and self.isEqual(point);
    
    def __and__(self,ano_line_seg):
        c1 = self.c;
        m1 = self.m;

        c2 = ano_line_seg.c;
        m2 = ano_line_seg.m;

        if m1 is None and m2 is None:
            return None;
        elif m1 is None:
            x = self.st[0];
            y = m2 * x + c2;
        elif m2 is None:
            x = ano_line_seg.st[0];
            y = m1 * x + c1;
        elif m1 == m2:
            if c1 == c2:
                ans = [];
                if self.inBetween(ano_line_seg.st):ans.append(ano_line_seg.st);
                if self.inBetween(ano_line_seg.en):ans.append(ano_line_seg.en);
                if ano_line_seg.inBetween(self.st):ans.append(self.st);
                if ano_line_seg.inBetween(self.en):ans.append(self.en);
                return ans;
            else: return None;
        else:
            x = (c2 - c1)/(m1 - m2);
            y = (m1 * c2 - c1 * m2)/(m1 - m2);
        
            print(x,y);
        try:
            x = x if not x.is_integer() else int(x);
        except AttributeError as a:
            pass

        try:
            y = y if not y.is_integer() else int(y);
        except AttributeError as a:
            pass
        
        
        return (x,y) if self.isOnLineSegment((x,y)) and ano_line_seg.isOnLineSegment((x,y)) else None;

    def __str__(self):
        return f"{self.st} <-> {self.en}";

    def __repr__(self) -> str:
        return str(self);

class Street():
    def __init__(self,name,path):
        self.name = name;
        self.lineSegments = [
            LineSegment(path[i-1],path[i]) for i in range(1,len(path))
        ];

    def __and__(self,ano_street):
        intersections = [];
        street_coords_having_intersection = [];

        for s in self.lineSegments:
            for r in ano_street.lineSegments:
                intersection = s & r;
                if isinstance(intersection,list):
                    for instance in intersection:
                        intersections.append(instance);
                        street_coords_having_intersection.append((s.st,s.en,r.st,r.en));
                elif intersection is not None:
                    intersections.append(intersection);
                    street_coords_having_intersection.append((s.st,s.en,r.st,r.en));
    
        return intersections,street_coords_having_intersection;

    def modifyPath(self,new_path):
        self.lineSegments = [
            LineSegment(new_path[i-1],new_path[i]) for i in range(1,len(new_path))
        ];

    def __str__(self):
        return f"{self.name} : {self.lineSegments}";

    def __repr__(self) -> str:
        return str(self);

class Map():
    def __init__(self):
        self.streets = [];

    def add(self,street_name, street_coords):
        streetPresent = False;
        for street in self.streets:
            if street.name == street_name:
                streetPresent = True;
        
        if streetPresent:
            raise Exception("Street already present. Try another name or use `mod` for modification");
        else:
            new_street = Street(street_name,street_coords);
            self.streets.append(new_street);

    def modify(self,street_name,new_street_coords):
        flagModify = False;
        for street in self.streets:
            if street.name == street_name:
                street.modifyPath(new_street_coords);
                flagModify = True;
        else:
            if not flagModify:
                raise Exception("No such Street present inside MAP");

    def remove(self,street_name):
        item = None;
        for street in self.streets:
            if street.name == street_name:
                item = street;

        if item is None:
            raise Exception("No such street present inside MAP");
        else:
            self.streets.remove(item);

    def gg(self):
        V_set = set();
        V_index = {};
        V_index_inv = {};
        V = {};
        E = set();

        for i in range(len(self.streets)-1):
            for j in range(i+1,len(self.streets)):
                intersections, streets_coords = self.streets[i] & self.streets[j];

                for intersection,streets_coord in zip(intersections,streets_coords):
                    if intersection in V.keys():
                        V[intersection] += streets_coord;
                    else:
                        V[intersection] = streets_coord;

        for key in V.keys():
            for end_point in V[key]:
                V_set.add(end_point);
            V_set.add(key);

        for idx,items in enumerate(V_set):
            V_index[idx] = items;
            V_index_inv[items] = idx;

        intersections = list(V.keys());

        print(intersections);

        for i in range(0,len(intersections)):
            point_1 = intersections[i];

            for j in range(len(V[point_1])):
                end_point = V[point_1][j];

                if point_1 == end_point:
                    continue;
                else:
                    for k in range(0,len(intersections)):
                        if k != i:
                            if LineSegment(point_1,end_point).inBetween(intersections[k]):
                                end_point = intersections[k];
                    else:
                        edge = (V_index_inv[point_1],V_index_inv[end_point]);
                        if edgeTranspose(edge) not in E:
                            E.add(edge);


        return V_index,E;

def edgeTranspose(t):
    return (t[1],t[0]);

def parser(line):
    ops = re.findall(r"^(\S+)",line);
    on = re.findall(r"(\".*\")",line);
    data = re.findall(r"(\(\s*-?\d+\s*,\s*-?\d+\s*\))",line);

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


def main():

    map = Map();
    while True:

        line = sys.stdin.readline();
        if line == "":
            break;
        
        ops,on,data = parser(line);
        
        flagError = False;
        if not ops[0] == "gg":
            if ops[0] == "rm" and len(on) == 0:
                flagError = True;
                print("Street name is Expected");
            if ops[0] in ["mod","add"]:
                if len(on) == 0:
                    flagError = True;
                    print("Street name is Expected",end=" ");
                if len(data) == 0:
                    flagError = True;
                    print("co-ordinate is Expected",end=" ");
                if flagError:
                    print();

        if not flagError:
            ops = ops[0];
            if ops in ["add","mod","rm"]:
                on = on[0];
                if ops in ["add","mod"]:
                    data = [strToTup(_) for _ in data];

            try:
                if ops == "add":
                    map.add(on,data);
                elif ops == "mod": 
                    map.modify(on,data);
                elif ops == "rm":
                    map.remove(on);
                elif ops == "gg":
                    V,E = map.gg();
                    formattedPrinting(V,E);
            except Exception as e:
                print(e);

    print("Finished reading input");
    sys.exit(0);

if __name__ == "__main__":
    main();
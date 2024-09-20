from assignment_1.main.street import Street;
from assignment_1.main.linesegment import LineSegment;
from assignment_1.main.errors import *;
from assignment_1.main.utils import *;

class Map():
    def __init__(self):
        self.streets = [];

    def add(self,street_name, street_coords):
        streetPresent = False;
        for street in self.streets:
            if street.name == street_name:
                streetPresent = True;
        
        if streetPresent:
            raise StreetAlreadyPresent();
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
                raise StreetNotFound();

    def remove(self,street_name):
        item = None;
        for street in self.streets:
            if street.name == street_name:
                item = street;

        if item is None:
            raise StreetNotFound();
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

if __name__ == "__main__":
    map = Map();
    print("map is created..");
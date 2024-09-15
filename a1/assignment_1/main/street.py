from assignment_1.main.linesegment import LineSegment;

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

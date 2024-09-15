import unittest;
from assignment_1.main.linesegment import LineSegment;

class TestLineSegmentIntersection(unittest.TestCase):
    def test_intersection(self):
        tests = [[((0,0),(0,4)),((0,2),(2,2))],
                 [((0,0),(5,0)),((5,5),(5,-5))],
                 [((-4,0),(200,0)),((0,70),(0,-751))],
                 [((0,4),(4,0)),((0,0),(4,4))],
                 [((0,0),(0,4)),((2,2),(-2,6))],
                 [((4,8),(2,3)),((2,3),(-2,5))],
                 [((5,11),(8,17)),((4,9),(9,19))],
                 [((0,0),(0,10)),((0,2),(0,8))],
                 [((0,0),(10,0)),((2,0),(8,0))]];

        results = [(0,2),
                   (5,0),
                   (0,0),
                   (2,2),
                   (0,4),
                   (2,3),
                   [(5,11),(8,17)],
                   [(0,2),(0,8)],
                   [(2,0),(8,0)]];

        for testcase,result in zip(tests,results):
            point1,point2 = testcase;

            point1_st,point1_en = point1;
            point2_st,point2_en = point2;

            linesegment1 = LineSegment(point1_st,point1_en);
            linesegment2 = LineSegment(point2_st,point2_en);

            intersection = linesegment1 & linesegment2;

            self.assertEqual(intersection,result);


if __name__ == "__main__":
    unittest.main();
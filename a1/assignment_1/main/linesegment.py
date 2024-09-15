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
            if c1 == c2:
                ans = [];
                if self.inBetween(ano_line_seg.st):ans.append(ano_line_seg.st);
                if self.inBetween(ano_line_seg.en):ans.append(ano_line_seg.en);
                if ano_line_seg.inBetween(self.st):ans.append(self.st);
                if ano_line_seg.inBetween(self.en):ans.append(self.en);
                return ans;
            else: return None;
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

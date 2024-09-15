class ExpectedStreetName(Exception):
    def __init__(self):
        self.message = "Error: Street Name is Expected";

    def __str__(self):
        return self.message;

class ExpectedCoordinate(Exception):
    def __init__(self):
        self.message = "Error: Co-ordinate is Expected";

    def __str__(self):
        return self.message;

class StreetNotFound(Exception):
    def __init__(self):
        self.message = "Error: Street not found";

    def __str__(self):
        return self.message;

class StreetAlreadyPresent(Exception):
    def __init__(self):
        self.message = "Error: Street already present";

    def __str__(self):
        return self.message;

class InvalidInput(Exception):
    def __init__(self):
        self.message = "Invalid input provided";

    def __str__(self):
        return self.message;
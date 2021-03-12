#Deep Space Food Challenge - BioBox - Model

day = 0

class ShrimpClass:
    def __init__(self, population):
        self.population = population
        self.ages = [0] * population
    
    def feed(self, n):
        Larvae.population -= n
        self.population += n

class LarvaeClass:
    def __init__(self, population):
        self.population = population

    def feed(self, n):
        Plants.population -= n
        self.population += n

class PlantsClass:
    def __init__(self, population):
        self.population = population
    
    def grow(self, n):
        self.population += n

Shrimp = ShrimpClass(10)
Larvae = LarvaeClass(100)
Plants = PlantsClass(50)

def print_populations():
    print("Shrimp: " + str(Shrimp.population))
    print("Larvae: "  + str(Larvae.population))
    print("Plants: " + str(Plants.population))
    print()

def increment_day(n):
    Shrimp.feed(n)
    Larvae.feed(n)
    Plants.grow(n)
    global day
    day += n

def main():
    print_populations()
    increment_day(1)
    print_populations()

main()

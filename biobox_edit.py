import simpy
import random
import statistics
#import numpy

class BioBox():
    def __init__(self, env):
        self.env = env

def age(env, birth):
    return env.now - birth    

def shrimp_egg_death_chance(env, biobox):
    #chance = 2(4 - biobox.salinity)
    return 0.8

def shrimp(env, biobox):
    #Egg
    if random.random() < shrimp_egg_death_chance(env, biobox):
        alive = False
        return

    # Birth
    alive = True
    birth = env.now
    biobox.shrimp_pop += 1
    #print("time: " + str(env.now) + ", pop is " + str(biobox.shrimp_pop) + ", birth")

    # Life
    while alive:
        # Feed

        # Mate
        if (random.random() < 0.1) & (biobox.food_for_shrimp > 0) & (age(env, birth) > 5):
            biobox.food_for_shrimp -= 1 
            yield env.timeout(2)
            eggs = 20 + int(10 * random.random())
            for i in range(eggs):
                env.process(shrimp(env, biobox))

        # Die?
        if (random.random() < 0.1) | (biobox.food_for_shrimp <= 0):
            alive = False
            break

        # Pass time
        yield env.timeout(1)

    # Death
    death = age(env, birth)
    biobox.shrimp_pop -= 1
    #print("time: " + str(env.now) + ", pop is " + str(biobox.shrimp_pop) + ", death at " + str(death))
    biobox.obituary_ages.append(death)

def biobox_response(env, biobox):
    while True:
        biobox.food_for_shrimp += 1
        yield env.timeout(1)
        if biobox.shrimp_pop <= 0:
            break

def data_collector(env, biobox):
    while True:
        print(biobox.shrimp_pop)
        yield env.timeout(1)
        if biobox.shrimp_pop <= 0:
            break

def start(env, biobox):
    # Create BioBox data
    biobox.shrimp_pop = 0
    biobox.init_shrimp_pop = 10
    biobox.obituary_ages = []
    biobox.food_for_shrimp = 100

    #Create initial shrimp population
    for i in range(biobox.init_shrimp_pop):
        env.process(shrimp(env, biobox))

    #Create BioBox Response
    env.process(biobox_response(env, biobox))

    #Create Data Collector
    env.process(data_collector(env, biobox))
    
    #seems neccesarry for generator
    yield env.timeout(0)

def main():
    # Setup
    random.seed()
    env = simpy.Environment()
    biobox = BioBox(env)

    # Run the simulation
    env.process(start(env, biobox))
    env.run()

    # Display Results
    print("Time of simulation: " + str(env.now))
    print("Avg age at death: " + str(statistics.mean(biobox.obituary_ages)))
    print("Food for shrimp: " + str(biobox.food_for_shrimp))

if __name__ == "__main__":
    main()

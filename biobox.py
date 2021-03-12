import simpy
import random
import statistics

class BioBox():
    def __init__(self, env):
        self.env = env

def age(env, birth):
    return env.now - birth

def shrimp(env, biobox):
    # Birth
    alive = True
    birth = env.now
    biobox.shrimp_pop += 1
    #print("time: " + str(env.now) + ", pop is " + str(biobox.shrimp_pop) + ", birth")

    # Life
    while alive:
        # Feed

        # Mate
        if random.random() < 0.1:
            yield env.timeout(2)
            env.process(shrimp(env, biobox))

        # Die?
        if random.random() < .1:
            alive = False
            break

        # Pass time
        yield env.timeout(1)

    # Death
    death = age(env, birth)
    biobox.shrimp_pop -= 1
    #print("time: " + str(env.now) + ", pop is " + str(biobox.shrimp_pop) + ", death at " + str(death))
    biobox.obituary_ages.append(death)

def start(env, biobox):
    # Create BioBox data
    biobox.shrimp_pop = 0
    biobox.init_shrimp_pop = 10
    biobox.obituary_ages = []

    #Create initial shrimp population
    for i in range(biobox.init_shrimp_pop):
        env.process(shrimp(env, biobox))
    yield env.timeout(0) #seems neccesarry for generator

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

if __name__ == "__main__":
    main()

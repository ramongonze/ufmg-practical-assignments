import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

variables = {'F'}
terminals = {'+', '-'}
axiom = "F++F++F"

# Variables' rules
varRules = {
    'F': "F-F++F-F"
}

# Terminals' rules: Each pair (key,value) indicates
# that for a given 'key', the turtle will turn 'value' degrees
# in clockwise.
# PS: The angles must be in radius
terRules = {
    '+': math.pi/3.0, # 60 degrees
    '-': (math.pi*5)/3.0 # 300 degrees
}

# Set animation=True if you want to see an animated graphic among the iteractions
animation = False

# Number of iteractions (must be 0 if animation=True)
iteractions = 6

# Used to create an animation among the iteractions
# if animation:
fig = plt.figure()
p1 = fig.add_subplot(111)

if animation:
    ani = animation.FuncAnimation(fig, draw, interval=1000)
else:
    # Draw only one state of the fractal: the number of iteractions
    draw()

# Returns a string containing the turtle's trail
# @Parameter iteractions: Used to animate the graphic among iteractions
def buildTrail(animation):
    global variables, terminals, axiom, varRules, terRules, iteractions

    trail = axiom
    for _ in range(iteractions):
        i = 0
        while i < len(trail):
            if trail[i] in variables:
                rule = varRules[trail[i]]

                if 0 < i < len(trail)-1:
                    trail = trail[:i] + rule + trail[i+1:]
                elif i > 0 and i >= len(trail)-1:
                    trail = trail[:i] + rule
                elif i == 0 and i < len(trail)-1:
                    trail = rule + trail[i+1:]
                else:
                    trail = rule

                i += len(rule)
            else:
                i += 1

    # Iteractions is inscreased by 1 to generate the animation
    if animation:
        iteractions += 1

    return trail

# Given a string containing variables and terminals, draw the turtle's trail
# Parameter @trail: String containing the turtle's trail
def draw():
    global animation, p1, variables, terRules, terminals, axiom, varRules, iteractions

    # Initial position
    x1, y1 = 0,0
    # Initial angle
    theta = 0.0

    trail = buildTrail(animation)

    p1.clear()
    for c in trail:
        if c in variables:
            x2 = x1 + math.cos(theta)
            y2 = y1 + math.sin(theta)
            p1.plot([x1,x2], [y1,y2], color='blue')

            # Update the inital point
            x1, y1 = x2, y2
        else:
            theta = (theta + terRules[c])%(2*math.pi)

# Show the trail in a graphic
plt.show()

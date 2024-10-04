from tools.FluxCalculator import FluxCalculator




if __name__=='__main__':

    a = 0.274/2.0
    b = 0.274/2.0

    d = [1.1, 0.001, 0.8, 0.9]
    nlayer = [8, 1, 8, 8]
    eff = [0.806063189301508, 1.0, 0.806063189301508, 0.806063189301508]
    #eff = [1.0, 1.0]
    I0 = 72
    t = 100000.
    x = 0
    y = 0
    stepx = 0.005
    stepphi = 0.005
    for i, id in enumerate(d):
        f = FluxCalculator(stepx, stepphi, a, b, d[i], nlayer[i], eff[i], I0)
        N = f.giveNMuons(t)
        print('Number of muons for d:', d[i], 'nlayer:',  nlayer[i], 'eff:', eff[i], 'is:', int(N))




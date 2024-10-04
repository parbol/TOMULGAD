import math

#####################################################################
# This program estimates the expected flux of muons passing through #
# all the layers of a detector with N layers separated a distance d #
# and with an efficiency epsilon each.                              #
#####################################################################

class FluxCalculator:

    def __init__(self, step, stepphi, a, b, d, nlayer, eff, I0):

        self.step = step
        self.stepphi = stepphi
        self.a = a
        self.b = b
        self.d = d
        self.nlayer = nlayer
        self.eff = eff
        self.I0 = I0
        
    # These are the upper limits of the flux integrals
    def thetaMax1(self, x, phi):

        return math.atan2( (self.a-x)/math.cos(phi), self.d)

    def thetaMax2(self, y, phi):

        return math.atan2((self.b-y)/math.sin(phi), self.d)

    def thetaMax3(self, x, phi):

        return math.atan2((-self.a-x)/math.cos(phi), self.d)

    def thetaMax4(self, y, phi):

        return math.atan2((-self.b-y)/math.sin(phi), self.d)

    # These are the critical points to integrate a rectangle
    def phi1(self, x, y):

        return math.atan2(self.b-y, self.a-x)

    def phi2(self, x, y):

        return math.atan2(self.b-y, -self.a-x)

    def phi3(self, x, y):

        return 2.0*math.pi + math.atan2(-self.b-y, -self.a-x)

    def phi4(self, x, y):

        return 2.0*math.pi + math.atan2(-self.b-y, self.a-x)


    def giveNMuons(self, t):

        Nx = int((2.0*self.a)/self.step)
        Ny = int((2.0*self.b)/self.step)
        
        #Integral accumulators
        I1 = 0.0
        I2 = 0.0
        I3 = 0.0
        I4 = 0.0
        for ix in range(1, Nx-1):
            for iy in range(1, Ny-1):
                x = -self.a + ix * self.step
                y = -self.b + iy * self.step
                #Estimate the critical points for x, y 
                phi1_ = self.phi1(x, y)
                phi2_ = self.phi2(x, y)
                phi3_ = self.phi3(x, y)
                phi4_ = self.phi4(x, y)
                
                #Estimate binning in phi
                Nphi1 = int((phi1_+(2.0*math.pi-phi4_))/self.stepphi)
                Nphi2 = int((phi2_ - phi1_)/self.stepphi)
                Nphi3 = int((phi3_ - phi2_)/self.stepphi)
                Nphi4 = int((phi4_ - phi3_)/self.stepphi)
                
                for iphi in range(0, Nphi1):
                    phia = phi4_ + iphi * self.stepphi
                    phib = phia + self.stepphi
                    I1a = (1.0 - math.pow(math.cos(self.thetaMax1(x, phia)), 3))
                    I1b = (1.0 - math.pow(math.cos(self.thetaMax1(x, phib)), 3))
                    I1 = I1 + (I1a+I1b)/2.0
                for iphi in range(0, Nphi2):
                    phia = phi1_ + iphi * self.stepphi
                    phib = phia + self.stepphi
                    I2a = (1.0 - math.pow(math.cos(self.thetaMax2(y, phia)), 3))
                    I2b = (1.0 - math.pow(math.cos(self.thetaMax2(y, phib)), 3))
                    I2 = I2 + (I2a+I2b)/2.0                                       
                for iphi in range(0, Nphi3):
                    phia = phi2_ + iphi * self.stepphi
                    phib = phia + self.stepphi
                    I3a = (1.0 - math.pow(math.cos(self.thetaMax3(x, phia)), 3))
                    I3b = (1.0 - math.pow(math.cos(self.thetaMax3(x, phib)), 3))
                    I3 = I3 + (I3a+I3b)/2.0   
                for iphi in range(0, Nphi4):
                    phia = phi3_ + iphi * self.stepphi
                    phib = phia + self.stepphi
                    I4a = (1.0 - math.pow(math.cos(self.thetaMax4(y, phia)), 3))
                    I4b = (1.0 - math.pow(math.cos(self.thetaMax4(y, phib)), 3))
                    I4 = I4 + (I4a+I4b)/2.0   
                 
        return self.step * self.step * self.stepphi * (self.I0/3.0) * math.pow(self.eff, self.nlayer) * t * (I1+I2+I3+I4)









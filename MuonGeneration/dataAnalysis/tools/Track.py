




class Track:

    def __init__(self):

        self.x0 = 0
        self.y0 = 0
        self.z0 = 0
        self.bx = 0
        self.by = 0
        self.bz = 0
        self.chi2 = 0
        self.hits = []

    def insertHit(self, x, y, z, t, energy):

        hit = [x, y, z, t, energy]
        self.hits.append(hit)

    def build(self):

        t = x = y = z = 0.
        t2 = x2 = y2 = z2 = 0.
        xt = yt = zt = 0.

        for h in self.hits:
            x = x + h[0]
            x2 = x2 + h[0]**2
            y = y + h[1]
            y2 = y2 + h[1]**2
            z = z + h[2]
            z2 = z2 + h[2]**2
            t = t + h[3]
            t2 = t2 + h[3]**2
            xt = xt + h[0]*h[3]
            yt = yt + h[1]*h[3]
            zt = zt + h[2]*h[3]
        x = x/len(self.hits)
        x2 = x2/len(self.hits)
        y = y/len(self.hits)
        y2 = y2/len(self.hits)
        z = z/len(self.hits)
        z2 = z2/len(self.hits)
        t = t/len(self.hits)
        t2 = t2/len(self.hits)
        xt = xt/len(self.hits)
        yt = yt/len(self.hits)
        zt = zt/len(self.hits)

        self.bx = (xt - x * t)/(t2-t*t)
        self.by = (yt - y * t)/(t2-t*t)
        self.bz = (zt - z * t)/(t2-t*t)
        self.x0 = x - self.bx*t
        self.y0 = y - self.by*t
        self.z0 = z - self.bz*t

        xplus = yplus = zplus = 0.0
        for h in self.hits:
            xplus = xplus + (h[0]-self.x0-self.bx*h[3])**2
            yplus = yplus + (h[1]-self.y0-self.by*h[3])**2
            zplus = zplus + (h[2]-self.z0-self.bz*h[3])**2
        self.chi2 = xplus+yplus+zplus    


    def print(self):

        print('Track parameters: (' + str(self.x0) + ', ', str(self.y0) + ', ' + str(self.z0) + ') + (' + str(self.bx) + ', ' + str(self.by) + ', ' + str(self.bz) + ') * t')
        print('Hits:')
        for h in self.hits:
            print('x:', h[0], 'y:', h[1], 'z:', h[2], 't:', h[3])



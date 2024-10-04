import ROOT as r
import math


class POCAEstimator:

    def __init__(self, tree, binInfo, threshold):

        self.tree = tree
        self.threshold = threshold
        
        self.thetax = r.TH1F('thetax', '', 100, -0.2, 0.2)
        self.thetax.SetStats(0)
        self.thetax.GetXaxis().SetTitle('#Delta#theta_{x}')
        self.thetay = r.TH1F('thetay', '', 100, -0.2, 0.2)
        self.thetay.SetStats(0)
        self.thetay.GetXaxis().SetTitle('#Delta#theta_{y}')
        self.hxyproj = r.TH2F('hxyproj', '', int(binInfo['hxy'][0]), binInfo['hxy'][1], binInfo['hxy'][2], int(binInfo['hxy'][3]), binInfo['hxy'][4], binInfo['hxy'][5])
        self.hxyproj.SetStats(0)
        self.hxyproj.GetXaxis().SetTitle('X [cm]')
        self.hxyproj.GetYaxis().SetTitle('Y [cm]')
        self.hxyproj.GetZaxis().SetTitle('N. Events')
        self.hxy = r.TH2F('hxy', '', int(binInfo['hxy'][0]), binInfo['hxy'][1], binInfo['hxy'][2], int(binInfo['hxy'][3]), binInfo['hxy'][4], binInfo['hxy'][5])
        self.hxy.SetStats(0)
        self.hxy.GetXaxis().SetTitle('X [cm]')
        self.hxy.GetYaxis().SetTitle('Y [cm]')
        self.hxy.GetZaxis().SetTitle('N. Events')
        self.hxz = r.TH2F('hxz', '', int(binInfo['hxz'][0]), binInfo['hxz'][1], binInfo['hxz'][2], int(binInfo['hxz'][3]), binInfo['hxz'][4], binInfo['hxz'][5])
        self.hxz.SetStats(0)
        self.hxz.GetXaxis().SetTitle('X [cm]')
        self.hxz.GetYaxis().SetTitle('Z [cm]')
        self.hxz.GetZaxis().SetTitle('N. Events')
        self.hyz = r.TH2F('hyz', '', int(binInfo['hyz'][0]), binInfo['hyz'][1], binInfo['hyz'][2], int(binInfo['hyz'][3]), binInfo['hyz'][4], binInfo['hyz'][5])
        self.hyz.SetStats(0)
        self.hyz.GetXaxis().SetTitle('Y [cm]')
        self.hyz.GetYaxis().SetTitle('Z [cm]')
        self.hyz.GetZaxis().SetTitle('N. Events')
        self.hxy_theta = r.TH2F('hxy_theta', '', int(binInfo['hxy'][0]), binInfo['hxy'][1], binInfo['hxy'][2], int(binInfo['hxy'][3]), binInfo['hxy'][4], binInfo['hxy'][5])
        self.hxz_theta = r.TH2F('hxz_theta', '', int(binInfo['hxz'][0]), binInfo['hxz'][1], binInfo['hxz'][2], int(binInfo['hxz'][3]), binInfo['hxz'][4], binInfo['hxz'][5])
        self.hyz_theta = r.TH2F('hyz_theta', '', int(binInfo['hyz'][0]), binInfo['hyz'][1], binInfo['hyz'][2], int(binInfo['hyz'][3]), binInfo['hyz'][4], binInfo['hyz'][5])
        self.hxy_theta2 = r.TH2F('hxy_theta2', '', int(binInfo['hxy'][0]), binInfo['hxy'][1], binInfo['hxy'][2], int(binInfo['hxy'][3]), binInfo['hxy'][4], binInfo['hxy'][5])
        self.hxz_theta2 = r.TH2F('hxz_theta2', '', int(binInfo['hxz'][0]), binInfo['hxz'][1], binInfo['hxz'][2], int(binInfo['hxz'][3]), binInfo['hxz'][4], binInfo['hxz'][5])
        self.hyz_theta2 = r.TH2F('hyz_theta2', '', int(binInfo['hyz'][0]), binInfo['hyz'][1], binInfo['hyz'][2], int(binInfo['hyz'][3]), binInfo['hyz'][4], binInfo['hyz'][5])
        self.var_xy = r.TH2F('var_xy', '', int(binInfo['hxy'][0]), binInfo['hxy'][1], binInfo['hxy'][2], int(binInfo['hxy'][3]), binInfo['hxy'][4], binInfo['hxy'][5])
        self.var_xy.SetStats(0)
        self.var_xy.GetXaxis().SetTitle('X [cm]')
        self.var_xy.GetYaxis().SetTitle('Y [cm]')
        self.var_xy.GetZaxis().SetTitle('#sigma(#theta)p#beta [rad MeV]')
        self.var_xz = r.TH2F('var_xz', '', int(binInfo['hxz'][0]), binInfo['hxz'][1], binInfo['hxz'][2], int(binInfo['hxz'][3]), binInfo['hxz'][4], binInfo['hxz'][5])
        self.var_xz.SetStats(0)
        self.var_xz.GetXaxis().SetTitle('X [cm]')
        self.var_xz.GetYaxis().SetTitle('Z [cm]')
        self.var_xz.GetZaxis().SetTitle('#sigma(#theta) [rad]')
        self.var_yz = r.TH2F('var_yz', '', int(binInfo['hyz'][0]), binInfo['hyz'][1], binInfo['hyz'][2], int(binInfo['hyz'][3]), binInfo['hyz'][4], binInfo['hyz'][5])
        self.var_yz.SetStats(0)
        self.var_yz.GetXaxis().SetTitle('Y [cm]')
        self.var_yz.GetYaxis().SetTitle('Z [cm]')
        self.var_yz.GetZaxis().SetTitle('#sigma(#theta)p#beta [rad MeV]')

    def loop(self):

        for ev in self.tree:
            x1 = [ev.x1, ev.y1, ev.z1]
            x2 = [ev.x2, ev.y2, ev.z2]
            vx1 = [ev.vx1, ev.vy1, ev.vz1]
            vx2 = [ev.vx2, ev.vy2, ev.vz2]
            if abs(ev.dthetax) > self.threshold or abs(ev.dthetay) > self.threshold:
                a = self.getPoint(x1, x2, vx1, vx2, ev.p1, ev.p2, ev.dthetax, ev.dthetay)


    def cross(self, v1, v2):

        a = [0.0, 0.0, 0.0]
        a[0] = v1[1] * v2[2] - v1[2] * v2[1]
        a[1] = v1[2] * v2[0] - v1[0] * v2[2]
        a[2] = v1[0] * v2[1] - v1[1] * v2[0]
        return a

    def dot(self, v1, v2):

        return v1[0]*v2[0] + v1[1]*v2[1] + v1[2] * v2[2]

    def norm(self, v):

        return math.sqrt(v[0]**2+v[1]**2+v[2]**2)

    def normalize(self, v):
        n = self.norm(v)
        a = v
        a[0] = v[0]/n
        a[1] = v[1]/n
        a[2] = v[2]/n
        return a

    def minus(self, v1, v2):

        return [v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]]
    
    def plus(self, v1, v2):

        return [v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2]]

    def divide(self, v1, k):

        return [v1[0]/k, v1[1]/k, v1[2]/k]
    
    def multiply(self, v1, k):

        return [v1[0]*k, v1[1]*k, v1[2]*k]

    def getPoint(self, x1, x2, p1, p2, k1, k2, thetax, thetay):


        self.thetax.Fill(thetax)
        self.thetay.Fill(thetay)
        b1 = k1 / math.sqrt(k1**2 + 105.66**2)
        b2 = k2 / math.sqrt(k2**2 + 105.66**2)
        momentum = (k1+k2)/2.0
        beta = (b1+b2)/2.0
        betap = momentum * beta

        cross_st = self.cross(p1, p2)
        cross_stnorm = self.norm(cross_st)
        vts = self.dot(p1, p2)
        if cross_stnorm < 1.0e-4 or vts < 1.0e-4:
            return False
        
        cross_sst = self.cross(p1, cross_st)
        DeltaR = self.minus(x1, x2)
        
        xproj = x2[0] + (20.0-x2[2])/p2[2] * p2[0]
        yproj = x2[1] + (20.0-x2[2])/p2[2] * p2[1]
        self.hxyproj.Fill(xproj, yproj)
        xpoca2 = self.minus(x2, self.multiply(p2, self.dot(DeltaR, cross_sst)/cross_stnorm**2))
        xpoca1 = self.plus(x1, self.multiply(p1, self.dot(self.minus(xpoca2, x1), p1)/vts))
        v = self.multiply(self.plus(xpoca1, xpoca2), 0.5)
        self.hxy.Fill(v[0], v[1])
        binxy = self.hxy.FindBin(v[0], v[1])
        valxy = self.hxy_theta.GetBinContent(binxy)
        #self.hxy_theta.SetBinContent(binxy, valxy + (thetax + thetay)*betap)
        print('Inserting:', valxy, thetax, thetay)
        self.hxy_theta.SetBinContent(binxy, valxy + (thetax + thetay))
        valxy2 = self.hxy_theta2.GetBinContent(binxy)
        #self.hxy_theta2.SetBinContent(binxy, valxy2 + (thetax*betap)**2 + (thetay*betap)**2)
        self.hxy_theta2.SetBinContent(binxy, valxy2 + (thetax)**2 + (thetay)**2)
        self.hxz.Fill(v[0], v[2])
        binxz = self.hxz.FindBin(v[0], v[2])
        valxz = self.hxz_theta.GetBinContent(binxz)
        self.hxz_theta.SetBinContent(binxz, valxz + thetax + thetay)
        valxz2 = self.hxz_theta2.GetBinContent(binxz)
        self.hxz_theta2.SetBinContent(binxz, valxz2 + thetax**2 + thetay**2)
        self.hyz.Fill(v[1], v[2])
        binyz = self.hyz.FindBin(v[1], v[2])
        valyz = self.hyz_theta.GetBinContent(binyz)
        self.hyz_theta.SetBinContent(binyz, valyz + thetax + thetay)
        valyz2 = self.hyz_theta2.GetBinContent(binyz)
        self.hyz_theta2.SetBinContent(binyz, valyz2 + thetax**2 + thetay**2)
        return True


    def makeVar(self):
       for ix in range(1, self.hxy.GetNbinsX()-1):
            for iy in range(1, self.hxy.GetNbinsY()-1):
                n = 2.0 * self.hxy.GetBinContent(ix, iy)
                theta = self.hxy_theta.GetBinContent(ix, iy)
                theta2 = self.hxy_theta2.GetBinContent(ix, iy)
                print('n:', n)
                var = math.sqrt(theta2/n - (theta/n)**2)
                if n > 4 and var < 0.05:
                    self.var_xy.SetBinContent(ix, iy, math.sqrt(theta2/n - (theta/n)**2))
       for ix in range(0, self.hxz.GetNbinsX()):
            for iy in range(0, self.hxz.GetNbinsY()):
                n = 2.0 * self.hxz.GetBinContent(ix, iy)
                theta = self.hxz_theta.GetBinContent(ix, iy)
                theta2 = self.hxz_theta2.GetBinContent(ix, iy)
                if n > 2:
                    self.var_xz.SetBinContent(ix, iy, math.sqrt(theta2/n - (theta/n)**2))

       for ix in range(0, self.hyz.GetNbinsX()):
            for iy in range(0, self.hyz.GetNbinsY()):
                n = 2.0 * self.hyz.GetBinContent(ix, iy)
                theta = self.hyz_theta.GetBinContent(ix, iy)
                theta2 = self.hyz_theta2.GetBinContent(ix, iy)
                if n > 2:
                    self.var_yz.SetBinContent(ix, iy, math.sqrt(theta2/n - (theta/n)**2))

 
    def MakePlot(self):
        
        self.makeVar()
        cthetax = r.TCanvas('cthetax', 'cthetax')
        self.thetax.Draw()
        cthetax.SaveAs('cthetax.png')
        cthetay = r.TCanvas('cthetay', 'cthetay')
        self.thetay.Draw()
        cthetay.SaveAs('cthetay.png')
        cxyproj = r.TCanvas('cxyproj', 'cxyproj')
        self.hxyproj.Draw('COLZ')
        cxyproj.SaveAs('hxyproj.png')
        cxy = r.TCanvas('cxy', 'cxy')
        self.hxy.Draw('COLZ')
        cxy.SaveAs('hxy.png')
        cxz = r.TCanvas('cxz', 'cxz')
        self.hxz.Draw('COLZ')
        cxz.SaveAs('hxz.png')
        cyz = r.TCanvas('cyz', 'cyz')
        self.hyz.Draw('COLZ')
        cyz.SaveAs('hyz.png')
        varcxy = r.TCanvas('varcxy', 'varcxy')
        varcxy.SetLogz(True)
        self.var_xy.Draw('COLZ')
        varcxy.SaveAs('varhxy.png')
        varcxz = r.TCanvas('varcxz', 'varcxz')
        self.var_xz.Draw('COLZ')
        varcxz.SaveAs('varhxz.png')
        varcyz = r.TCanvas('varcyz', 'varcyz')
        self.var_yz.Draw('COLZ')
        varcyz.SaveAs('varhyz.png')
 

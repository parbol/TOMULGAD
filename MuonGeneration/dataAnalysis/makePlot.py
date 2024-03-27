import ROOT as r
import math


hpt = r.TH1F("pt", "Muon momentum", 100, 0, 100)
han = r.TH1F("han", "Angle with respect to vertical", 100, 0, 3.1415)

for i in open('datos.txt').readlines():

    k = i.split()
    E = float(k[4])
    if E < 0.105:
        p = 0
    else:
        p = math.sqrt(E**2 - 0.105**2)
    vx = float(k[5])
    vy = float(k[6])
    vz = float(k[7])
    phi = math.acos(-vz/math.sqrt(vx**2+vy**2+vz**2))
    hpt.Fill(p)
    han.Fill(phi)


cpt = r.TCanvas('ptcan')
cpt.SetLogy(1)
hpt.GetXaxis().SetTitle('pt [GeV]')
hpt.SetStats(0)
hpt.Draw()
cpt.SaveAs("pt.png")
can = r.TCanvas('ancan')
han.GetXaxis().SetTitle('#theta [rad]')
han.SetStats(0)
han.Draw()
can.SaveAs("phi.png")





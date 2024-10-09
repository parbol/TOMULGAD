import json, sys, optparse
import ROOT as r
from array import array
from tools.Track import Track
from tools.TrackFinder import TrackFinder
import math


def correctAngle(vx, vz):
    
    if vx >= 0 and vz >= 0:
        return math.atan2(vx, vz)
    if vx < 0 and vz >= 0:
        return 2.0 * math.pi - math.atan2(-vx, vz)
    if vx < 0 and vz < 0:
        return math.atan2(-vx, -vz) + math.pi
    else:
        return math.pi - math.atan2(vx, -vz)


def correctNumberOfHits(ev):

    if ev.nhits < 8:
        return False
    seq = [0, 1, 2, 3, 4, 5, 6, 7]    
    counter = 0
    for i in range(0, ev.nhits):
        n = ev.det[i] * 4 + ev.layer[i]
        if n in seq:
            seq.remove(n)
            counter = counter + 1
    
    if counter == 8:
        return True
    return False



if __name__ == '__main__':

    parser = optparse.OptionParser(usage='usage: %prog [options] path', version='%prog 1.0')
    parser.add_option('-i', '--input', action='store', type='string', dest='inputFile', default='input.root', help='Input ROOT File')
    parser.add_option('-o', '--output', action='store', type='string', dest='outputFile', default='output.root', help='Output ROOT file')
    (opts, args) = parser.parse_args()
    
    c = 29.9792458 #cm/ns 
    mmu = 105.6583755 #MeV

    try:
        input = r.TFile(opts.inputFile)
    except:
        print('Cannot open input file')
        sys.exit()


    try:
        output = r.TFile(opts.outputFile, 'RECREATE')
    except:
        print('Cannot open output file')
        sys.exit()


    resp = r.TH1F('resp', '', 100, -10, 10)

    t = r.TTree('events', 'events') 
    nevent = array('i', [0]) 
    x1 = array('f', [0]) 
    y1 = array('f', [0]) 
    z1 = array('f', [0]) 
    vx1 = array('f', [0]) 
    vy1 = array('f', [0]) 
    vz1 = array('f', [0]) 
    p1 = array('f', [0]) 
    x2 = array('f', [0]) 
    y2 = array('f', [0]) 
    z2 = array('f', [0]) 
    vx2 = array('f', [0]) 
    vy2 = array('f', [0]) 
    vz2 = array('f', [0]) 
    p2 = array('f', [0]) 
    dthetax = array('f', [0]) 
    dthetay = array('f', [0]) 
 
    t.Branch('nevent', 'nevent', 'nevent/I') 
    t.Branch('x1', x1, 'x1/F') 
    t.Branch('y1', y1, 'y1/F') 
    t.Branch('z1', z1, 'z1/F') 
    t.Branch('vx1', vx1, 'vx1/F') 
    t.Branch('vy1', vy1, 'vy1/F') 
    t.Branch('vz1', vz1, 'vz1/F')
    t.Branch('p1', p1, 'p1/F')
    t.Branch('x2', x2, 'x2/F') 
    t.Branch('y2', y2, 'y2/F') 
    t.Branch('z2', z2, 'z2/F') 
    t.Branch('vx2', vx2, 'vx2/F') 
    t.Branch('vy2', vy2, 'vy2/F') 
    t.Branch('vz2', vz2, 'vz2/F')
    t.Branch('p2', p2, 'p2/F')
    t.Branch('dthetax', dthetax, 'dthetax/F')
    t.Branch('dthetay', dthetay, 'dthetay/F')

    for ev in input.events:

        if not correctNumberOfHits(ev):
            continue
        tf = TrackFinder(ev, 4)
        nevent[0] = ev.nevent
        x1[0] = tf.track1.x0
        y1[0] = tf.track1.y0
        z1[0] = tf.track1.z0
        vx1[0] = tf.track1.bx
        vy1[0] = tf.track1.by
        vz1[0] = tf.track1.bz
        v1 = math.sqrt(vx1[0]*vx1[0]+vy1[0]*vy1[0]+vz1[0]*vz1[0])
        beta1 = v1/c
        x2[0] = tf.track2.x0
        y2[0] = tf.track2.y0
        z2[0] = tf.track2.z0
        vx2[0] = tf.track2.bx
        vy2[0] = tf.track2.by
        vz2[0] = tf.track2.bz
        v2 = math.sqrt(vx2[0]*vx2[0]+vy2[0]*vy2[0]+vz2[0]*vz2[0])
        beta2 = v2/c
        #velocity estimation
        nux1 = vx1[0]/v1
        nuy1 = vy1[0]/v1
        nuz1 = vz1[0]/v1
        nux2 = vx2[0]/v2
        nuy2 = vy2[0]/v2
        nuz2 = vz2[0]/v2
        denominator = tf.track1.t2 + tf.track2.t2
        num1 = nux1 * tf.track1.xt + nuy1 * tf.track1.yt + nuz1 * tf.track1.zt
        num2 = nux2 * tf.track2.xt + nuy2 * tf.track2.yt + nuz2 * tf.track2.zt
        num3 = (nux1 * tf.track1.x0 + nuy1 * tf.track1.y0 + nuz1 * tf.track1.z0)* tf.track1.t
        num4 = (nux2 * tf.track2.x0 + nuy2 * tf.track2.y0 + nuz2 * tf.track2.z0)* tf.track2.t
        v = (num1 + num2 - num3 + num4)/denominator
        beta = v/c

        anglex1 = correctAngle(vx1[0], vz1[0])
        anglex2 = correctAngle(vx2[0], vz2[0])
        angley1 = correctAngle(vy1[0], vz1[0])
        angley2 = correctAngle(vy2[0], vz2[0])
        dthetax[0] = anglex1-anglex2
        dthetay[0] = angley1-angley2
        e11 = tf.track1.hits[0][4]
        e12 = tf.track1.hits[1][4]
        e13 = tf.track1.hits[2][4]
        e14 = tf.track1.hits[3][4]
        t.Fill()
        if e11 < mmu or e12 < mmu or e13 < mmu or e14 < mmu:
            continue
        p11 = math.sqrt(e11**2-mmu**2)
        p12 = math.sqrt(e12**2-mmu**2)
        p13 = math.sqrt(e13**2-mmu**2)
        p14 = math.sqrt(e14**2-mmu**2)
        if beta1 <= 1.0 and beta2 <= 1.0:
            gamma1 = 1.0/math.sqrt(1.0 - beta1**2)
            gamma2 = 1.0/math.sqrt(1.0 - beta2**2)
            p1[0] = mmu * beta1 * gamma1
            p2[0] = mmu * beta2 * gamma2

            resp.Fill(p1[0]-(p11+p12+p13+p14)/4.0)

        

    c = r.TCanvas('cresp', 'cresp')
    resp.GetXaxis().SetTitle('p_{reco}-p_{gen} [MeV]')
    resp.Draw()
    c.SaveAs('resp.png')

    output.Write()
    output.Close()
    input.Close()


 



 



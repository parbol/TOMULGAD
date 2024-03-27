import ROOT as r


class Event:

    def __init__(self, nEvent):
        self.nEvent = nEvent
        self.det = []
        self.layer = []
        self.lgad = []
        self.xpad = []
        self.ypad = []
    
    def add(self, det_, layer_, lgad_, xpad_, ypad_):
        self.det.append(det_)
        self.layer.append(layer_)
        self.lgad.append(lgad_)
        self.xpad.append(xpad_)
        self.ypad.append(ypad_)

    def nHits(self):

        return len(self.det)

if __name__=='__main__':

    f = r.TFile('output_0.5m_short.root')
    
    
    events = []
    event = -1
    counter = -1
    for ev in f.hits:
        if event != ev.eventNumber:
            newEvent = Event(ev.eventNumber)
            newEvent.add(ev.det, ev.layer, ev.lgad, ev.xpad, ev.ypad)
            events.append(newEvent)
            event = ev.eventNumber
            counter = counter + 1
        else:
            events[counter].add(ev.det, ev.layer, ev.lgad, ev.xpad, ev.ypad)
    f.Close()

    h = r.TH1F("h", "Number of hits", 10, 0, 10)

    for ev in events:
        h.Fill(ev.nHits())

    hcan = r.TCanvas('hcan')
    hcan.SetLogy(1)
    h.GetXaxis().SetTitle('N. hits')
    h.GetYaxis().SetRangeUser(0.1, 1e5)
    h.SetStats(0)
    h.Draw()
    hcan.SaveAs("Nhits_ShortPlus.png")



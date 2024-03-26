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

    f = r.TFile('output_0.5m.root')
    
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
          
    for ev in events:
        if ev.nHits() < 2:
            continue
        print(ev.nHits())

    f.Close()



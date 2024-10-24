import json, sys, optparse
import math
from matplotlib.collections import PatchCollection
from matplotlib.patches import Rectangle



#######################################################################
# This is a helper class to produce configuration files for the setup #
# Notice that here we are assuming that each detector/layer/sensor is #
# similar. If this is not the case you need to do some manual editing #
#######################################################################
if __name__=='__main__':

    parser = optparse.OptionParser(usage='usage: %prog [options] path', version='%prog 1.0')
    parser.add_option('-n', '--name', action='store', type='string', dest='name', default='', help='Name of output file')
    
    (opts, args) = parser.parse_args()
    if opts.name == '':
        print('Please specify a name for the output configuration file')
        sys.exit()
    
   
    #Sensor configuration
    nSensors = 144
    sensorSize = 2.2
    sensorZSize_ = 0.03
    centralCorridor = 0.1
    interpad = 0.01
    border = 0.005
    nPad = 16
    chargeThreshold = 20.0
    noise = 1.5
    lnSigma = 0.025
    tdcsigma = 0.01
    gain = 10.0
    nRow = int(math.sqrt(nSensors))
    L = (nRow*sensorSize+(nRow-2.0)*centralCorridor)

    #Efficiency estimation
    effSensor = nSensors * sensorSize**2 / L**2
    effPad = ((sensorSize - 2.0*border - (nPad-2)*interpad))**2 / sensorSize**2
    eff = effSensor * effPad
    print('L', L, 'Efficiency:', effSensor, effPad, eff)

    #Distance between the first layer of first detector and last of second
    Delta = 90.0
    #Distance between the last layer of first detector and first of second
    delta = 10

    #Detector Number
    nDetectors = 2
    detectorPositionX_ = 0
    detectorPositionY_ = 0
    detectorPositionZ_ = 1.0/4.0 * (Delta + delta)
    detectorXSize_ = L + 2
    detectorYSize_ = L + 2
   
    #Layer configuration
    nLayers = 4
    layerXPosition_ = 0
    layerYPosition_ = 0
    layerXSize_ = L + 1
    layerYSize_ = L + 1
    layerZSize_ = 1.2
    layerDistance = 1.0/6.0*(Delta-delta)
    detectorZSize_ = 3.0 * layerDistance + 4
    layer1Position = (detectorZSize_ - 4.0) / 2.0
    layer2Position = layer1Position - layerDistance
    layer3Position = layer1Position - 2.0 * layerDistance
    layer4Position = layer1Position - 3.0 * layerDistance

    globalFirstLayer = detectorPositionZ_ + layer1Position
    effectiveLength = globalFirstLayer
    
    #The world
    zSizeWorld = 250
    SkySize = detectorXSize_ + 2 
    xSizeWorld = 100
    ySizeWorld = 100
    zSizeWorld = 250
    zCeiling = 125
    thetaMin = 0.0
    thetaMax = math.atan2(math.sqrt(2)*SkySize/2.0, effectiveLength) 
    SkyX = 0
    SkyY = 0
    SkyZ = detectorPositionZ_ + layer1Position + layerZSize_ /2.0 + 0.1
    
    

    #We take the structure from this basic json file and adapt the dictionary
    with open('../data/confExample.json', 'r') as f:
        data_ = json.load(f)
    
    theWorld = data_['theWorld']
    detector = data_['Detectors'][0]
    layer = detector['Layers'][0]
    sensor = layer['Sensors'][0]
    phantom = data_['Phantoms'][0]

    #The world configuration
    theWorld["zSizeWorld"] = zSizeWorld
    theWorld["zCeiling"] = zCeiling
    theWorld["SkySize"] = SkySize
    theWorld["xSizeWorld"] = xSizeWorld
    theWorld["ySizeWorld"] = ySizeWorld
    theWorld["zSizeWorld"] = zSizeWorld
    theWorld["zCeiling"] = zCeiling
    theWorld["thetaMin"] = thetaMin
    theWorld["thetaMax"] = thetaMax
    theWorld["SkyX"] = SkyX
    theWorld["SkyY"] = SkyY
    theWorld["SkyZ"] = SkyZ

    sensors = []
    for isensor in range(0, nSensors):
        copysens = sensor.copy()
        sensors.append(copysens)
    layer['Sensors'] = sensors

    layers = []
    for ilayer in range(0, nLayers):
        copylayer = layer.copy()
        layers.append(copylayer)
    detector['Layers'] = layers

    detectors = []
    for idetector in range(0, nDetectors):
        copydetector = detector.copy()
        detectors.append(copydetector)

    scenario = 0
  
    if scenario == 1:
        #Phantom Number
        nPhantoms = 1
        phantoms = []
        for iphantom in range(0, nPhantoms):
            copyphantom = phantom.copy()
            phantoms.append(copyphantom)
        phantoms[0]['name'] = 'iron1'
        phantoms[0]['material'] = 'iron'
        phantoms[0]['xPos'] = 0.0
        phantoms[0]['yPos'] = 0.0
        phantoms[0]['zPos'] = 0.0
        phantoms[0]['type'] = 1
        phantoms[0]['radius'] = 0.0
        phantoms[0]['zsize'] = 0.0
        phantoms[0]['sideX'] = 5.0
        phantoms[0]['sideY'] = 5.0
        phantoms[0]['sideZ'] = 3.0
        #phantoms[2]['name'] = 'iron3'
        #phantoms[2]['material'] = 'aluminium'
        #phantoms[2]['xPos'] = -2.0
        #phantoms[2]['yPos'] = 2.0
        #phantoms[2]['zPos'] = 0.0
        #phantoms[2]['type'] = 0
        #phantoms[2]['radius'] = 1.0
        #phantoms[2]['zsize'] = 3.0
        #phantoms[2]['sideX'] = 0
        #phantoms[2]['sideY'] = 0
        #phantoms[2]['sideZ'] = 0

        #phantoms[3]['name'] = 'iron4'
        #phantoms[3]['material'] = 'lead'
        #phantoms[3]['xPos'] = -2.0
        #phantoms[3]['yPos'] = -2.0
        #phantoms[3]['zPos'] = 0.0
        #phantoms[3]['type'] = 0
        #phantoms[3]['radius'] = 1.0
        #phantoms[3]['zsize'] = 3.0
        #phantoms[3]['sideX'] = 0
        #phantoms[3]['sideY'] = 0
        #phantoms[3]['sideZ'] = 0
    elif scenario == 2:
        #Phantom Number
        nPhantoms = 14
        phantoms = []
        offset = -2.5
        scale = 0.8
        name = []
        Lx = []
        Ly = []
        LW = []
        LH = []
        name.append('L1')
        Lx.append((-7 * scale + offset)+2.0*scale/2.0)
        Ly.append(-4 * scale+7.0*scale/2.0)
        LW.append(2.0 * scale)
        LH.append(7.0 * scale)
        name.append('L2')
        Lx.append(-5 * scale + offset+2.0*scale/2.0)
        Ly.append(-4 * scale+2.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(2 * scale)
        name.append('G1')
        Lx.append(-2 * scale + offset+2.0*scale/2.0)
        Ly.append(-4 * scale + 7.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(7 * scale)
        name.append('G2')
        Lx.append(0 * scale + offset + 2.0*scale/2.0)
        Ly.append(1 * scale + 2.0 * scale/2.0)
        LW.append(2 * scale)
        LH.append(2 * scale)
        name.append('G3')
        Lx.append(0 * scale + offset + 2.0*scale/2.0)
        Ly.append(-4 * scale + 2.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(2 * scale)
        name.append('G4')
        Lx.append(1 * scale + offset + 1.0*scale/2.0)
        Ly.append(-2 * scale + 1.0*scale/2.0)
        LW.append(1 * scale)
        LH.append(1 * scale)
        name.append('A1')
        Lx.append(3 * scale + offset + 2.0*scale/2.0)
        Ly.append(-4 * scale + 7.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(7 * scale)
        name.append('A2')
        Lx.append(6 * scale + offset+2.0*scale/2.0)
        Ly.append(-4 * scale + 7.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(7 * scale)
        name.append('A3')
        Lx.append(5 * scale + offset +1.0*scale/2.0)
        Ly.append(2 * scale + 1.0*scale/2.0)
        LW.append(1 * scale)
        LH.append(1 * scale)
        name.append('A4')
        Lx.append(5 * scale + offset + 1.0*scale/2.0)
        Ly.append(-1 * scale + 1.0*scale/2.0)
        LW.append(1 * scale)
        LH.append(1 * scale)
        name.append('D1')
        Lx.append(9 * scale + offset + 2.0*scale/2.0)
        Ly.append(-4 * scale + 7.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(7 * scale)
        name.append('D2')
        Lx.append(12 * scale + offset + 2.0*scale/2.0)
        Ly.append(-2.0 * scale + 3.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(3 * scale)
        name.append('D3')
        Lx.append(11 * scale + offset + 2.0*scale/2.0)
        Ly.append(-4.0 * scale + 2.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(2 * scale)
        name.append('D4')
        Lx.append(11 * scale + offset + 2.0*scale/2.0)
        Ly.append(1.0 * scale + 2.0*scale/2.0)
        LW.append(2 * scale)
        LH.append(2 * scale)

        for iphantom in range(0, nPhantoms):
            copyphantom = phantom.copy()
            phantoms.append(copyphantom)
            phantoms[iphantom]['name'] = name[iphantom]
            phantoms[iphantom]['material'] = 'iron'
            phantoms[iphantom]['xPos'] = Lx[iphantom]
            phantoms[iphantom]['yPos'] = Ly[iphantom]
            phantoms[iphantom]['zPos'] = 0.0
            phantoms[iphantom]['type'] = 1
            phantoms[iphantom]['radius'] = 0.0
            phantoms[iphantom]['zsize'] = 0.0
            phantoms[iphantom]['sideX'] = LW[iphantom]
            phantoms[iphantom]['sideY'] = LH[iphantom]
            phantoms[iphantom]['sideZ'] = 3.0
            
 
    else:
        phantoms = []


    data = {} 
    data['theWorld'] = theWorld
    data['Phantoms'] = phantoms
    data['Detectors'] = detectors
    

    #DetectorInfo 
    detectorXPosition = [detectorPositionX_, detectorPositionX_]
    detectorYPosition = [detectorPositionY_, detectorPositionY_]
    detectorZPosition = [detectorPositionZ_, -detectorPositionZ_]
    detectorXSize = [detectorXSize_, detectorXSize_]
    detectorYSize = [detectorYSize_, detectorYSize_]
    detectorZSize = [detectorZSize_, detectorZSize_]
    layerXPosition = [layerXPosition_, layerXPosition_, layerXPosition_, layerXPosition_]
    layerYPosition = [layerYPosition_, layerYPosition_, layerYPosition_, layerYPosition_]
    layerZPosition = [layer1Position, layer2Position, layer3Position, layer4Position]
    layerXSize = [layerXSize_, layerXSize_, layerXSize_, layerXSize_]
    layerYSize = [layerYSize_, layerYSize_, layerYSize_, layerYSize_]
    layerZSize = [layerZSize_, layerZSize_, layerZSize_, layerZSize_]
   
    posX = -L/2.0 + sensorSize / 2.0
    posY = -L/2.0 + sensorSize / 2.0
    sensorXPosition = []
    sensorYPosition = []
    sensorZPosition = []
    sensorXSize = []
    sensorYSize = []
    sensorZSize = []
    for ix in range(0, int(nRow/2)):
        for iy in range(0, int(nRow/2)):
            Xc = centralCorridor/2.0 + sensorSize/2.0 + ix * (sensorSize + centralCorridor)
            Yc = centralCorridor/2.0 + sensorSize/2.0 + iy * (sensorSize + centralCorridor)
            sensorXPosition.append(Xc)
            sensorYPosition.append(Yc)
            sensorZPosition.append(0.1 + sensorZSize_/2.0)
            sensorXSize.append(sensorSize)
            sensorYSize.append(sensorSize)
            sensorZSize.append(sensorZSize_)
            sensorXPosition.append(-Xc)
            sensorYPosition.append(Yc)
            sensorZPosition.append(0.1 + sensorZSize_/2.0)
            sensorXSize.append(sensorSize)
            sensorYSize.append(sensorSize)
            sensorZSize.append(sensorZSize_)
            sensorXPosition.append(Xc)
            sensorYPosition.append(-Yc)
            sensorZPosition.append(0.1 + sensorZSize_/2.0)
            sensorXSize.append(sensorSize)
            sensorYSize.append(sensorSize)
            sensorZSize.append(sensorZSize_)
            sensorXPosition.append(-Xc)
            sensorYPosition.append(-Yc)
            sensorZPosition.append(0.1 + sensorZSize_/2.0)
            sensorXSize.append(sensorSize)
            sensorYSize.append(sensorSize)
            sensorZSize.append(sensorZSize_)

    for i, det_ in enumerate(data['Detectors']):
        det_['xPosDetector'] = detectorXPosition[i]  
        det_['yPosDetector'] = detectorYPosition[i]        
        det_['zPosDetector'] = detectorZPosition[i]
        det_['xSizeDetector'] = detectorXSize[i]
        det_['ySizeDetector'] = detectorYSize[i]
        det_['zSizeDetector'] = detectorZSize[i]
        for j, layer_ in enumerate(det_['Layers']):
            layer_['xPosLayer'] = layerXPosition[j]
            layer_['yPosLayer'] = layerYPosition[j]
            layer_['zPosLayer'] = layerZPosition[j]
            layer_['xSizeLayer'] = layerXSize[j]
            layer_['ySizeLayer'] = layerYSize[j]
            layer_['zSizeLayer'] = layerZSize[j]
            for k, sensor_ in enumerate(layer_['Sensors']):
                sensor_['xPosSensor'] = sensorXPosition[k]
                sensor_['yPosSensor'] = sensorYPosition[k]
                sensor_['zPosSensor'] = sensorZPosition[k]
                sensor_['xSizeSensor'] = sensorXSize[k]
                sensor_['ySizeSensor'] = sensorYSize[k]
                sensor_['zSizeSensor'] = sensorZSize[k]
                sensor_['interPadx'] = interpad
                sensor_['interPady'] = interpad
                sensor_['xborder'] = border
                sensor_['yborder'] = border
                sensor_['nPadx'] = nPad
                sensor_['nPady'] = nPad
                sensor_['noise'] = noise
                sensor_['tdcSigma'] = tdcsigma
                sensor_['lnSigma'] = lnSigma
                sensor_['gain'] = gain

    # Serializing json
    json_object = json.dumps(data, indent=4)
    # Writing to sample.json
    with open(opts.name, "w") as outfile:
        outfile.write(json_object)



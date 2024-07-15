import json, sys, optparse
import math

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
    
    nDetectors = 2
    nLayers = 4
    nSensors = 1521
 
    #We take the structure from this basic json file and adapt the dictionary
    with open('../data/confExample.json', 'r') as f:
        data_ = json.load(f)
    
    theWorld = data_['theWorld']
    detector = data_['Detectors'][0]
    layer = detector['Layers'][0]
    sensor = layer['Sensors'][0]

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

    theWorld['xSizeWorld'] = 30000
    theWorld['ySizeWorld'] = 30000
    theWorld['zSizeWorld'] = 30000
    theWorld['zOffsetCRY'] = 15000
    theWorld['sizeBoxCRY'] = 15000

    data = {} 
    data['theWorld'] = theWorld
    data['Detectors'] = detectors

    
    #This must be configured for every setup 
    angle = 30.0
    anglerad= angle * math.pi / 180.0
    detectorXPosition = [-12000 + 10.0 * math.cos(anglerad), -12000 - 10 * math.cos(anglerad)]
    detectorYPosition = [0, 0]
    detectorZPosition = [150 + 10.0 * math.sin(anglerad), 150 - 10.0 * math.sin(anglerad)]
    detectorXSize = [110, 110]
    detectorYSize = [110, 110]
    detectorZSize = [16, 16]
    yDirDetector = [-(90.0-angle), -(90.0-angle)]
    layerXPosition = [0, 0, 0, 0]
    layerYPosition = [0, 0, 0, 0]
    layerZPosition = [7.5, 2.5, -2.5, -7.5]
    layerXSize = [110, 110, 110, 110]
    layerYSize = [110, 110, 110, 110]
    layerZSize = [1, 1, 1, 1]
    sensorSize = 2.2
    centralCorridor = 0.05
    interpad = 0.05
    L = (39*sensorSize+37*centralCorridor)
    posX = -L/2.0 + sensorSize / 2.0
    posY = -L/2.0 + sensorSize / 2.0
    sensorXPosition = []
    sensorYPosition = []
    sensorZPosition = []
    sensorXSize = []
    sensorYSize = []
    sensorZSize = []
    for ix in range(0, 39):
        for iy in range(0, 39):
            Xc = posX + ix * (sensorSize + centralCorridor)
            Yc = posY + iy * (sensorSize + centralCorridor)
            sensorXPosition.append(Xc)
            sensorYPosition.append(Yc)
            sensorZPosition.append(0)
            sensorXSize.append(sensorSize)
            sensorYSize.append(sensorSize)
            sensorZSize.append(0.05)

    for i, det_ in enumerate(data['Detectors']):
        det_['xPosDetector'] = detectorXPosition[i]  
        det_['yPosDetector'] = detectorYPosition[i]        
        det_['zPosDetector'] = detectorZPosition[i]
        det_['xSizeDetector'] = detectorXSize[i]
        det_['ySizeDetector'] = detectorYSize[i]
        det_['zSizeDetector'] = detectorZSize[i]
        det_['yDirDetector'] = yDirDetector[i]
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
                sensor_['gain'] = 10


    # Serializing json
    json_object = json.dumps(data, indent=4)
    # Writing to sample.json
    with open(opts.name, "w") as outfile:
        outfile.write(json_object)



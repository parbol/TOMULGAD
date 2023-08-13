import json, sys, optparse

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
    nLayers = 3
    nSensors = 4
 
    #We take the structure from this basic json file and adapt the dictionary
    with open('../data/confExample.json', 'r') as f:
        data = json.load(f)
    
    theWorld = data['theWorld']
    detector = data['Detectors'][0]
    layer = detector['Layers'][0]
    sensor = layer['Sensors'][0]
 
    sensors = []
    for isensor in range(0, nSensors):
        sensors.append(sensor)
    layer['Sensors'] = sensors

    layers = []
    for ilayer in range(0, nLayers):
        layers.append(layer)
    detector['Layers'] = layers

    detectors = []
    for idetector in range(0, nDetectors):
        detectors.append(detector)
    data['Detectors'] = detectors

    #This must be configured for every setup 
    detectorXPosition = [0, 0]
    detectorYPosition = [0, 0]
    detectorZPosition = [25, -25]
    detectorXSize = [7, 7]
    detectorYSize = [7, 7]
    detectorZSize = [30, 30]
    layerXPosition = [0, 0, 0]
    layerYPosition = [0, 0, 0]
    layerZPosition = [10, 0, -10]
    layerXSize = [6, 6, 6]
    layerYSize = [6, 6, 6]
    layerZSize = [1, 1, 1]
    sensorSize = 2.2
    centralCorridor = 0.4
    posS = (sensorSize+centralCorridor)/2.0
    sensorXPosition = [posS, -posS, posS, -posS]
    sensorYPosition = [posS, -posS, posS, -posS]
    sensorZPosition = [0, 0, 0, 0]
    sensorXSize = [sensorSize, sensorSize, sensorSize, sensorSize]
    sensorYSize = [sensorSize, sensorSize, sensorSize, sensorSize]
    sensorSizeZ = 0.05
    sensorZSize = [sensorSizeZ, sensorSizeZ, sensorSizeZ, sensorSizeZ]

    for i, det in enumerate(data['Detectors']):
        det['xPosDetector'] = detectorXPosition[i]  
        det['yPosDetector'] = detectorYPosition[i]        
        det['zPosDetector'] = detectorZPosition[i]
        det['xSizeDetector'] = detectorXSize[i]
        det['ySizeDetector'] = detectorYSize[i]
        det['zSizeDetector'] = detectorZSize[i]
        for j, layer in enumerate(det['Layers']):
            layer['xPosLayer'] = layerXPosition[j]
            layer['yPosLayer'] = layerYPosition[j]
            layer['zPosLayer'] = layerZPosition[j]
            layer['xSizeLayer'] = layerXSize[j]
            layer['ySizeLayer'] = layerYSize[j]
            layer['zSizeLayer'] = layerZSize[j]
            for k, sensor in enumerate(layer['Sensors']):
                sensor['xPosSensor'] = sensorXPosition[k]
                sensor['yPosSensor'] = sensorYPosition[k]
                sensor['zPosSensor'] = sensorZPosition[k]
                sensor['xSizeSensor'] = sensorXSize[k]
                sensor['ySizeSensor'] = sensorYSize[k]
                sensor['zSizeSensor'] = sensorZSize[k]


    # Serializing json
    json_object = json.dumps(data, indent=4)
    # Writing to sample.json
    with open(opts.name, "w") as outfile:
        outfile.write(json_object)



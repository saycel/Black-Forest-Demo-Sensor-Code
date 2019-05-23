#!/usr/bin/python3

import urllib.request
import time
import argparse
import random

class ConfigurationInfo:
    loopcount = 0
    appkey = ""
    sleeptime = 0
    networkid = ""
    deviceid = ""
    startvalue = 0
    valuerange = 0
    verbose = False

def log(message):
    if(config.verbose == True):
        print(message)

def handle_commandline():
    parser = argparse.ArgumentParser(description = "Emulates MFS calls to ESN service")
    parser.add_argument('--loopcount', default=10, help='How many calls to make. 0 for loop forever')
    parser.add_argument('--appkey', required = True, help = 'ESN application key')
    parser.add_argument('--sleeptime', default='60', help='Seconds between calls')
    parser.add_argument('--networkid', required=True, help='Network ID')
    parser.add_argument('--deviceid', required=True, help='Device ID')
    parser.add_argument('--startvalue', default='1000', help='Starting value of the sensor reading')
    parser.add_argument('--valuerange', default = 10, help='Range of changing value (-n to n)')
    parser.add_argument('--verbose', action='store_true', help='Verbose Output')

    args = parser.parse_args()

    config.loopcount = int(args.loopcount)
    config.appkey = args.appkey
    config.sleeptime= args.sleeptime
    config.networkid = args.networkid
    config.deviceid = args.deviceid
    config.startvalue = args.startvalue
    config.valuerange = args.valuerange
    config.verbose = args.verbose
    log("Done reading command line arguments")
    log("\tappkey: " + config.appkey)
    log("\tloopcount: " + str(config.loopcount))
    log("\tsleeptime: " + config.sleeptime)
    log("\tnetworkid: " + config.networkid)
    log("\tdeviecid: " + config.deviceid)
    log("\tstartvalue: " + config.startvalue)
    log("\tvaluerange: " + str(config.valuerange))


def setup():
    log("Setup")
    current_channel_1_value = config.startvalue

config = ConfigurationInfo()
handle_commandline()
running = True
current_it = 1
current_channel_1_value = int(config.startvalue)

setup()

while running:
    if(int(config.loopcount) != 0):
        log("\n"+str(config.loopcount - current_it + 1)+": Generating value for channel 1")
    else:
        log("\nGenerating value for channel 1")

    change_in_value_1 = random.randrange(config.valuerange/2*-1, config.valuerange/2)
    current_channel_1_value += change_in_value_1
    #web_call = "http://192.168.88.229:3000/test?value="+config.appkey+":"+config.networkid+":"+config.deviceid+":"+str(current_channel_1_value)
    #Format of the API is http://{ip}/sensor/collector/{app_key}/{net_key}/{device_id}/?ch{number}={value}
    #For reference, as of the current build of the API, it returns a JSON object with a key "message" having the value "success" when the call was successful.
    web_call = "http://192.168.100.240:2323/sensor/collector/"+config.appkey+"/"+config.networkid+"/"+config.deviceid+"/?ch1="+str(current_channel_1_value)
    log("\tCalling web service with:")
    log("\t["+web_call+"]")
    try:
        contents = urllib.request.urlopen(web_call).read()
        log("\tReceived: "+str(contents))    
    except:
        print("Unable to open url")    

    log("\tSleeping")
    time.sleep(float(config.sleeptime))

    if(int(config.loopcount) != 0):
        if(current_it >= int(config.loopcount)):
            running = False
        else:
            current_it = current_it + 1

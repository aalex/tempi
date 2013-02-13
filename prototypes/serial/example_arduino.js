var tempi = require("tempi");
tempi.setLogLevel("INFO");

tempi.createGraph("g");

tempi.createNode("g", "base.serial.device", "arduino");
tempi.createNode("g", "base.flow.print", "print");

tempi.waitUntilAllNodesAreInitiated("g");

tempi.connect("g", "arduino", "fudi", "print", "0");

tempi.setNodeAttributeValue("g", "arduino", "device", "/dev/ttyACM0");
tempi.setNodeAttributeValue("g", "print", "prefix", "From Arduino: ");



setInterval((function() {}), 5000);

setTimeout((function() {
    tempi.messageInlet("g", "arduino", "fudi", "led0", 1023);
    //tempi.messageInlet("g", "arduino", "fudi", "r");
    tempi.messageInlet("g", "arduino", "fudi", "interval", 500);
    tempi.messageInlet("g", "arduino", "fudi", "start");
}), 2000);

setTimeout((function() {
    tempi.messageInlet("g", "arduino", "fudi", "stop");
}), 12000);

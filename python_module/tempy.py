import tempi
import time

_wrapper = tempi.Wrapper()

def mess(*args):
    ret = tempi.Message()
    for arg in args:
        if type(arg) == int:
            ret.appendInt(arg)
        if type(arg) == str:
            ret.appendString(arg)
        if type(arg) == float:
            ret.appendFloat(arg)
        if type(arg) == bool:
            ret.appendBoolean(arg)
        else:
            print("Unsupported atom type: %s" % (type(arg)))
    return ret

class Graph(object):
    def __init__(self, name="graph0"):
        self._graph_name = name
        _wrapper.createGraph(self._graph_name)

    def node(self, typeName, name):
        _wrapper.createNode(self._graph_name, typeName, name)

    def connect(self, fromNode, fromOutlet, toNode, toOutlet):
        _wrapper.connect(self._graph_name, fromNode, fromOutlet, toNode, toOutlet)

    def getWrapper(self):
        return _wrapper
    
    def runUntilControlC(self):
        try:
            while True:
                time.sleep(0.1)
        except KeyboardInterrupt, e:
            pass
    
    def attr(self, node, attribute, value):
        _wrapper.setNodeAttributeValue(self._graph_name, node, attribute, value)

        


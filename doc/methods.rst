Tempi/Entity Methods
====================

Usage
-----
Here is how we could use them in Nodejs in a synchronous way::

  var inputs = tempi.callNodeMethod("graph0", "midiinput0", "listInputs");
  for (var i = 0; i < inputs.length; i++)
  {
      console.log(inputs[i]);
  }

Here is how we could use them in Nodejs in an asynchronous way::

  tempi.callNodeMethod("graph0", "midiinput0", "listInputs", [], function (result) {
      for (var i = 0; i < result.length; i++)
      {
          console.log(inputs[i]);
      }
  });

For now, we prefer the first - synchronous - way.

Design
------
Methods require a signal for when they are called.

Each node can define its own methods. They own a map of NodeMethod instances. They must create a child class of EntityMethod to be registered. Its doCall virtual method must then prepare a return message and modify whatever needs to be done with the node.


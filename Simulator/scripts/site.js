$(document).ready(function () {
    ko.applyBindings(model);
});

var model = {
}
model.switches = ko.observableArray([
    createSwitch("1",       1,  1, 0, "up",     "Rig", [0]),
    createSwitch("2",       2,  2, 0, "up",     "Rig", [1]),
    createSwitch("3",       3,  3, 0, "up",     "Rig", [2]),
    createSwitch("4",       4,  4, 0, "up",     "Rig", [3]),
    createSwitch("5",       5,  5, 0, "up",     "Rig", [4]),
    createSwitch("I",       6,  1, 0, "right",  "Stomp", [5,6,7,8]),
    createSwitch("II",      7,  2, 0, "right",  "Stomp", [9,10,11,12]),
    createSwitch("III",     8,  3, 0, "right",  "Stomp", [13,14,15,16]),
    createSwitch("IIII",    9,  4, 0, "right",  "Stomp", [17,18,19,20]),
    createSwitch("Tap",     10, 1, 0, "up",     "Tap", [21]),
    createSwitch("Tuner",   11, 1, 0, "up",     "Tuner", [22,23,24]),
    createSwitch("Looper",  12, 1, 0, "up",     "Looper", [25]),
    createSwitch("Up",      13, 1, 0, "up",     "Up", []),
    createSwitch("Down",    14, 2, 0, "up",     "Down", []),
/*
        { title: "1",       switchId: 1,    lid: 1, state: 0, titlePos: "up",       tid: "Rig",     leds: ko.computed(getLeds, model), ledIds: [0]},
        { title: "2",       switchId: 2,    lid: 2, state: 0, titlePos: "up",       tid: "Rig",     leds: ko.computed(getLeds, model), ledIds: [1]},
        { title: "3",       switchId: 3,    lid: 3, state: 0, titlePos: "up",       tid: "Rig",     leds: ko.computed(getLeds, model), ledIds: [2]},
        { title: "4",       switchId: 4,    lid: 4, state: 0, titlePos: "up",       tid: "Rig",     leds: ko.computed(getLeds, model), ledIds: [3]},
        { title: "5",       switchId: 5,    lid: 5, state: 0, titlePos: "up",       tid: "Rig",     leds: ko.computed(getLeds, model), ledIds: [4]},
        { title: "I",       switchId: 6,    lid: 1, state: 0, titlePos: "right",    tid: "Stomp",   leds: ko.computed(getLeds, model), ledIds: [5,6,7,8]},
        { title: "II",      switchId: 7,    lid: 2, state: 0, titlePos: "right",    tid: "Stomp",   leds: ko.computed(getLeds, model), ledIds: [9,10,11,12]},
        { title: "III",     switchId: 8,    lid: 3, state: 0, titlePos: "right",    tid: "Stomp",   leds: ko.computed(getLeds, model), ledIds: [13,14,15,16]},
        { title: "IIII",    switchId: 9,    lid: 4, state: 0, titlePos: "right",    tid: "Stomp",   leds: ko.computed(getLeds, model), ledIds: [17,18,19,20]},
        { title: "Tap",     switchId: 10,   lid: 1, state: 0, titlePos: "right",    tid: "Tap",     leds: ko.computed(getLeds, model), ledIds: [21]},
        { title: "Tuner",   switchId: 11,   lid: 1, state: 0, titlePos: "up",       tid: "Tuner",   leds: ko.computed(getLeds, model), ledIds: [22,23,24]},
        { title: "Looper",  switchId: 12,   lid: 1, state: 0, titlePos: "up",       tid: "Looper",  leds: ko.computed(getLeds, model), ledIds: [25]},
        { title: "Up",      switchId: 13,   lid: 1, state: 0, titlePos: "up",       tid: "Up",      },
        { title: "Down",    switchId: 14,   lid: 2, state: 0, titlePos: "up",       tid: "Down",    }, */
]);

function createSwitch(title, switchId, lid, state, titlePos, tid, ledIds) {
    var item = {
        title: title,
        switchId: switchId,
        lid: lid,
        state: state,
        titlePos: titlePos,
        tid: tid,
        ledIds: ledIds,
        updateLed: ko.observable(0)
    };
    item.ledColor = function (id) {
        return ko.pureComputed(function () {
            item.updateLed();
            if (item.ledIds && (item.ledIds[id] || item.ledIds[id] === 0) && item.ledData)
            {
                var led = item.ledData[item.ledIds[id]];
                if (item.ledData)
                    return "rgb("+led.r+", "+led.g+", "+led.b+")";
            }
            return "rgb(127, 127, 127)";
        });
    };
    return item;
}

KemperArduino = {
    onSetLeds: null,
    onDebug: null,
    onDisplay: null,
    socket: null,
    initialize: function () {
        var self = this;
        this.socket = io('http://localhost');
        this.socket.on('connect', function () {
        });
        this.socket.on('setLeds', function (data) {
            ko.utils.arrayForEach(model.switches(), function (sw) {
                sw.ledData = data;
                sw.updateLed(1-sw.updateLed());
            });

        });
        this.socket.on('debug', function (data) {
            debug(data.text)
        });
        this.socket.on('display', function (data) {
            if (self.onDisplay)
                self.onDisplay(data.data);
        });
    },
    switchDown: function (idx) {
        this.socket.emit('switchDown', { idx: idx });
    },
    switchUp: function (idx) {
        this.socket.emit("switchUp", { idx: idx });
    },
};

setTimeout(function () {
    KemperArduino.switchDown(2);
    setTimeout(function () {
        KemperArduino.switchUp(2);
    }, 300);
}, 1000);

KemperArduino.initialize();

function debug(str) {
}
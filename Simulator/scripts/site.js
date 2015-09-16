var ctx;
var canvas;

function is_touch_device() {
    return (('ontouchstart' in window)
         || (navigator.MaxTouchPoints > 0)
         || (navigator.msMaxTouchPoints > 0));
}

var isTouch = is_touch_device();

$(document).ready(function () {
    ko.applyBindings(model);
    canvas = document.getElementById("DisplayCanvas");
    ctx = canvas.getContext("2d");
    $("body").addClass("Touch");
});

var model = {
    switchStates: [],
    curExpPedal: ko.observable(0),
    curExpPedalValue: ko.observable(0),
    onSwitchDown: function (sw) {
        KemperArduino.switchDown(sw.switchId);
    },
    onExpClick: function (x, e) {
        var expId = $(e.target).attr("data-exp-id");
        if (x.curExpPedal() != expId)
            x.curExpPedal(expId);
        else
            x.curExpPedal(0);
    }, 
    onSwitchUp: function (sw) {
        KemperArduino.switchUp(sw.switchId);
    },
    onKeyDown: function (obj, e) {
        var keyCode = window.event ? e.keyCode : e.which;
        var idx = keyCode - 49;
        if (idx>=0 && idx<8) {
            KemperArduino.switchDown(101 + idx);
            KemperArduino.switchUp(101 + idx);
        }
        var sw = obj.getKeySwitch(keyCode);
        if (sw >= 0 && !obj.switchStates[sw]) {
            obj.switchStates[sw] = true;
            KemperArduino.switchDown(sw + 1);
        }
        sw = obj.getExtendedSwitch(keyCode);
        if (sw>=0 && !obj.switchStates[200 + sw]) {
            obj.switchStates[200 + sw] = true;
            KemperArduino.switchDown(sw + 201);
        }
        //console.log("onKeyDown", keyCode);
        return true;
    },
    onKeyUp: function (obj, e) {
        var keyCode = window.event ? e.keyCode : e.which;
        var sw = obj.getKeySwitch(keyCode);
        if (sw>=0) {
            obj.switchStates[sw] = false;
            KemperArduino.switchUp(sw + 1);
        }
        sw = obj.getExtendedSwitch(keyCode);
        if (sw>=0) {
            obj.switchStates[200 + sw] = false;
            KemperArduino.switchUp(sw + 201);
        }
        return true;
    },
    getKeySwitch: function (keyCode) {
        var keyMap = "QWERTYUIOPASDFGHJKL;";
        var ch = String.fromCharCode(keyCode);
        return keyMap.indexOf(ch);
    },
    getExtendedSwitch: function (keyCode) {
        var keyMap = "ZXCVBNM,./";
        var ch = String.fromCharCode(keyCode);
        return keyMap.indexOf(ch);
    }
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
                if (item.ledData && (led.r > 0 || led.g > 0 || led.b>0))
                    return "rgb("+led.r+", "+led.g+", "+led.b+")";
            }
            return "rgb(127, 127, 127)";
        });
    };
    return item;
}

KemperArduino = {
    socket: null,
    initialize: function () {
        var self = this;
        this.socket = io('http://' + location.hostname + (location.port!=80?":"+location.port:""));
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
            ProcessDisplayData(data.data);
        });
    },
    switchDown: function (idx) {
        this.socket.emit('switchDown', { idx: idx });
    },
    switchUp: function (idx) {
        this.socket.emit("switchUp", { idx: idx });
    },
    expPedal: function (expId, expValue) {
        this.socket.emit("expPedal", { expId: expId, expValue: expValue });
    },
    testMessage: function (p1, p2, p3) {
        this.socket.emit("testMessage", { p1: p1, p2: p2, p3: p3});
    },
};

function ProcessDisplayData(data) {
    function getColor(a) {
        return {
            //css: "rgb(" + ((a>>11)<<3) + "," + (((a>>5)&0x3f)<<2) + "," + ((a&0x1f)<<3) + ")"
            css: "rgb(" + ((a>>10)<<4) + "," + (((a>>5)&0x1f)<<3) + "," + ((a&0x1f)<<3) + ")"
        };
    }
        
    if (data[0] == 1) //drawRect
    {
        if (data[7]) {
            ctx.fillStyle = getColor(data[6]).css;
            if (data[5]) 
                roundRect(ctx, data[1], data[2], data[3] - data[1], data[4]-data[2], data[5], data[7], !data[7]);
            else
                ctx.fillRect(data[1], data[2], data[3] - data[1], data[4]-data[2]);
        } else {
            ctx.lineWidth="0";
            ctx.strokeStyle=getColor(data[6]).css;
            if (data[5]) 
                roundRect(ctx, data[1], data[2], data[3] - data[1], data[4]-data[2], data[5], data[7], !data[7]);
            else {
                ctx.beginPath();
                ctx.rect(data[1], data[2], data[3] - data[1], data[4]-data[2]);
                ctx.stroke();
            }
        }
    } else if (data[0] == 2) { // drawText 
        ctx.font=(data[7]) + "px Verdana";
        ctx.fillStyle = getColor(data[8]).css;
        ctx.textBaseline = "hanging";
        ctx.textAlign = "left";
        var text = "";
        for (var i=0;i<data[9];i++)
            text += String.fromCharCode(data[10+i]);
            
        textArr = text.split(" ");
        var w = data[3];
        var h = data[4];
        var horAlign = data[5];
        var verAlign = data[6];

        if (w == 0 || h == 0)
        {
            drawText(data[1], data[2], text);
        }
        else 
        {
            var curX = 0;
            var curY = 0;
            var words = [];
            var wordH = data[7];
            var lineWidths = [0];
            for (var i=0;i<textArr.length;i++) {
                var word = textArr[i];
                var wordW = ctx.measureText(word).width;
                if (wordW + curX > w && curX > 0) {
                    curX = 0;
                    curY += wordH * 1.2; // pt vs px
                    lineWidths.push(0);
                }
                words.push({x: curX, y: curY, word: word, line: lineWidths.length-1});
                curX += wordW;
                lineWidths[lineWidths.length-1] = curX;
                curX += wordH * 0.3; // letter spacing
            }
            if (verAlign == 1)
                curY = (h - lineWidths.length*wordH*1.2 + wordH*0.25)/2;
            else if  (verAlign == 2)
                curY = (h - lineWidths.length*wordH*1.2 + wordH*0.25);
            else
                curY = 0;
            curY += data[2];
            for (var i=0;i<lineWidths.length;i++) {
                if (horAlign == 1)
                    curX = (w - lineWidths[i])/2;
                else if  (horAlign == 2)
                    curX = (w - lineWidths[i]);
                else
                    curX = 0;
                curX += data[1];
                for (var j=0;j<words.length;j++) {
                    if (words[j].line == i) {
                        drawText(curX + words[j].x, curY + words[j].y, words[j].word);
                    }
                }
                    
            }
        }
        function drawText(x, y, text) {
            ctx.save();
            ctx.translate(x, y);
            //ctx.rotate(Math.PI * data[9] / 18000);
            ctx.fillText(text, 0, 0);
            ctx.restore();
        }
        function roundRect(ctx, x, y, width, height, radius, fill, stroke) {
            if (typeof stroke == "undefined" ) {
                stroke = true;
            }
            if (typeof radius === "undefined") {
                radius = 5;
            }
            ctx.beginPath();
            ctx.moveTo(x + radius, y);
            ctx.lineTo(x + width - radius, y);
            ctx.quadraticCurveTo(x + width, y, x + width, y + radius);
            ctx.lineTo(x + width, y + height - radius);
            ctx.quadraticCurveTo(x + width, y + height, x + width - radius, y + height);
            ctx.lineTo(x + radius, y + height);
            ctx.quadraticCurveTo(x, y + height, x, y + height - radius);
            ctx.lineTo(x, y + radius);
            ctx.quadraticCurveTo(x, y, x + radius, y);
            ctx.closePath();
            if (stroke) {
                ctx.stroke();
            }
            if (fill) {
                ctx.fill();
            }        
        }
            
    } else if (data[0] == 3) //fillTriangle
    {
        var path = new Path2D();
        path.moveTo(data[1], data[2]);
        path.lineTo(data[3], data[4]);
        path.lineTo(data[5], data[6]);
        ctx.fillStyle = getColor(data[7]).css;
        ctx.fill(path);
    }

}

KemperArduino.initialize();



function debug(str) {
    var display = document.getElementById("DebugDisplay");
    var atBottom = display.scrollTop > display.scrollHeight - display.offsetHeight - 10;
    $(display).find("pre").append("<div>"+str+"</div>");
    if (atBottom) {
        display.scrollTop = display.scrollHeight;
    }
}

var orientationReceived = false;
var lastSend = 0;

window.addEventListener("deviceorientation", function (event) {
    if (!orientationReceived && event && event.alpha) {
        orientationReceived = true;
    }
    if (!lastSend || ((new Date()) - lastSend >= 100)) {
        //debug(event.alpha + " " + event.beta + " " + event.gamma);
        var expId = model.curExpPedal();
        var newVal = (event.beta + 30) / 75;
        sendExpValue(expId, 1-newVal);
        lastSend = new Date();
    }
}, true);


var pageX = 0;
var lastVal = 0;
setInterval(function () {
    if (orientationReceived)
        return;
    var expId = model.curExpPedal();
    if (expId > 0) {
        var $cont = $(".ExpressionPedalsValueContainer");
        var o = $cont.offset();
        var newVal = (pageX-o.left) / $cont.width();
        sendExpValue(expId, newVal);
    }
}, 100);

function sendExpValue(expId, newVal) {
    newVal = Math.min(1, newVal);
    newVal = Math.max(0, newVal);
    newVal = Math.floor(newVal * 255)
    if (newVal == 0)
        newVal = 1;

    if (newVal == 13) // cariage return is problematic using getc
        newVal = 14;
    if (newVal == 26) // substitude character is problematic using getc
        newVal = 27;
    if (newVal != lastVal) {
        lastVal = newVal;
        model.curExpPedalValue(newVal / 255);
        KemperArduino.expPedal(expId - 1, newVal);
    }
}

$( document ).on( "mousemove", function( event ) {
    pageX = event.pageX;
});
﻿var SerialPort = require("serialport").SerialPort

var spawn = require('child_process').spawn;
var exec = require('child_process').exec;

var serialPort = null;
var testConsole = null;

//serialPort = new SerialPort("COM12", {baudrate: 115200});
//serialPort = new SerialPort("COM14", {baudrate: 115200});
//serialPort = new SerialPort("COM14", {baudrate: 921600});
//testConsole = spawn('TestKemperWin.exe', [], {encoding: 'binary', maxBuffer: 0});
testConsole = spawn('KemperRemoteDIYWin.exe', ["0"], { encoding: 'binary', maxBuffer: 0 });


if (testConsole) {
    testConsole.stdout.on('data', function (data) {
        //console.log('stdout: ' + data);
        onData(data);
    });
    
    testConsole.stderr.on('data', function (data) {
        console.log('stderr: ' + data);
    });
    
    testConsole.on('close', function (code) {
        console.log('child process exited with code ' + code);
    });
    
    setInterval(function () {
        testConsole.stdin.write(new Buffer([0]));
        //console.log(testConsole.stdin);
        //console.log("This is heartbeat");
    }, 30);
}
var buffer = null;

function onData(data) {
    //console.log("XXX", data.length, data);
    var start = 0;
    for (var i = 0; i < data.length; i++) {
        //console.log("HIIIII", data[i], String.fromCharCode(data[i]));
        //console.log(data[i], data[i]>=0x21, data[i] <= 0x7e);
        if ((data[i] & 0xf0) == 0xf0 && data[i] != 0xff) {
            //start of command
            start = i;
            //console.log("start", i);
        }
        if (data[i] == 0xff) {
            //end of command
            if (buffer != null)
                buffer = Buffer.concat([buffer, data.slice(start, i + 1)]);
            else
                buffer = data.slice(start, i + 1);
            applyCommand(buffer);
            buffer = null;
            start = i + 1;
        }
    }
    if (start < data.length) {
        if (buffer == null)
            buffer = data.slice(start, data.length);
        else
            buffer = Buffer.concat([buffer, data.slice(start, data.length)]);
    }
    
    function applyCommand(data) {
        //console.log("YESYESYE", data.length, data);
        if (data[0] == 0xf0 && data[data.length - 1] == 0xff) { // led info
            //console.log("APPLY");
            var leds = [];
            for (var i = 1; i < data.length - 1; i += 3) {
                /*
                data[i]   : switch stomp idx
                data[i+1] : slot idx (0 for first stomp, 1 for second, others ignored)
                data[i+2] : stomp active
                data[i+3] : color r
                data[i+4] : color g
                data[i+5] : color b
                */
                var info = {};
                info.r = data[i] << 1;
                info.g = data[i + 1] << 1;
                info.b = data[i + 2] << 1;
                leds.push(info); // 6 is the led no
            }
            if (sock) {
                sock.emit("setLeds", leds);
            }
        }
        if (data[0] == 0xf1 && data[data.length - 1] == 0xff) { // string debug
            //console.log('data received: ', data);
            if (sock) {
                var str = "";
                for (var i = 1; i < data.length - 1; i += 1) {
                    str += String.fromCharCode(data[i]);
                }
                sock.emit("debug", { text: str });
            }
        }
        if (data[0] == 0xf2 && data[data.length - 1] == 0xff) {
            if (sock) {
                var arr = [];
                for (var i = 1; i < data.length - 1; i += 1) {
                    //arr.push((data[i]<<4) | data[i+1]);
                    if (data[i] != 13 || data[i + 1] != 10)
                        arr.push(data[i]);
                }
                var arr2 = [];
                /*
                for (var i=0;i<arr.length;i+=4) {
                    arr2.push((arr[i]<<12) | (arr[i+1]<<8) | (arr[i+2]<<4) | arr[i+3]);
                }
                */
                for (var i = 0; i < arr.length; i += 2) {
                    arr2.push((arr[i] << 7) | arr[i + 1]);
                }
                /*
                console.log("Display");
                
                for (var i=0;i<arr2.length;i+=1) {
                    if (arr2[i]>=0x21 && arr2[i] <= 0x7e)
                        console.log(String.fromCharCode(arr2[i]));
                }
                */
                sock.emit("display", { data: arr2 });
            }
        }
    }
    
}

if (serialPort)
    serialPort.on("open", function () {
        console.log('open');
        
        serialPort.on('data', onData);

    });


var sock;

var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);


server.listen(80);

app.use(express.static('./'));

app.get('/', function (req, res) {
    res.sendfile(__dirname + '/index.html');
});

io.on('connection', function (socket) {
    sock = socket;
    socket.on('switchDown', function (data) {
        if (serialPort)
            serialPort.write([1, data.idx, 1], function (err, results) {
            //console.log('switchDown err ' + err);
            //console.log('results ' + results);
            });
        if (testConsole) {
            console.log("switchDown");
            testConsole.stdin.write(new Buffer([1, data.idx, 1]));
        }
    });
    socket.on('switchUp', function (data) {
        if (serialPort)
            serialPort.write(new Buffer([1, data.idx, 0]), function (err, results) {
            });
        if (testConsole) {
            testConsole.stdin.write(new Buffer([1, data.idx, 0]));
            console.log("switchUp");
        }
    });
    socket.on('expPedal', function (data) {
        if (serialPort)
            serialPort.write(new Buffer([2, data.expId + 1, data.expValue >> 2]), function (err, results) {
            });
        if (testConsole) {
            console.log(data.expValue >> 2);
            testConsole.stdin.write(new Buffer([2, data.expId+1, data.expValue >> 2]));
        }
    });
});

process.on('SIGINT', function () {
    console.warn("Caught interrupt signal");
    process.exit();
});



var consts = require("./consts.js");

var warnings = [];

console.log("\n\// Kemper Parameter Option Values");

var maxKemperParamOptionLength = 0;
var maxKemperParamLength = 0;

for (var a in consts.types) {
    var type = consts.types[a];
    type.optionNames = [];
    if (type.options)
    {
        for (var key in type.options) {
            var optionName = "Option_" + a + "_" + key;
            type.optionNames.push(optionName);
            console.log("const KemperParamOption " + optionName + " PROGMEM = {" +key+",\"" +type.options[key] + "\"};");
        }
    }
    maxKemperParamOptionLength = Math.max(type.optionNames.length, maxKemperParamOptionLength);
}

console.log("\n\// Kemper Parameter Option Collections");
for (var a in consts.types) {
    var type = consts.types[a];
    if (type.options)
    {
        var o = "const KemperParamOption * const Options_" + a + "[] PROGMEM = {";
        for (var key in type.optionNames) {
            o += "&" + type.optionNames[key] + ", ";
        }
        console.log(o.substr(0, o.length-2) + "};");
//        console.log("\t{" + type.minValue + "," + type.maxValue + ",{" + o.substr(1) + "}},");
    }
}

console.log("\n\// Kemper Parameters");
for (var a in consts.stompParams) {
    var s = consts.stompParams[a];
    var arr = s.split("|");
    var name = s;
    var type = "f";
    if (arr.length>1) {
        name = arr[1];
        type = arr[0];
    }
    var minValue = 0;
    var maxValue = 100;
    var optionCount = 0;
    if (consts.types[type]) {
        minValue = consts.types[type].minValue;
        maxValue = consts.types[type].maxValue;
        optionCount = consts.types[type].optionNames.length;
    }
    console.log("const PGM_KemperParam Param_" + getVarName(name) + " PROGMEM =  {"+a+", \""+name+"\", " + minValue + ", " + maxValue + ", " + optionCount + ", " + (type!="f"&& consts.types[type]?"Options_" + type:0 )+"};");
}

console.log("\n\// Kemper Stomp Parameters");
var paramNames = {};
for (var a in consts.stomps) {
    var s = consts.stomps[a];
    var o = "const PGM_KemperParam * const Params_" + getVarName(s.name) +"[] PROGMEM = {";
    var hasParam = false;
    var paramCount = 0;
    for (var b in s.params) {
        var pName = consts.stompParams[s.params[b]];
        if (pName)
        {
            var arr = pName.split("|");
            if (arr.length>1) {
                pName = arr[1];
            }
            o += "&Param_" + getVarName(pName) + ", ";
            hasParam = true;
            paramCount++;
        } else {
            warnings.push(["Error!!, No parameter name", b, s.params[b], consts.stompParams[s.params[b]]]);
        }
    }
    s.paramCount = paramCount;
    maxKemperParamLength = Math.max(paramCount, maxKemperParamLength);
    if (hasParam) {
        paramNames[getVarName(s.name)] = 1;
        console.log(o.substr(0, o.length-2) + "};");
    }
}

console.log("\n\// Kemper Stomps");
console.log("const PGM_StompInfo AllStomps[] PROGMEM = {");
for (var a in consts.stomps) {
    var s = consts.stomps[a];
    console.log("\t{" + a + ", Color" + s.color + ",\"" + s.name + "\", " + s.paramCount + ", " + (paramNames[getVarName(s.name)] ? "Params_" + getVarName(s.name) : 0) + ", " + (s.isExpWah ? 1 : 0) + ", " + (s.isExpPitch ? 1 : 0) + "},");
}
console.log("\t{601, ColorGreen,\"Tap Delay\"},");
console.log("\t{602, ColorGreen,\"Free Delay\"},");
console.log("\t{603, ColorGreen,\"Analog Delay\"},");
console.log("\t{701, ColorGreen,\"Hall\"},");
console.log("\t{702, ColorGreen,\"Large Room\"},");
console.log("\t{703, ColorGreen,\"Small Room\"},");
console.log("\t{704, ColorGreen,\"Ambience\"},");
console.log("\t{705, ColorGreen,\"Matchbox\"}, ");
console.log("\t{0, ColorNone,\"None\"},");
console.log("};");

console.log("\n");
console.log("#define MAX_KEMPER_PARAM_OPTION_LENGTH " + maxKemperParamOptionLength);
console.log("#define MAX_KEMPER_PARAM_LENGTH " + maxKemperParamLength);

console.log("\n\n\n");
for (var a in warnings) {
    console.log("// " +warnings[a]);
}

function getVarName(name) {
    return name.replace(/[^0-9a-zA-Z_]/g, "_");
}
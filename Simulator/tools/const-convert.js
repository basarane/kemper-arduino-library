var consts = require("./consts.js");

var warnings = [];

console.log("\n//Kemper Parameter Value Types");

for (var a in consts.stompParamValue) {
    var value = consts.stompParamValue[a];
    if (value) {
        if (value.special)
            value = { minValue: 0, maxValue: 0, suffix: null, exponential: null, special: true };
        console.log("const KemperParamValue " + "Value_" + a + " PROGMEM = {" + a + "," + value.minValue + "," + value.maxValue + "," + (value.maxParam?value.maxParam:0) + "," + (value.suffix ? "\"" + value.suffix + "\"" : 0) + "," + (value.exponential ? "true" : "false") + "," + (value.special ? "true" : "false") + "};");
    }

}

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
    }
}

console.log("\n\// Kemper Parameters");
for (var a in consts.stompParams) {
    var s = consts.stompParams[a];
    if (typeof s === "string")
        s = { "default": s };

    for (var id in s) {
        var arr = s[id].split("|");
        var name = s[id];
        var type = "f";
        if (arr.length>1) {
            name = arr[1];
            type = arr[0];
        }
        var value = parseInt(type);
        var optionCount = 0;
        if (consts.types[type]) {
            optionCount = consts.types[type].optionNames.length;
        }
        console.log("const PGM_KemperParam Param_" + getVarName(name+"_"+ a + "_" +id) + " PROGMEM =  {"+a+", \""+name+"\", " +  optionCount + ", " + (type!="f"&& consts.types[type]?"Options_" + type:0 )+", "+ (!isNaN(value)?"&Value_" + value:0) +"};");
    }
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
            if (typeof pName === "string")
                pName = { "default": pName };

            var param = "";
            var paramKey = "";
            for (var id in pName) {
                if (id.indexOf("," + a + ",") >= 0) {
                    param = pName[id];
                    paramKey = id;
                }
            }
            if (!param) {
                param = pName["default"];
                paramKey = "default";
            }

            if (param)
            {
                var arr = param.split("|");
                if (arr.length > 1) {
                    param = arr[1];
                }
                o += "&Param_" + getVarName(param + "_" + s.params[b] + "_" + paramKey) + ", ";
                hasParam = true;
                paramCount++;
            } else {
                warnings.push(["Error!!, No parameter default", b, s.params[b], consts.stompParams[s.params[b]]]);
            }
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
/*
console.log("\t{201, ColorGreen,\"Tap Delay\"},");
console.log("\t{202, ColorGreen,\"Free Delay\"},");
console.log("\t{203, ColorGreen,\"Analog Delay\"},");
console.log("\t{211, ColorGreen,\"Hall\"},");
console.log("\t{212, ColorGreen,\"Large Room\"},");
console.log("\t{213, ColorGreen,\"Small Room\"},");
console.log("\t{214, ColorGreen,\"Ambience\"},");
console.log("\t{215, ColorGreen,\"Matchbox\"}, "); */
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
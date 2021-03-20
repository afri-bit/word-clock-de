

// Color
var btnSetColorBackground = document.getElementById('btn_set_color_background')
var btnSetColorActiveLetter = document.getElementById('btn_set_color_active_letter')
var btnSetColorMinute = document.getElementById('btn_set_color_minute')
var btnSetColorSecond = document.getElementById('btn_set_color_second')
var btnSetColorAmPm = document.getElementById('btn_set_color_ampm')

var colorBackground = document.getElementById('color_background')
var colorActiveLetter = document.getElementById('color_active_letter')
var colorMinute = document.getElementById('color_minute')
var colorSecond = document.getElementById('color_second')
var colorAmPm = document.getElementById('color_ampm')

// Brightness
var rangeBrightnessMax = document.getElementById('range_brightness_max')
var rangeBrightnessValue = document.getElementById('range_brightness_value')
var checkBrightessAuto = document.getElementById('check_brightness_auto')

// Light
var rangeLightTimeout = document.getElementById('range_light_timeout')
var checkLightAlwaysOn = document.getElementById('check_light_always_on')

var urlSetColorBackground = "/set/color/background"
var urlSetColorActiveLetter = "/set/color/active_letter"
var urlSetColorMinute = "/set/color/minute"
var urlSetColorSecond = "/set/color/second"
var urlSetColorAmPm = "/set/color/ampm"
var urlSetLight = "/set/light"
var urlSetBrightness = "/set/brightness"

var urlGetConfig = "/get/config"
var urlGetColorBackground = "/get/color/background"
var urlGetColorActiveLetter = "/get/color/active_letter"
var urlGetColorMinute = "/get/color/minute"
var urlGetColorSecond = "/get/color/second"
var urlGetColorAmPm = "/get/color/ampm"
var urlGetLight = "/get/light"
var urlGetBrightness = "/get/brightness"


// ============================================================================
// Color
// ============================================================================

function componentToHex(c) {
    var hex = c.toString(16)
    return hex.length == 1 ? "0" + hex : hex
}

function colorRgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b)
}

function colorHexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex)
    return result ? {
        red: parseInt(result[1], 16),
        green: parseInt(result[2], 16),
        blue: parseInt(result[3], 16)
    } : null
}

function initUI(data) {

    // Set UI background color
    colorBackground.value = colorRgbToHex(data.color.background.red, data.color.background.green, data.color.background.blue)

    // Set UI active letter color
    colorActiveLetter.value = colorRgbToHex(data.color.active_letter.red, data.color.active_letter.green, data.color.active_letter.blue)

    // Set UI minute color
    colorMinute.value = colorRgbToHex(data.color.minute.red, data.color.minute.green, data.color.minute.blue)

    // Set UI second color
    colorSecond.value = colorRgbToHex(data.color.second.red, data.color.second.green, data.color.second.blue)

    // Set UI ampm color
    colorAmPm.value = colorRgbToHex(data.color.ampm.red, data.color.ampm.green, data.color.ampm.blue)

    // Set UI brightness 
    checkBrightessAuto.checked = data.brightness.auto
    rangeBrightnessMax.value = data.brightness.max
    rangeBrightnessValue.value = data.brightness.value
    rangeBrightnessValue.disabled = data.brightness.auto

    // Set UI Light
    checkLightAlwaysOn.checked = data.light.always_on
    rangeLightTimeout.value = data.light.timeout
    rangeLightTimeout.disabled = data.light.always_on

}

btnSetColorBackground.onclick = function () {
    var xhr = new XMLHttpRequest()
    xhr.open("POST", urlSetColorBackground, true)
    color = colorHexToRgb(colorBackground.value)
    var data = JSON.stringify(color)
    xhr.send(data)
}

btnSetColorActiveLetter.onclick = function () {

    var xhr = new XMLHttpRequest()
    xhr.open("POST", urlSetColorActiveLetter, true)
    color = colorHexToRgb(colorActiveLetter.value)
    var data = JSON.stringify(color)
    xhr.send(data)
}

btnSetColorMinute.onclick = function () {
    var xhr = new XMLHttpRequest()
    xhr.open("POST", urlSetColorMinute, true)
    color = colorHexToRgb(colorMinute.value)
    var data = JSON.stringify(color)
    xhr.send(data)
}

btnSetColorSecond.onclick = function () {
    var xhr = new XMLHttpRequest()
    xhr.open("POST", urlSetColorSecond, true)
    color = colorHexToRgb(colorSecond.value)
    var data = JSON.stringify(color)
    xhr.send(data)
}

btnSetColorAmPm.onclick = function () {
    var xhr = new XMLHttpRequest()
    xhr.open("POST", urlSetColorAmPm, true)
    color = colorHexToRgb(colorAmPm.value)
    var data = JSON.stringify(color)
    xhr.send(data)
}

// ============================================================================
// Brightness
// ============================================================================

function sendBrightnessInfo() {
    var xhr = new XMLHttpRequest()
    xhr.open("POST", urlSetBrightness, true)
    var data = JSON.stringify({ "auto": checkBrightessAuto.checked, "max": parseInt(rangeBrightnessMax.value), "value": parseInt(rangeBrightnessValue.value) })
    xhr.send(data)
}

rangeBrightnessMax.oninput = function () {
    if (rangeBrightnessMax.value < 20) {
        rangeBrightnessMax.value = 20
    }
}

rangeBrightnessValue.oninput = function () {
    if (rangeBrightnessValue.value < 10) {
        rangeBrightnessValue.value = 10
    }
}

checkBrightessAuto.onchange = function () {
    if (checkBrightessAuto.checked) {
        rangeBrightnessValue.disabled = true
    } else {
        rangeBrightnessValue.disabled = false
    }

    sendBrightnessInfo()
}

rangeBrightnessMax.onmouseup = function () {
    sendBrightnessInfo()
}

rangeBrightnessValue.onmouseup = function () {
    sendBrightnessInfo()
}

// ============================================================================
// Light
// ============================================================================

function sendLightInfo() {
    var xhr = new XMLHttpRequest()
    xhr.open("POST", urlSetLight, true)
    var data = JSON.stringify({ "always_on": checkLightAlwaysOn.checked, "timeout": parseInt(rangeLightTimeout.value) })
    xhr.send(data)
}

checkLightAlwaysOn.onchange = function () {
    if (checkLightAlwaysOn.checked) {
        rangeLightTimeout.disabled = true
    } else {
        rangeLightTimeout.disabled = false
    }

    sendLightInfo()
}
rangeLightTimeout.onmouseup = function () {
    sendLightInfo()
}

// ============================================================================
// Startup
// ============================================================================
window.onload = function () {

    var request = new XMLHttpRequest()

    request.open('GET', '/get/config', true)

    request.onload = function () {
        var data = JSON.parse(this.response)
        initUI(data)
    }

    request.send()
}
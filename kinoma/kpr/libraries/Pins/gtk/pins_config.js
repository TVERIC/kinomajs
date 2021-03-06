//@module
/*
 *     Copyright (C) 2010-2016 Marvell International Ltd.
 *     Copyright (C) 2002-2010 Kinoma, Inc.
 *
 *     Licensed under the Apache License, Version 2.0 (the "License");
 *     you may not use this file except in compliance with the License.
 *     You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */
 
var Pins = require("pins");


exports.configurationToMux = function(config)
{
	var mux = {
			leftPins: makeDisconnectedHeader(8),
			rightPins: makeDisconnectedHeader(8),
			back: makeBackHeader(),
			leftVoltage: ("leftVoltage" in config) ? config.leftVoltage : undefined,
			rightVoltage: ("rightVoltage" in config) ? config.rightVoltage : undefined
	};

	for (var bll in config) {
		if ((bll == "leftVoltage") || (bll == "rightVoltage"))
			continue;
		if (!("pins" in config[bll]))
			continue;
		var pins = config[bll].pins;
		for (var pin in pins) {
			pin = pins[pin];
			switch (pin.type) {
				case "Digital":
					setPin(mux, pin.pin, ("output" == pin.direction) ? Pins.digitalOut : Pins.digitalIn);
					break;
				case "A2D":
				case "Analog":
					setPin(mux, pin.pin, Pins.analogIn);
					break;
				case "I2C":
					setPin(mux, pin.sda, Pins.i2cData);
					setPin(mux, pin.clock, Pins.i2cClock);
					break;
				case "Serial":
					if ("rx" in pin) setPin(mux, pin.rx, Pins.serialRx);
					if ("tx" in pin) setPin(mux, pin.tx, Pins.serialTx);
					break;
				case "PWM":
					setPin(mux, pin.pin, Pins.pwm);
					break;
				case "Power":
					if ((5 != pin.voltage) && (3.3 != pin.voltage)) throw new Error("unsupported voltage " + pin.voltage)
					setPin(mux, pin.pin, (5 == pin.voltage) ? Pins.power5V : Pins.power3_3V);
					break;
				case "Ground":
					setPin(mux, pin.pin, Pins.ground);
					break;
				case "Audio":
					break;
				default:
					throw new Error("Unrecognized pin type " + pin.type);
					break;
			}
		}
	}

	if (undefined === mux.leftVoltage)
		mux.leftVoltage = 3.3;
	if (undefined === mux.rightVoltage)
		mux.rightVoltage = 3.3;

	return mux;
}

function setPin(mux, pin, type)
{
	if ((51 <= pin) && (pin <= 58)) {
		validatePinAssignment(pin, mux.leftPins[pin - 51], type);
		if ((Pins.power3_3V == type) || (Pins.power5V == type)) {
			var leftVoltage = (Pins.power3_3V == type) ? 3.3 : 5;
			if (undefined === mux.leftVoltage)
				mux.leftVoltage = leftVoltage;
			else if (mux.leftVoltage !== leftVoltage)
				throw new Error("Cannot assign two different voltages to pins in left header");
			type = Pins.power3_3V;
		}
		mux.leftPins[pin - 51] = type;
		switch (pin - 51) {		// mirror front left pins to back
			case 0:	mux.back[37] = type; break;
			case 1:	mux.back[36] = type; break;
			case 2:	mux.back[39] = type; break;
			case 3:	mux.back[40] = type; break;
			case 4:	mux.back[43] = type; break;
			case 5:	mux.back[42] = type; break;
			case 6:	mux.back[47] = type; break;
			case 7:	mux.back[46] = type; break;
		}
	}
	else
	if ((59 <= pin) && (pin <= 66)) {
		validatePinAssignment(pin, mux.rightPins[pin - 59], type);
		if ((Pins.power3_3V == type) || (Pins.power5V == type)) {
			var rightVoltage = (Pins.power3_3V == type) ? 3.3 : 5;
			if (undefined === mux.rightVoltage)
				mux.rightVoltage = rightVoltage;
			else if (mux.rightVoltage !== rightVoltage)
				throw new Error("Cannot assign two different voltages to pins in right header");
			type = Pins.power3_3V;
		}
		mux.rightPins[pin - 59] = type;
	}
	else
	if ((1 <= pin) && (pin <= 50)) {
		validatePinAssignment(pin, mux.back[pin - 1], type);
		mux.back[pin - 1] = type;
		switch (pin - 51) {		// mirror back pins to front left
			case 37: mux.leftPins[0] = type; break;
			case 36: mux.leftPins[1] = type; break;
			case 39: mux.leftPins[2] = type; break;
			case 40: mux.leftPins[3] = type; break;
			case 43: mux.leftPins[4] = type; break;
			case 42: mux.leftPins[5] = type; break;
			case 47: mux.leftPins[6] = type; break;
			case 46: mux.leftPins[7] = type; break;
		}
	}
}

function validatePinAssignment(pin, from, to)
{
	if (from == to)
		return;

	if ((from == Pins.digitalUnconfigured) &&
		((to == Pins.digitalIn) || (to == Pins.digitalOut)))
		return;

	if (from == Pins.disconnected)
		return;

	throw new Error("Cannot configure pin " + pin + ", already assigned.");
}

/* currently unused

function normalizeFrontConfig(config)
{
	if (!config) config = new Array;
	config = config.map(function(pin) {
		switch (pin) {
			case Pins.power5V:
			case Pins.power3_3V:
				return Pins.power3_3V;
			case Pins.ground:
			case Pins.analogIn:
			case Pins.digitalIn:
			case Pins.digitalOut:
			case Pins.i2cClock:
			case Pins.i2cData:
			case Pins.pwm:
				return pin;
			default:
				return Pins.disconnected; 
		}
	}, exports);

	while (config.length < 8)
		config.push(Pins.disconnected);

	return config;
}
*/

function makeBackHeader(count)
{
	var header = makeDisconnectedHeader(51);
	
	for (var i = 1; i <= 24; i++)
		header[i] = Pins.digitalUnconfigured;

	header[1] = header[2] =
	header[13] = header[14] =
	header[26] = header[32] =
	header[35] = header[36] =
	header[41] = header[42] =
	header[45] = header[46] = Pins.ground;

	header[25] = Pins.power3_3V;
	header[49] = Pins.power3_3V;
	header[50] = Pins.power5V;

	header[28] = header[30] = header[34] = Pins.pwm;

	header[31] = Pins.serialTx;
	header[33] = Pins.serialRx;

	header[27] = Pins.i2cData;
	header[29] = Pins.i2cClock;

	header.shift();
	
	return header;
}

function makeDisconnectedHeader(count)
{
	var header = new Array(count);
	for (var i = 0; i < count; i++)
		header[i] = Pins.disconnected;
	return header;
}

exports.getFixed = function(directions)
{	
	if (undefined == directions)
		directions = new Array(50);
	var fixedPins = [
		{ pin: 1, type: "Ground" },
		{ pin: 2, type: "Ground" },
		{ pin: 3, type: "Digital", direction: directions[3] },
		{ pin: 4, type: "Digital", direction: directions[4] },
		{ pin: 5, type: "Digital", direction: directions[5] },
		{ pin: 6, type: "Digital", direction: directions[6] },
		{ pin: 7, type: "Digital", direction: directions[7] },
		{ pin: 8, type: "Digital", direction: directions[8] },
		{ pin: 9, type: "Digital", direction: directions[9] },
		{ pin: 10, type: "Digital", direction: directions[10] },
		{ pin: 11, type: "Digital", direction: directions[11] },
		{ pin: 12, type: "Digital", direction: directions[12] },
		{ pin: 13, type: "Ground" },
		{ pin: 14, type: "Ground" },
		{ pin: 15, type: "Digital", direction: directions[15] },
		{ pin: 16, type: "Digital", direction: directions[16] },
		{ pin: 17, type: "Digital", direction: directions[17] },
		{ pin: 18, type: "Digital", direction: directions[18] },
		{ pin: 19, type: "Digital", direction: directions[19] },
		{ pin: 20, type: "Digital", direction: directions[20] },
		{ pin: 21, type: "Digital", direction: directions[21] },
		{ pin: 22, type: "Digital", direction: directions[22] },
		{ pin: 23, type: "Digital", direction: directions[23] },
		{ pin: 24, type: "Digital", direction: directions[24] },
		{ pin: 25, type: "Power", voltage: 3.3 },
		{ pin: 26, type: "Ground" },
		{ pin: 27, type: "I2CData" },
		{ pin: 28, type: "PWM" },
		{ pin: 29, type: "I2CClock" },
		{ pin: 30, type: "PWM" },
		{ pin: 31, type: "UartTX" },
		{ pin: 32, type: "Ground" },
		{ pin: 33, type: "UartRX" },
		{ pin: 34, type: "PWM" },
		{ pin: 35, type: "Ground" },
		{ pin: 36, type: "Ground" },
		{ pin: 37, type: "Mirrored" },
		{ pin: 38, type: "Mirrored" },
		{ pin: 39, type: "Mirrored" },
		{ pin: 40, type: "Mirrored" },
		{ pin: 41, type: "Ground" },
		{ pin: 42, type: "Ground" },
		{ pin: 43, type: "Mirrored" },
		{ pin: 44, type: "Mirrored" },
		{ pin: 45, type: "Ground" },
		{ pin: 46, type: "Ground" },
		{ pin: 43, type: "Mirrored" },
		{ pin: 44, type: "Mirrored" },
		{ pin: 49, type: "Power", voltage: 3.3 },
		{ pin: 50, type: "Power", voltage: 5 },
		
		{ pin: 51, type: "Left-Front" },
		{ pin: 52, type: "Left-Front" },
		{ pin: 53, type: "Left-Front" },
		{ pin: 54, type: "Left-Front" },
		{ pin: 55, type: "Left-Front" },
		{ pin: 56, type: "Left-Front" },
		{ pin: 57, type: "Left-Front" },
		{ pin: 58, type: "Left-Front" },
		{ pin: 59, type: "Right-Front" },
		{ pin: 60, type: "Right-Front" },
		{ pin: 61, type: "Right-Front" },
		{ pin: 62, type: "Right-Front" },
		{ pin: 63, type: "Right-Front" },
		{ pin: 64, type: "Right-Front" },
		{ pin: 65, type: "Right-Front" },
		{ pin: 66, type: "Right-Front" },
	];
	return fixedPins;
}

exports.hasMuxablePins = function()
{
	return true;
}

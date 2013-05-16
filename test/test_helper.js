/*
 * モジュールのテスト.
 */
var QUnit = require('qunitjs');
var qunitTap = require('qunit-tap').qunitTap;
var util = require('util');

qunitTap(QUnit, util.puts, {noPlan: true});

QUnit.init();
QUnit.config.updateRate = 0;

exports.QUnit = QUnit;
exports.assert = QUnit;
